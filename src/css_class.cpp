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
#include "exceptions.hpp"


    //
#include "css_class.hpp"

using namespace std;

css_class::css_class(const string & name):
    class_name(name)
{
    if(name.empty())
	throw WEBDAR_BUG;
}

css_class::css_class(const string & name, const css & ref)
{
    if(name.empty())
	throw WEBDAR_BUG;

    class_name = name;
    set_value(ref);
}


void css_class::set_selector(selector_type sel, const css & ref)
{
    map<selector_type, string>::iterator it = selectors.find(sel);

    if(it == selectors.end())
	selectors[sel] = ref.css_get_raw_string();
    else
	throw WEBDAR_BUG;
}

void css_class::clear_selector(selector_type sel)
{
    map<selector_type, string>::iterator it = selectors.find(sel);

    if(it != selectors.end())
	selectors.erase(sel);
	// else we ignore, the selector is not present
}


string css_class::get_definition() const
{
    string ret;
    map<selector_type, string>::const_iterator it = selectors.begin();

    if(class_value.empty() && selectors.empty())
	return "";

    if(!class_value.empty())
	ret += "*." + class_name + " { " + class_value + " }\n";

    while(it != selectors.end())
    {
	if(it->second.empty())
	    throw WEBDAR_BUG;

	ret += "*." + class_name + ":" + get_selector_name(it->first) + " { " + it->second + " }\n";
	++it;
    }

    return ret;
}


string css_class::get_selector_name(selector_type sel)
{
    switch(sel)
    {
    case active:
	return "active";
    case checked:
	return "checked";
    case disabled:
	return "disabled";
    case enabled:
	return "enabled";
    case focus:
	return "focus";
    case hover:
	return "hover";
    case link:
	return "link";
    case in_range:
	return "in-range";
    case invalid:
	return "invalid";
    case out_of_range:
	return "out-of-range";
    case read_only:
	return "read-only";
    case read_write:
	return "read-write";
    case root:
	return "root";
    case target:
	return "target";
    case valid:
	return "valid";
    case visited:
	return "visited";
    default:
	throw WEBDAR_BUG;
    }
}
