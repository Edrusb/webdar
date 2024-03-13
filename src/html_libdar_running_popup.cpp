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
#include "html_page.hpp"

    //
#include "html_libdar_running_popup.hpp"

using namespace std;

const string html_libdar_running_popup::libdar_has_finished = "html_libdar_running_popup_finished";


html_libdar_running_popup::html_libdar_running_popup():
    html_popup(90,90),
    enable_refresh(false),
    changed_refresh(false)
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

string html_libdar_running_popup::inherited_get_body_part(const chemin & path,
						    const request & req)
{
    if(get_visible() != get_next_visible())
    {
	web_ui->set_visibile(get_next_visible());
	ack_visible();
	    // we propage the web_ui our own visibity status
    }

    if(get_visible())
    {
	if(changed_refresh)
	{
	    html_page* page = nullptr;

	    closest_ancestor_of_type(page);
	    changed_refresh = false;

	    if(page != nullptr)
	    {
		if(enable_refresh)
		    page->set_refresh_redirection(1, req.get_uri().get_path().display(false));
		else
		    page->set_refresh_redirection(0, ""); // disable refresh
	    }
	}

	return get_body_part_from_all_children(path, req);
    }
    else
	return "";
}

void html_libdar_running_popup::on_event(const std::string & event_name)
{
    if(event_name == html_web_user_interaction::libdar_has_finished)
    {
	act(libdar_has_finished); // propagating the event
	set_visible(false); // nothing more to show
    }
    else if(event_name == html_web_user_interaction::can_refresh)
    {
	enable_refresh = true;
	changed_refresh = true;
    }
    else if(event_name == html_web_user_interaction::dont_refresh)
    {
	enable_refresh = false;
	changed_refresh = true;
    }
    else
	throw WEBDAR_BUG;
}
