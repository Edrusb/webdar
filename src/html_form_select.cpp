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
#include "webdar_tools.hpp"


    //
#include "html_form_select.hpp"

using namespace std;

const string html_form_select::changed = "html_form_select_changed";

html_form_select::html_form_select(const string & label, const string & x_event_name):
    enabled(true),
    x_label(label)
{
    if(x_event_name.empty())
	event_name = changed;
    else
	event_name = x_event_name;
    register_name(event_name);
    record_actor_on_event(this, html_form_radio::changed);
}

html_form_select::html_form_select(const html_form_select & ref):
    html_form_radio(ref),
    actor(ref),
    x_label(ref.x_label),
    event_name(ref.event_name)
{
	// no need to register_name(event_name), this has been copied (without
	// the referred actors to these events) by the copy constructor of class events
    record_actor_on_event(this, html_form_radio::changed);
}

void html_form_select::set_enabled(bool val)
{
    if(enabled != val)
    {
	enabled = val;
	act(event_name);
	my_body_part_has_changed();
    }
}

string html_form_select::inherited_get_body_part(const chemin & path, const request & req)
{
    string ret = "";
    string select_id = get_path().namify();
    const vector<record> & choices = get_choices();
    vector<record>::const_iterator it = choices.begin();

	// for POST method only, extract used choice from the body of the request
	// and update this object's fields;
    update_field_from_request(req);

	// for any request provide an updated HTML content in response

    if(! x_label.get_label().empty())
    {
	x_label.set_for_field(webdar_tools_html_display(select_id));
	ret += x_label.get_body_part();
    }

    ret += "<select name=\"" + webdar_tools_html_display(select_id) + "\" id=\"" + webdar_tools_html_display(select_id) + "\"";
    if(!enabled)
	ret += " disabled";
    ret += + ">\n";

    while(it != get_choices().end())
    {
	ret += "<option value=\"" + webdar_tools_html_display(it->id) + "\"";
	if(it->id == get_selected_id())
	    ret += " selected";
	ret += ">" + it->label.get_label() + "</option>\n";
	++it;
    }
    ret += "</select>\n";

    if(!get_no_CR())
	ret += "<br />\n";

    unlock_update_field_from_request();

    return ret;
};
