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


    // webdar headers
#include "webdar_css_style.hpp"


    //
#include "html_archive_repair.hpp"

using namespace std;

html_archive_repair::html_archive_repair():
    repair_dest("Where to create the repaired backup",
		"/",
		"80%",
		"Select path where will be created the repaired backup"),
    basename("Repaired backup basename",
	     html_form_input::text,
	     "",
	     "80%"),
    repair_fs(""),
    repair_form("Update")
{

	// components initialization

    static const char* sect_repair = "repair";
    deroule.add_section(sect_repair, "Repairing parameters");
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
    deroule.adopt_in_section(sect_repair, &opt_repair);
    adopt(&deroule);
    adopt(&repoxfer);

	// events

    opt_repair.record_actor_on_event(this, html_options_repair::entrepot_changed);


	// visibility
    repoxfer.set_visible(false);

	// css

    webdar_css_style::normal_button(deroule, true);
}

void html_archive_repair::on_event(const string & event_name)
{
    if(event_name == html_options_repair::entrepot_changed)
    {
	if(is_running())
	    throw WEBDAR_BUG;
	join(); // in case a previous execution triggered an exception
	repoxfer.set_visible(true);
	repoxfer.run_and_control_thread(this);
    }
    else
	throw WEBDAR_BUG;
}

string html_archive_repair::inherited_get_body_part(const chemin & path,
						    const request & req)
{
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
    repair_dest.set_entrepot(opt_repair.get_entrepot(ptr));
}

void html_archive_repair::signaled_inherited_cancel()
{
    pthread_t libdar_tid;
    libdar::thread_cancellation th;

    if(is_running(libdar_tid))
	th.cancel(libdar_tid, true, 0);
}
