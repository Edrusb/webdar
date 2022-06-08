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


    //
#include "css_class.hpp"

using namespace std;

css_class::css_class(const std::string & name):
    class_name(name)
{
    if(name.empty())
	throw WEBDAR_BUG;
}

css_class::css_class(const std::string & name, const css & ref)
{
    if(name.empty())
	throw WEBDAR_BUG;

    set_value(ref);
}

string css_class::get_definition() const
{
    string ret = convert_name(class_name);

    if(class_value.empty())
	throw WEBDAR_BUG;

    ret += " { " + class_value + " }\n";

    return ret;
}



