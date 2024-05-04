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

    //
#include "html_libdar_running_page.hpp"

using namespace std;

const string html_libdar_running_page::libdar_has_finished = "html_libdar_running_page_finished";


html_libdar_running_page::html_libdar_running_page():
    html_page("THIS IS A BUG"),
    sessname(""),
    enable_refresh(true)
{
    web_ui.reset(new (nothrow) html_web_user_interaction());
    if(!web_ui)
	throw exception_memory();

    adopt(web_ui.get());

	// signals and events
    register_name(libdar_has_finished);

    web_ui->record_actor_on_event(this, html_web_user_interaction::libdar_has_finished);
    web_ui->record_actor_on_event(this, html_web_user_interaction::can_refresh);
    web_ui->record_actor_on_event(this, html_web_user_interaction::dont_refresh);
}

string html_libdar_running_page::inherited_get_body_part(const chemin & path,
						    const request & req)
{
    string body = get_body_part_from_children_as_a_block(path, req);

    if(enable_refresh)
	set_refresh_redirection(1, req.get_uri().get_path().display(false));
    else
	set_refresh_redirection(0, ""); // disable refresh

    return get_body_part_given_the_body(path, req, body);
}

void html_libdar_running_page::on_event(const string & event_name)
{
    if(event_name == html_web_user_interaction::libdar_has_finished)
    {
	set_title(webdar_tools_get_title(sessname, "Libdar thread has ended"));
	my_body_part_has_changed();
	act(libdar_has_finished);
    }
    else if(event_name == html_web_user_interaction::can_refresh)
    {
	if(!enable_refresh)
	{
	    my_body_part_has_changed();
	    enable_refresh = true;
	}
    }
    else if(event_name == html_web_user_interaction::dont_refresh)
    {
	if(enable_refresh)
	{
	    my_body_part_has_changed();
	    enable_refresh = false;
	}
    }
    else
	throw WEBDAR_BUG;
}
