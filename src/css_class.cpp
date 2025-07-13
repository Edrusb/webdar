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
#include "exceptions.hpp"


    //
#include "css_class.hpp"

using namespace std;

css_class::css_class(const string & name):
    class_name(name)
{
}

css_class::css_class(const string & name, const css & ref)
{
    if(name.empty())
	throw WEBDAR_BUG;

    class_name = name;
    class_value = ref;
}


void css_class::set_selector(selector_type sel,
			     const css & ref,
			     const string & descendant)
{
    map<selector_type, couple>::iterator it = selectors.find(sel);

    if(it == selectors.end())
	selectors[sel] = couple(ref, descendant);
    else
	throw WEBDAR_BUG;
}

void css_class::clear_selector(selector_type sel)
{
    map<selector_type, couple>::iterator it = selectors.find(sel);

    if(it != selectors.end())
	selectors.erase(sel);
	// else we ignore, the selector is not present
}


void css_class::set_pseudo_element(pseudo_element_type pe,
				   const css & ref,
				   const std::string & descendant)
{
    map<pseudo_element_type, couple>::iterator it = pseudo_elements.find(pe);

    if(it == pseudo_elements.end())
	pseudo_elements[pe] = couple(ref, descendant);
    else
	throw WEBDAR_BUG;
}

void css_class::clear_pseudo_element(pseudo_element_type pe)
{
    map<pseudo_element_type, couple>::iterator it = pseudo_elements.find(pe);

    if(it != pseudo_elements.end())
	pseudo_elements.erase(pe);
	// else we ignore, the selector is not present
}

string css_class::get_definition() const
{
    string ret;
    map<selector_type, couple>::const_iterator it = selectors.begin();
    map<pseudo_element_type, couple>::const_iterator peit = pseudo_elements.begin();
    string raw_val = class_value.css_get_raw_string();
    string sub;

    if(class_name.empty())
	throw WEBDAR_BUG; // name is missing

    if(raw_val.empty() && selectors.empty() && pseudo_elements.empty())
	return "";

    if(! raw_val.empty())
	ret += "*." + class_name + " { " + raw_val + " }\n";

    while(it != selectors.end())
    {
	if(it->second.subcomp.empty())
	    sub = "";
	else
	    sub = " " + it->second.subcomp;

	raw_val = it->second.css_def.css_get_raw_string();
	if(raw_val.empty())
	    throw WEBDAR_BUG; // why adding a css if it has no properties at all!

	ret += "*." + class_name + ":" + get_selector_name(it->first) + sub + " { " + raw_val + " }\n";
	++it;
    }

    while(peit != pseudo_elements.end())
    {
	if(it->second.subcomp.empty())
	    sub = "";
	else
	    sub = " " + it->second.subcomp;

	raw_val = peit->second.css_def.css_get_raw_string();
	if(raw_val.empty())
	    throw WEBDAR_BUG; // same as above for selectors / pseudo classes

	ret += "*." + class_name + "::" + get_pseudo_element_name(peit->first) + sub + " { " + raw_val + " }\n";
	++peit;
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

string css_class::get_pseudo_element_name(pseudo_element_type pe)
{
    switch(pe)
    {
    case first_line:
	return "first-line";
    case first_letter:
	return "first-letter";
    case before:
	return "before";
    case after:
	return "after";
    case marker:
	return "marker";
    case selection:
	return "selection";
    default:
	throw WEBDAR_BUG;
    }
}
