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
#include "html_page.hpp"

    //
#include "html_libdar_running_popup.hpp"

using namespace std;

const string html_libdar_running_popup::libdar_has_finished = "html_libdar_running_popup_finished";


html_libdar_running_popup::html_libdar_running_popup():
    html_popup(90,90),
    enable_refresh(false)
{
    web_ui.reset(new (nothrow) html_web_user_interaction());
    if(!web_ui)
	throw exception_memory();

    adopt(web_ui.get());

	// signals and events
    register_name(libdar_has_finished);

    web_ui->record_actor_on_event(this, html_web_user_interaction::display_started);
    web_ui->record_actor_on_event(this, html_web_user_interaction::can_refresh);
    web_ui->record_actor_on_event(this, html_web_user_interaction::dont_refresh);
    web_ui->record_actor_on_event(this, html_web_user_interaction::libdar_has_finished);
}

string html_libdar_running_popup::inherited_get_body_part(const chemin & path,
							  const request & req)
{
    string ret;
    html_page* page = nullptr;

    closest_ancestor_of_type(page);
    if(page == nullptr)
	throw WEBDAR_BUG;

	// we want our visibility status to follow and act on
	// our html_web_user_interaction component visibility
	// such a way that changing either ours or this component
	// which we provide direct access to, to stay identical at anytime

	// the first direction webui -> this is done by the events
	// html_web_user_interaction::display_started we act upon
	// and html_web_user_interaction::libdar_has_finished in regard
	// to the auto_hide property

	// second, our component's visibilty should change ours accordingly
	// if we get running inherited_get_body_part, we are visible, web_ui
	// should also be set accordingly. If not, web_ui our adopted children
	// is neither called and nor visibile.
    if(!web_ui->get_visible())
	set_visible(web_ui->get_visible());
	// our component visibility has changed
	// we have to propagate this to ourself for
	// it be visible as expected

    ret = html_popup::inherited_get_body_part(path, req);

    if(enable_refresh)
	page->set_refresh_redirection(1, req.get_uri().url_path_part());
    else
	page->set_refresh_redirection(0, ""); // disable refresh

    return ret;
}

void html_libdar_running_popup::my_visibility_has_changed()
{
    if(!web_ui)
	throw WEBDAR_BUG;
    web_ui->set_visible(get_visible());
}

void html_libdar_running_popup::on_event(const string & event_name)
{
    if(event_name == html_web_user_interaction::display_started)
    {
	set_visible(true);
	my_body_part_has_changed(); // in case we were already visible
    }
    else if(event_name == html_web_user_interaction::libdar_has_finished)
    {
	my_body_part_has_changed();
	set_visible(false); // nothing more to show
	act(libdar_has_finished); // propagating the event
    }
    else if(event_name == html_web_user_interaction::can_refresh)
    {
	enable_refresh = true;
    }
    else if(event_name == html_web_user_interaction::dont_refresh)
    {
	enable_refresh = false;
    }
    else
	throw WEBDAR_BUG;
}
