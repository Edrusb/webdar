/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2023 Denis Corbin
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
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "webdar_css_style.hpp"


    //
#include "html_archive_isolate.hpp"

using namespace std;

html_archive_isolate::html_archive_isolate():
    form("Update"),
    fs("Isolated catalog to create"),
    sauv_path("Where to create the isolated catalogue", "/", 20, "Select the directory where to create the isolated catalog..."),
    basename("Archive basename", html_form_input::text, "", 10)
{
    static const char* sect_archive = "isolation options";
    deroule.add_section(sect_archive, "Archive Isolation");
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
    adopt(&deroule);
    adopt(&options);
    adopt(&repoxfer);

	// events
    options.record_actor_on_event(this, html_options_isolate::entrepot_changed);

	/// visibility
    repoxfer.set_visible(false);

	// css

    webdar_css_style::normal_button(deroule, true);
}

void html_archive_isolate::on_event(const string & event_name)
{
    if(event_name == html_options_isolate::entrepot_changed)
    {
	repoxfer.set_visible(true);
	repoxfer.run_and_control_thread(this);
    }
    else if(event_name == html_libdar_running_popup::libdar_has_finished)
    {
	repoxfer.set_visible(false);
    }
    else
	throw WEBDAR_BUG;
}

string html_archive_isolate::inherited_get_body_part(const chemin & path,
						     const request & req)
{
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
    sauv_path.set_entrepot(options.get_entrepot(ptr));
}

void html_archive_isolate::signaled_inherited_cancel()
{
    pthread_t libdar_tid;
    libdar::thread_cancellation th;

    if(is_running(libdar_tid))
	th.cancel(libdar_tid, true, 0);
}
