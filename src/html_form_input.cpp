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
#include "webdar_tools.hpp"
#include "exceptions.hpp"


    //
#include "html_form_input.hpp"

using namespace std;

const string html_form_input::changed = "html_form_input_changed";

html_form_input::html_form_input(const string & label,
				 input_type type,
				 const string & initial_value,
				 const string & size):
    enabled(true),
    x_label(label),
    x_type(string_for_type(type)),
    x_init(initial_value),
    x_size(size),
    x_min(""),
    x_max(""),
    value_set(false),
    modif_change("")
{
    register_name(changed);
}

void html_form_input::set_range(const libdar::infinint & min,
				const libdar::infinint & max)
{
    string next_min = libdar::deci(min).human();
    string next_max = libdar::deci(max).human();

    check_min_max_change(next_min, next_max);
}

void html_form_input::set_min_only(const libdar::infinint & min)
{
    string next_min = libdar::deci(min).human();
    string next_max = "";

    check_min_max_change(next_min, next_max);
}

void html_form_input::set_max_only(const libdar::infinint & max)
{
    string next_min = "";
    string next_max = libdar::deci(max).human();

    check_min_max_change(next_min, next_max);
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
	my_act();
	my_body_part_has_changed();
	value_set = true;
    }
}

void html_form_input::set_value_as_bool(bool val)
{
    if(val != get_value_as_bool())
    {
	x_init = val ? "x" : "";
	my_act();
	my_body_part_has_changed();
	value_set = true;
    }
}

void html_form_input::set_value_as_int(int val)
{
    if(x_type == string_for_type(number))
	set_value(webdar_tools_convert_to_string(val));
    else
	throw WEBDAR_BUG;
}

int html_form_input::get_value_as_int() const
{
    if(x_type == string_for_type(number))
	return webdar_tools_convert_to_int(x_init);
    else
	throw WEBDAR_BUG;
}

void html_form_input::set_enabled(bool val)
{
    if(enabled != val)
    {
	enabled = val;
	my_act();
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
	    my_act();
	    my_body_part_has_changed();
	}
    }

	// we can now return the up to date value of
	// the field

    if(x_type != "checkbox")
	ret += "<label for=\"" + x_id + "\">" + x_label + "</label>\n";
    ret += "<input " + get_css_classes() + " type=\"" + x_type + "\" name=\"" + x_id + "\" id=\"" + x_id + "\" ";
    if(x_min != "")
	ret += "min=\"" + x_min + "\" ";
    if(x_max != "")
	ret += "max=\"" + x_max + "\" ";
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
    if(name == "")
	throw WEBDAR_BUG;
	// we expect non-empty string as valid event name

    if(modif_change.empty())
	modif_change = changed;

	// leveraging new featur from events class
	// all registered actors are kept:
    rename_name(modif_change, name);
    modif_change = name;
}

void html_form_input::check_min_max_change(const string & next_min, const string & next_max)
{
    if(next_min != x_min
       || next_max != x_max)
    {
	my_body_part_has_changed();
	x_min = next_min;
	x_max = next_max;
	value_set = true;
    }
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

