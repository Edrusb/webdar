/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013 Denis Corbin
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
#include "html_comparison_fields.hpp"

using namespace std;


html_comparison_fields::html_comparison_fields() : html_form_select("Comparison field")
{
    add_choice("all", "All fields");
    add_choice("ignore_owner", "Ignore ownership");
    add_choice("mtime", "Ignore ownership and permission");
    add_choice("inode_type", "ignore all but inode type");
    set_selected(0);
}

libdar::comparison_fields html_comparison_fields::get_value() const
{
    switch(get_selected_num())
    {
    case 0:
	return libdar::comparison_fields::all;
    case 1:
	return libdar::comparison_fields::ignore_owner;
    case 2:
	return libdar::comparison_fields::mtime;
    case 3:
	return libdar::comparison_fields::inode_type;
    default:
	throw WEBDAR_BUG;
    }
}

void html_comparison_fields::set_value(libdar::comparison_fields val)
{
    switch(val)
    {
    case libdar::comparison_fields::all:
	set_selected(0);
	break;
    case libdar::comparison_fields::ignore_owner:
	set_selected(1);
	break;
    case libdar::comparison_fields::mtime:
	set_selected(2);
	break;
    case libdar::comparison_fields::inode_type:
	set_selected(3);
	break;
    default:
	throw WEBDAR_BUG;
    }
}
