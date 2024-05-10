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



    //
#include "html_form_select.hpp"

using namespace std;

const string html_form_select::changed = "html_form_select_changed";

html_form_select::html_form_select(const string & label, const string & x_event_name)
{
    x_label = label;
    if(x_event_name.empty())
	event_name = changed;
    else
	event_name = x_event_name;
    register_name(event_name);
    record_actor_on_event(this, html_form_radio::changed);
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

    ret += "<label for=\"" + select_id + "\">" + x_label + "</label>\n";
    ret += "<select name=\"" + select_id + "\" id=\"" + select_id + "\">\n";
    while(it != get_choices().end())
    {
	ret += "<option value=\"" + it->id + "\"";
	if(it->id == get_selected_id())
	    ret += " selected";
	ret += ">" + it->label + "</option>\n";
	++it;
    }
    ret += "</select>\n";

    if(!get_no_CR())
	ret += "<br />\n";

    unlock_update_field_from_request();

    return ret;
};
