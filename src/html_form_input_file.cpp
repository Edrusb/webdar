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
#include "webdar_css_style.hpp"

    //
#include "html_form_input_file.hpp"

using namespace std;

const string html_form_input_file::changed_event = "html_form_input_file_changed";
const string html_form_input_file::triggered_event = "triggered";

html_form_input_file::html_form_input_file(const string & label,
					   const string & initial_value,
					   unsigned int size,
					   const string & popup_message):
    input(label, html_form_input::text, initial_value, size),
    trigger("+", triggered_event),
    user_select(popup_message),
    changed_event_name(changed_event),
    entrep(nullptr),
    refresh_get_body(false)
{
	// html adoption tree

    adopt(&input);
    adopt(&trigger);
    adopt(&user_select);

	// cabling events

    register_name(changed_event_name);

    input.record_actor_on_event(this, html_form_input::changed);
    trigger.record_actor_on_event(this, triggered_event);
    user_select.record_actor_on_event(this, html_select_file::entry_selected);

	// css classes

    webdar_css_style::normal_button(trigger);

    entrep.reset(new (nothrow) libdar::entrepot_local("", "", false));
    if(!entrep)
	throw exception_memory();
}


void html_form_input_file::set_change_event_name(const string & name)
{
    register_name(name);
    changed_event_name = name;
}

void html_form_input_file::set_entrepot(const shared_ptr<libdar::entrepot> & entrepot)
{
    if(!entrepot)
	throw WEBDAR_BUG;
    entrep = entrepot;
}

void html_form_input_file::on_event(const string & event_name)
{
    if(event_name == html_form_input::changed)
	act(changed_event_name);
    else if(event_name == triggered_event)
	user_select.run(entrep, input.get_value());
    else if(event_name == html_select_file::entry_selected)
    {
	input.set_value(user_select.get_selected_path());
	refresh_get_body = true;
    }
    else
	throw WEBDAR_BUG;
}

void html_form_input_file::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();

    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}

string html_form_input_file::inherited_get_body_part(const chemin & path,
						     const request & req)
{
    string ret;
    refresh_get_body = false;

    ret = html_div::inherited_get_body_part(path, req);
    if(refresh_get_body)
	ret = html_div::inherited_get_body_part(path, req);

    return ret;
}
