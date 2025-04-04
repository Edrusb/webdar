/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2024 Denis Corbin
//
// This file is part of Webdar
//
//  Webdar is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Webdar is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Webdar.  If not, see <http://www.gnu.org/licenses/>
//
//----
//  to contact the author: dar.linux@free.fr
/*********************************************************************/

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/tools.hpp>

    // webdar headers
#include "chemin.hpp"
#include "webdar_css_style.hpp"
#include "environment.hpp"

    //
#include "html_archive_read.hpp"

using namespace std;

const string html_archive_read::changed = "haread-changed";

html_archive_read::html_archive_read(const string & archive_description):
    form("Update"),
    fs(archive_description),
    arch_path("Backup Path",
	      "/",
	      "",
	      webdar_css_style::width_80vw,
	      "Select the backup to read..."),
    need_entrepot_update(false),
    ignore_events_st(event_propagated)
{
    opt_read.reset(new (nothrow) html_options_read());
    if(! opt_read)
	throw exception_memory();

	// components setup

    static const char* sect_archive = "archive";
    static const char* sect_options = "options";

    deroule.add_section(sect_archive, "Read Parameters");
    deroule.add_section(sect_options, "Read Options");
    deroule.set_active_section(0);

	// web components layout

    fs.adopt(&arch_path);
    form.adopt(&fs);
    deroule.adopt_in_section(sect_archive, &form);
    deroule.adopt_in_section(sect_options, &guichet_opt_read);
    adopt(&deroule);
    adopt(&libdarexec);

	// events and actor
    opt_read->record_actor_on_event(this, html_options_read::entrepot_has_changed);
    opt_read->record_actor_on_event(this, html_options_read::changed);
    opt_read->record_actor_on_event(this, html_options_read::landing_path_changed);
    arch_path.record_actor_on_event(this, html_form_input_file::changed_event);
    libdarexec.record_actor_on_event(this, html_libdar_running_popup::libdar_has_finished);
    register_name(changed);

	// initial values
    arch_path.set_select_mode(html_form_input_file::select_slice);
    libdarexec.set_visible(false);

	// css
    webdar_css_style::normal_button(deroule, true);

}

void html_archive_read::set_biblio(const shared_ptr<bibliotheque> & ptr)
{
    opt_read->set_biblio(ptr);
    guichet_opt_read.set_child(ptr,
			       bibliotheque::confread,
			       opt_read,
			       false);

    if(ptr->has_config(bibliotheque::confread, bibliotheque::default_config_name))
    {
	try
	{
	    guichet_opt_read.load_from_bibliotheque(bibliotheque::default_config_name);
	}
	catch(...)
	{
		// ignore exception here, as this is called from constructor
		// of the session components and nothing is yet available to
		// display execeptions messages (due to old config in configuration file
		// or corrupted configuration file we read from)
		// The consequence is that we have a cleared configuration:
	    guichet_opt_read.clear_json();
	}
    }
}

void html_archive_read::load_json(const json & source)
{
    try
    {
	unsigned int version;
	string class_id;
	json config = unwrap_config_from_json_header(source,
						     version,
						     class_id);

	if(class_id != myclass_id)
	    throw exception_range(libdar::tools_printf("Unexpected class_id in json data, found %s while expecting %s",
						       class_id.c_str(),
						       myclass_id));

	if(version > format_version)
	    throw exception_range(libdar::tools_printf("Json format version too hight for %s, upgrade your webdar software",
						       myclass_id));

	ignore_events(true);
	try
	{
	    arch_path.set_value(config.at(jlabel_path));
	    guichet_opt_read.load_json(config.at(jlabel_opt_read));
	}
	catch(...)
	{
	    ignore_events(false);
	    throw;
	}
	ignore_events(false);
    }
    catch(json::exception & e)
    {
	throw exception_json(libdar::tools_printf("Error loading %s config", myclass_id), e);
    }
}

