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
#include "html_form_radio.hpp"

using namespace std;

const string html_form_radio::changed = "html_form_radio::changed";

html_form_radio::html_form_radio():
    selected(0),
    value_set(false)
{
    register_name(changed);
}

void html_form_radio::add_choice(const string & id, const string & label)
{
    record x;
    x.id = id;
    x.label = label;
    choices.push_back(x);
    my_body_part_has_changed();
}

void html_form_radio::set_selected(unsigned int x)
{
    if(x >= choices.size())
    {
	unsigned int next_val = choices.size() - 1;
	if(next_val != selected)
	{
	    selected = next_val;
	    act(changed);
	    my_body_part_has_changed();
	}
    }
    else
    {
	if(x != selected)
	{
	    selected = x;
	    act(changed);
	    my_body_part_has_changed();
	}
    }

    value_set = true;
}

string html_form_radio::inherited_get_body_part(const chemin & path,
						const request & req)
{
    string ret = "";
    string radio_id = get_path().namify();

	// for POST method only, extract user choice from the body of the request
	// and update this object's fields

    update_field_from_request(req);

	// for any request provide an updated HMTL content in response

    for(unsigned int i = 0; i < choices.size(); ++i)
    {
	ret += "<input " + get_css_classes() + " type=\"radio\" name=\"" + radio_id + "\" id=\"" + choices[i].id + "\" value=\"" + choices[i].id + "\" ";
	if(i == selected)
	    ret += "checked ";
	ret += "/>\n";
	ret += "<label " + get_css_classes() + " for=\"" + choices[i].id + "\">" + choices[i].label + "</label>";
	if(i+1 < choices.size() || !get_no_CR())
	    ret += "<br />\n";
	else
	    ret += "\n";
    }

	// now unlocking the update_field_from_request()
	// if it was locked due to a call to set_selected()

    unlock_update_field_from_request();

    return ret;
}

void html_form_radio::update_field_from_request(const request & req)
{
    if(req.get_method() == "POST" && ! value_set)
    {
	map<string, string> bd = req.get_body_form();
	map<string, string>::const_iterator it = bd.find(get_path().namify());
	if(it != bd.end())
	{
	    unsigned int u = 0;
	    while(u < choices.size() && choices[u].id != it->second)
		++u;
	    if(u < choices.size())
	    {
		bool has_changed = selected != u;

		selected = u;
		if(has_changed)
		{
		    act(changed);
		    my_body_part_has_changed();
		}
	    }
	}
    }
}

void html_form_radio::unlock_update_field_from_request()
{
    if(!has_my_body_part_changed())
	value_set = false;
}
