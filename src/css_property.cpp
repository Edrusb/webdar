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



    //
#include "css_property.hpp"

using namespace std;

void css_property::clear()
{
    value = "";
    inheritable = false;
    unset = true;
}

void css_property::set_value(const string & val)
{
    value = val;
    unset = false;
}


void css_property::inherit_from(const css_property & ref,
				bool any_inheritance,
				bool even_if_set)
{
    if((even_if_set || is_unset())
       && (any_inheritance || ref.get_inheritance())
       && !ref.is_unset())
    {
	set_value(ref.get_value());
	set_inheritance(ref.get_inheritance());
    }
}