json html_archive_read::save_json() const
{
    json config;

    config[jlabel_path] = arch_path.get_value();
    config[jlabel_opt_read] = guichet_opt_read.save_json();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					config);
}

void html_archive_read::clear_json()
{
    ignore_events(true);
    try
    {
	arch_path.set_value("");
	guichet_opt_read.clear_json();
    }
    catch(...)
    {
	ignore_events(false);
	throw;
    }
    ignore_events(false);
}

void html_archive_read::on_event(const string & event_name)
{
    if(event_name == html_options_read::entrepot_has_changed)
    {
	if(!is_running())
	{
	    need_entrepot_update = true;
	    my_body_part_has_changed(); // force rerun of inherited_body_builder to update entrepot
	}
	else
	{
	    throw WEBDAR_BUG;
		// should not succeed as the popup
		// should display during the subthread running
		// and only vanish once the subtread has completed
		// while the popup forbids any change of entrepot configuration
	}
    }
    else if(event_name == html_form_input_file::changed_event)
    {
	if(! arch_path.get_min_digits().empty())
	    opt_read->set_src_min_digits(arch_path.get_min_digits());
	trigger_changed();
    }
    else if(event_name == html_libdar_running_popup::libdar_has_finished)
    {
	libdarexec.set_visible(false); // now hiding the popup
	my_body_part_has_changed();
	join();
    }
    else if(event_name == html_options_read::changed)
    {
	trigger_changed();
    }
    else if(event_name == html_options_read::landing_path_changed)
    {
	arch_path.set_value(opt_read->get_landing_path());
    }
    else
	throw WEBDAR_BUG;
}

string html_archive_read::inherited_get_body_part(const chemin & path,
						  const request & req)
{
    if(need_entrepot_update)
	update_entrepot();
    return get_body_part_from_all_children(path, req);
}

void html_archive_read::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}

string html_archive_read::get_archive_path() const
{
    chemin chem(arch_path.get_value());

    if(chem.size() >= 1)
    {
	chem.pop_back();
	return chem.display();
    }
    else
	return "";
}

string html_archive_read::get_archive_basename() const
{
    string ret;
    chemin chem(arch_path.get_value());

    if(chem.size() > 0)
	ret = chem.back();
    else
	ret = arch_path.get_value();

    return ret;
}

void html_archive_read::inherited_run()
{
    arch_path.set_entrepot(opt_read->get_entrepot(libdarexec.get_html_user_interaction()));
}

void html_archive_read::signaled_inherited_cancel()
{
    pthread_t libdar_tid;
    libdar::thread_cancellation th;

    if(is_running(libdar_tid))
	th.cancel(libdar_tid, true, 0);
}

void html_archive_read::update_entrepot()
{
    shared_ptr<html_web_user_interaction> webui(libdarexec.get_html_user_interaction());
    if(!webui)
	throw WEBDAR_BUG;
    webui->clear();
    webui->auto_hide(true, true);

    if(is_running())
	throw WEBDAR_BUG;
    libdarexec.set_visible(true);
    need_entrepot_update = false;
    libdarexec.run_and_control_thread(this);
}

void html_archive_read::trigger_changed()
{
    switch(ignore_events_st)
    {
    case event_propagated:
	act(changed);
	break;
    case event_ignored:
	ignore_events_st = event_pending;
	break;
    case event_pending:
	break;
    default:
	throw WEBDAR_BUG;
    }
}

void html_archive_read::ignore_events(bool mode)
{
    switch(ignore_events_st)
    {
    case event_propagated:
	if(mode)
	    ignore_events_st = event_ignored;
	break;
    case event_ignored:
	if(! mode)
	    ignore_events_st = event_propagated;
	break;
    case event_pending:
	if(mode)
	    throw WEBDAR_BUG;
	ignore_events_st = event_propagated;
	trigger_changed();
	break;
    default:
	throw WEBDAR_BUG;
    }
}
