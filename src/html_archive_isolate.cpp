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
#include "tooltip_messages.hpp"

    //
#include "html_archive_isolate.hpp"

using namespace std;

html_archive_isolate::html_archive_isolate():
    form("Update"),
    fs(""),
    sauv_path("Where to create the isolated catalog",
	      "/",
	      "",
	      webdar_css_style::width_100vw,
	      "Select the directory where to create the isolated catalog..."),
    basename("Isolated Catalog basename", html_form_input::text, "", "", webdar_css_style::width_100vw_3em),
    need_entrepot_update(false)
{
    static const char* sect_archive = "isolation";
    static const char* sect_options = "options";

#ifdef LIBTHREADAR_STACK_FEATURE
    set_stack_size(DEFAULT_STACK_SIZE);
#endif

    options.reset(new (nothrow) html_options_isolate());
    if(! options)
	throw exception_memory();

    deroule.add_section(sect_archive, "Isolation Parameters");
    deroule.add_section(sect_options, "Isolation Options");
    deroule.set_active_section(0);

    sauv_path.set_select_mode(html_form_input_file::select_dir);
    sauv_path.set_can_create_dir(true);
    if(repoxfer.get_html_user_interaction())
	repoxfer.get_html_user_interaction()->auto_hide(true, false);
    else
	throw WEBDAR_BUG;

	    // adoption tree

    fs.adopt(&sauv_path);
    fs.adopt(&basename);
    form.adopt(&fs);
    deroule.adopt_in_section(sect_archive, &form);
    deroule.adopt_in_section(sect_options, &guichet_options);
    adopt(&deroule);
    adopt(&repoxfer);

	// events

    options->record_actor_on_event(this, html_options_isolate::entrepot_changed);
    options->record_actor_on_event(this, html_options_isolate::landing_path_changed);
    basename.record_actor_on_event(this, html_form_input::changed);

	/// visibility

    repoxfer.set_visible(false);

	// css

    webdar_css_style::normal_button(deroule, true);

	// tooltips
    sauv_path.set_tooltip(TOOLTIP_HAI_SAUV_PATH);
    basename.set_tooltip(TOOLTIP_HAI_BASENAME);
}

void html_archive_isolate::set_biblio(const shared_ptr<bibliotheque> & ptr)
{
    options->set_biblio(ptr);

    guichet_options.set_child(ptr,
			      bibliotheque::confisolate,
			      options,
			      false);

    if(ptr->has_config(bibliotheque::confisolate, bibliotheque::default_config_name))
    {
	try
	{
	    guichet_options.load_from_bibliotheque(bibliotheque::default_config_name);
	}
	catch(...)
	{
		// ignore exception here, as this is called from constructor
		// of the session components and nothing is yet available to
		// display execeptions messages (due to old config in configuration file
		// or corrupted configuration file we read from)
		// The consequence is that we have a cleared configuration:
	    guichet_options.clear_json();
	}
    }
}

const string & html_archive_isolate::get_archive_basename() const
{
    if(basename.get_value().empty())
    {
	const_cast<html_archive_isolate*>(this)->basename.box_set_css_class(webdar_css_style::red_border);
	throw exception_range("Archive basename cannot be an empty string");
    }
    return basename.get_value();
}


void html_archive_isolate::on_event(const string & event_name)
{
    if(event_name == html_options_isolate::entrepot_changed)
    {
	need_entrepot_update = true;
    }
    else if(event_name == html_options_isolate::landing_path_changed)
    {
	sauv_path.set_value(options->get_landing_path());
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

string html_archive_isolate::inherited_get_body_part(const chemin & path,
						     const request & req)
{
    if(need_entrepot_update)
	update_entrepot();
    return get_body_part_from_all_children(path, req);
}

void html_archive_isolate::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}

void html_archive_isolate::inherited_run()
{
    shared_ptr<html_web_user_interaction> ptr(repoxfer.get_html_user_interaction());

    if(!ptr)
	throw WEBDAR_BUG;
    sauv_path.set_entrepot(options->get_entrepot(ptr));
}

void html_archive_isolate::signaled_inherited_cancel()
{
    pthread_t libdar_tid;
    libdar::thread_cancellation th;

    if(is_running(libdar_tid))
	th.cancel(libdar_tid, true, 0);
}

void html_archive_isolate::update_entrepot()
{
    if(is_running())
	throw WEBDAR_BUG;
    repoxfer.set_visible(true);
    need_entrepot_update = false;
    repoxfer.run_and_control_thread(this);
}
