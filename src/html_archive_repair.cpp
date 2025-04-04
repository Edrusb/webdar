/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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


    // webdar headers
#include "webdar_css_style.hpp"


    //
#include "html_archive_repair.hpp"

using namespace std;

html_archive_repair::html_archive_repair():
    repair_dest("Where to create the repaired backup",
		"/",
		"",
		webdar_css_style::width_100vw,
		"Select path where will be created the repaired backup"),
    basename("Repaired backup basename",
	     html_form_input::text,
	     "",
	     "",
	     webdar_css_style::width_100vw_3em),
    repair_fs(""),
    repair_form("Update"),
    need_entrepot_update(false)
{

	// components initialization

    static const char* sect_repair = "repair";
    static const char* sect_options = "options";

    opt_repair.reset(new (nothrow) html_options_repair());
    if(! opt_repair)
	throw exception_memory();

    deroule.add_section(sect_repair, "Repairing Parameters");
    deroule.add_section(sect_options, "Repairing Options");
    deroule.set_active_section(0);

    repair_dest.set_select_mode(html_form_input_file::select_dir);
    repair_dest.set_can_create_dir(true);

    if(repoxfer.get_html_user_interaction())
	repoxfer.get_html_user_interaction()->auto_hide(true, false);
    else
	throw WEBDAR_BUG;

	// adoption tree

    repair_fs.adopt(&repair_dest);
    repair_fs.adopt(&basename);
    repair_form.adopt(&repair_fs);
    deroule.adopt_in_section(sect_repair, &repair_form);
    deroule.adopt_in_section(sect_options, &guichet_opt_repair);
    adopt(&deroule);
    adopt(&repoxfer);

	// events

    opt_repair->record_actor_on_event(this, html_options_repair::entrepot_changed);
    opt_repair->record_actor_on_event(this, html_options_repair::landing_path_changed);
    basename.record_actor_on_event(this, html_form_input::changed);

	// visibility
    repoxfer.set_visible(false);

	// css

    webdar_css_style::normal_button(deroule, true);
}

const string & html_archive_repair::get_archive_basename() const
{
    if(basename.get_value().empty())
    {
	const_cast<html_archive_repair*>(this)->basename.box_set_css_class(webdar_css_style::red_border);
	throw exception_range("Archive basename cannot be an empty string");
    }
    return basename.get_value();
}

void html_archive_repair::set_biblio(const shared_ptr<bibliotheque> & ptr)
{
    opt_repair->set_biblio(ptr);

    guichet_opt_repair.set_child(ptr,
				 bibliotheque::confrepair,
				 opt_repair,
				 false);

    if(ptr->has_config(bibliotheque::confrepair, bibliotheque::default_config_name))
    {
	try
	{
	    guichet_opt_repair.load_from_bibliotheque(bibliotheque::default_config_name);
	}
	catch(...)
	{
		// ignore exception here, as this is called from constructor
		// of the session components and nothing is yet available to
		// display execeptions messages (due to old config in configuration file
		// or corrupted configuration file we read from)
		// The consequence is that we have a cleared configuration:
	    guichet_opt_repair.clear_json();
	}
    }
}


void html_archive_repair::on_event(const string & event_name)
{
    if(event_name == html_options_repair::entrepot_changed)
    {
	need_entrepot_update = true;
    }
    else if(event_name == html_options_repair::landing_path_changed)
    {
	repair_dest.set_value(opt_repair->get_landing_path());
    }
    else if(event_name == html_form_input::changed)
    {
	if(basename.get_value().empty())
	    basename.box_set_css_class(webdar_css_style::red_border);
	else
	    basename.box_set_css_class("");
    }
    else
	throw WEBDAR_BUG;
}

string html_archive_repair::inherited_get_body_part(const chemin & path,
						    const request & req)
{
    if(need_entrepot_update)
	update_entrepot();
    return get_body_part_from_all_children(path, req);
}

void html_archive_repair::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}

void html_archive_repair::inherited_run()
{
    shared_ptr<html_web_user_interaction> ptr(repoxfer.get_html_user_interaction());

    if(!ptr)
	throw WEBDAR_BUG;
    repair_dest.set_entrepot(opt_repair->get_entrepot(ptr));
}

void html_archive_repair::signaled_inherited_cancel()
{
    pthread_t libdar_tid;
    libdar::thread_cancellation th;

    if(is_running(libdar_tid))
	th.cancel(libdar_tid, true, 0);
}

void html_archive_repair::update_entrepot()
{
    if(is_running())
	throw WEBDAR_BUG;
    repoxfer.set_visible(true);
    need_entrepot_update = false;
    repoxfer.run_and_control_thread(this);
}
