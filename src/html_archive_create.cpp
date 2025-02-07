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
#include "html_archive_create.hpp"

using namespace std;

html_archive_create::html_archive_create():
    form("Update"),
    fs(""),
    fs_root("Directory to take as root for the backup", "/", "80%", "Select root directory to backup..."),
    sauv_path("Where to create the backup", "/", "80%", "Select path where to create the backup..."),
    basename("Backup basename", html_form_input::text, "", "80%"),
    need_entrepot_update(false)
{
    static const char* sect_archive = "archive";
    static const char* sect_options = "options";

    options.reset(new (nothrow) html_options_create());
    if(! options)
	throw exception_memory();

    deroule.add_section(sect_archive, "Backup Creation");
    deroule.add_section(sect_options, "Options");
    deroule.set_active_section(0);

    fs_root.set_select_mode(html_form_input_file::select_dir);
    fs_root.set_can_create_dir(false);
    fs_root.set_change_event_name(fs_root_change_event);
    sauv_path.set_select_mode(html_form_input_file::select_dir);
    sauv_path.set_can_create_dir(true);
    if(repoxfer.get_html_user_interaction())
	repoxfer.get_html_user_interaction()->auto_hide(true, false);
    else
	throw WEBDAR_BUG;
    options->set_fs_root(fs_root.get_value()); // will be kept updat to date by mean on events

	// adoption tree

    fs.adopt(&fs_root);
    fs.adopt(&sauv_path);
    fs.adopt(&basename);
    form.adopt(&fs);
    deroule.adopt_in_section(sect_archive, &form);
    deroule.adopt_in_section(sect_options, &guichet_options);
    adopt(&deroule);
    adopt(&repoxfer);

	// events
    fs_root.record_actor_on_event(this, fs_root_change_event);
    options->record_actor_on_event(this, html_options_create::entrepot_changed);
    repoxfer.record_actor_on_event(this, html_libdar_running_popup::libdar_has_finished);

	// visibility
    repoxfer.set_visible(false);

    	// css stuff

    webdar_css_style::normal_button(deroule, true);
}

void html_archive_create::set_biblio(const std::shared_ptr<bibliotheque> & ptr)
{
    options->set_biblio(ptr);

    guichet_options.set_child(ptr,
			      bibliotheque::confsave,
			      options,
			      false);
}


void html_archive_create::on_event(const string & event_name)
{
    if(event_name == html_options_create::entrepot_changed)
    {
	need_entrepot_update = true;
    }
    else if(event_name == html_libdar_running_popup::libdar_has_finished)
    {
	repoxfer.set_visible(false);
    }
    else if(event_name == fs_root_change_event)
    {
	options->set_fs_root(fs_root.get_value());
    }
    else
	throw WEBDAR_BUG;
}

string html_archive_create::inherited_get_body_part(const chemin & path,
						    const request & req)
{
    if(need_entrepot_update)
	update_entrepot();
    return get_body_part_from_all_children(path, req);
}

void html_archive_create::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}

void html_archive_create::inherited_run()
{
    shared_ptr<html_web_user_interaction> ptr(repoxfer.get_html_user_interaction());

    if(!ptr)
	throw WEBDAR_BUG;
    sauv_path.set_entrepot(options->get_entrepot(ptr));
}

void html_archive_create::signaled_inherited_cancel()
{
    pthread_t libdar_tid;
    libdar::thread_cancellation th;

    if(is_running(libdar_tid))
	th.cancel(libdar_tid, true, 0);
}

void html_archive_create::update_entrepot()
{
    if(is_running())
	throw WEBDAR_BUG;
    join(); // in case a previous execution triggered an exception
    repoxfer.set_visible(true);
    need_entrepot_update = false;
    repoxfer.run_and_control_thread(this);
}
