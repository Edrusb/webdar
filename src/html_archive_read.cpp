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
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "chemin.hpp"


    //
#include "html_archive_read.hpp"

using namespace std;

html_archive_read::html_archive_read(const string & archive_description):
    form("Update"),
    fs(archive_description),
    arch_path("Archive path",
	      "/",
	      50,
	      "Select the backup to read...")
{

	// web components layout

    fs.adopt(&arch_path);
    form.adopt(&fs);
    adopt(&form);
    adopt(&opt_read);
    adopt(&libdarexec);

	// events and actor
    opt_read.record_actor_on_event(this, html_options_read::entrepot_has_changed);
    arch_path.record_actor_on_event(this, html_form_input_file::changed_event);
    libdarexec.record_actor_on_event(this, html_libdar_running_popup::libdar_has_finished);

	// initial values
    arch_path.set_select_mode(html_form_input_file::select_slice);
    libdarexec.set_visible(false);
}

void html_archive_read::on_event(const string & event_name)
{
    if(event_name == html_options_read::entrepot_has_changed)
    {
	shared_ptr<html_web_user_interaction> webui(libdarexec.get_html_user_interaction());
	if(!webui)
	    throw WEBDAR_BUG;
	webui->clear();
	webui->auto_hide(true, true);
	if(!is_running())
	{
	    libdarexec.set_visible(true);
	    my_body_part_has_changed();
	    webui->run_and_control_thread(this);
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
	    opt_read.set_src_min_digits(arch_path.get_min_digits());
    }
    else if(event_name == html_libdar_running_popup::libdar_has_finished)
    {
	libdarexec.set_visible(false); // now hiding the popup
	my_body_part_has_changed();
	join();
    }
    else
	throw WEBDAR_BUG;
}

string html_archive_read::inherited_get_body_part(const chemin & path,
						  const request & req)
{
    return get_body_part_from_all_children(path, req);
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
    arch_path.set_entrepot(opt_read.get_entrepot(libdarexec.get_html_user_interaction()));
}

void html_archive_read::signaled_inherited_cancel()
{
    pthread_t libdar_tid;
    libdar::thread_cancellation th;

    if(is_running(libdar_tid))
	th.cancel(libdar_tid, true, 0);
}
