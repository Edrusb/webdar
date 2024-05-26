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
#include "webdar_tools.hpp"
#include "exceptions.hpp"


    //
#include "html_form_input.hpp"

using namespace std;

const string html_form_input::changed = "html_form_input_changed";

html_form_input::html_form_input(const string & label,
				 input_type type,
				 const string & initial_value,
				 unsigned int size):
    enabled(true),
    x_label(label),
    x_type(string_for_type(type)),
    x_init(initial_value),
    x_size(webdar_tools_convert_to_string(size)),
    x_min(""),
    x_max(""),
    value_set(false),
    modif_change("")
{
    register_name(changed);
}

void html_form_input::set_range(int min, int max)
{
    string next_min = webdar_tools_convert_to_string(min);
    string next_max = webdar_tools_convert_to_string(max);

    if(next_min != x_min
       || next_max != x_max)
    {
	my_body_part_has_changed();
	x_min = next_min;
	x_max = next_max;
	value_set = true;
    }
}

void html_form_input::change_label(const string & label)
{
    if(label != x_label)
    {
	x_label = label;
	my_body_part_has_changed();
	value_set = true;
    }
}

void html_form_input::change_type(input_type type)
{
    string next_type = string_for_type(type);

    if(next_type != x_type)
    {
	x_type = next_type;
	my_body_part_has_changed();
	value_set = true;
    }
}

void html_form_input::set_value(const string & val)
{
    if(x_init != val)
    {
	x_init = val;
	act(changed);
	my_body_part_has_changed();
	value_set = true;
    }
}

void html_form_input::set_value_as_bool(bool val)
{
    if(val != get_value_as_bool())
    {
	x_init = val ? "x" : "";
	act(changed);
	my_body_part_has_changed();
	value_set = true;
    }
}

void html_form_input::set_enabled(bool val)
{
    if(enabled != val)
    {
	enabled = val;
	act(changed);
	my_body_part_has_changed();
	value_set = true;
    }
}


string html_form_input::inherited_get_body_part(const chemin & path,
						const request & req)
{
    string ret = "";
    string x_id = get_path().namify();

	// first we extract informations from the returned form in
	// the body of the request

    if(req.get_method() == "POST"
       && path.empty()
       && get_visible()
       && enabled
       && !value_set)
    {
	string old = x_init;

	map<string, string> fields = req.get_body_form();
	map<string, string>::iterator it = fields.find(x_id);
	if(it != fields.end())
	    x_init = it->second;
	else
	{
		// checkboxes have specific behavior: unckecked, the browser does not
		// send any value and even not an empty value associated to this id.
	    if(x_type == string_for_type(check))
		x_init = "";
	}


	if(x_init != old)
	{
	    act(changed);
	    if(modif_change != "")
		act(modif_change);
		// yes we have to trigger both events,
		// the default, on which some objects might
		// have registered and the modifed one too.
	    my_body_part_has_changed();
	}
    }

	// we can now return the up to date value of
	// the field

    if(x_type != "checkbox")
	ret += "<label for=\"" + x_id + "\">" + x_label + "</label>\n";
    ret += "<input " + get_css_classes() + " type=\"" + x_type + "\" name=\"" + x_id + "\" id=\"" + x_id + "\" ";
    if(x_min != "" && x_max != "") // yes both
	ret += "min=\"" + x_min + "\" max=\"" + x_max + "\" ";
    if(x_init != "")
    {
	if(x_type == "checkbox")
	    ret += "checked ";
	else
	    ret += "value=\"" + webdar_tools_html_display(x_init) +"\" ";
    }
    if(x_size != "")
	ret += "size=\"" + x_size + "\" ";
    if(!enabled)
	ret += "disabled ";
    ret += "/>";
    if(x_type == "checkbox")
	ret += "<label for=\"" + x_id + "\">" + x_label + "</label>\n";
    if(!get_no_CR())
	ret += "<br />\n";

    if(!has_my_body_part_changed())
	value_set = false;

    return ret;
}

void html_form_input::set_change_event_name(const string & name)
{
    if(modif_change != "")
	throw WEBDAR_BUG; // can only be used once, as unregistering an event name is not available
    modif_change = name;
    register_name(modif_change); // this is in addition to the default event name
}

string html_form_input::string_for_type(input_type type)
{
    string ret;

    switch(type)
    {
    case text:
	ret = "text";
	break;
    case password:
	ret = "password";
	break;
    case number:
	ret = "number";
	break;
    case range:
	ret = "range";
	break;
    case check:
	ret = "checkbox";
	break;
    case file:
	ret = "file";
	break;
    default:
	throw WEBDAR_BUG;
    }

    return ret;
}

