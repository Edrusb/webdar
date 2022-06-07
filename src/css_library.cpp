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

void css_library::add(const string & name, const css & value)
{
    map<string, css>::iterator it = content.find(name);

    if(it == content.end())
	content[name] = value;
    else
	throw exception_range(string("label ") + name + string(" already present in this css_library\n"));
}

bool css_library::class_exists(const std::string & name) const
{
    css stored_value;

    return get_value(name, stored_value);
}

bool css_library::get_value(const string & name, css & stored_value) const
{
    map<string, css>::const_iterator it = content.find(name);

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
    map<string, css>::const_iterator it = content.find(name);

    if(it == content.end())
	throw exception_range(string("label ") + name + string(" absent from this css_library, cannot delete it\n"));
    else
	content.erase(it);
}

string css_library::generate_internal_css() const
{
    string ret = "<style>\n";

    map<string, css>::const_iterator it = content.begin();

    while(it != content.end())
    {
	ret += string(".") + it->first + string(" {");
	ret += it->second.css_get_raw_string();
	ret += string(" }\n");

	++it;
    }

    ret += "</style>\n";

    return ret;
}
