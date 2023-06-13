/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2022 Denis Corbin
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
#include "css.hpp"

    //
#include "css_library.hpp"

using namespace std;

void css_library::add(const css_class & value)
{
    map<string, string>::iterator it = content.find(value.get_name());

    if(it == content.end())
	content[value.get_name()] = value.get_definition();
    else
	throw exception_range(string("label ") + value.get_name() + string(" already present in this css_library\n"));
}

void css_library::add(const std::string & name, const css & value)
{
    css_class tmp(name);
    tmp.set_value(value);
    add(tmp);
}

bool css_library::class_exists(const std::string & name) const
{
    string stored_value;

    return get_value(name, stored_value);
}

bool css_library::get_value(const string & name, string & stored_value) const
{
    map<string, string>::const_iterator it = content.find(name);

    if(it == content.end())
	return false;
    else
    {
	stored_value = it->second;
	return true;
    }
}

void css_library::del(const string & name)
{
    map<string, string>::const_iterator it = content.find(name);

    if(it == content.end())
	throw exception_range(string("label ") + name + string(" absent from this css_library, cannot delete it\n"));
    else
	content.erase(it);
}

string css_library::get_html_class_definitions() const
{
    string ret = "";

    map<string, string>::const_iterator it = content.begin();

    while(it != content.end())
    {
	ret += it->second;
	++it;
    }

    return ret;
}
