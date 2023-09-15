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
#include "html_compression.hpp"

using namespace std;

const string html_compression::changed = "html_compression_changed";

html_compression::html_compression(const string &title): html_form_select(title)
{
    add_choice("none", "None");
    add_choice("gzip", "Gzip");
    add_choice("bzip2", "Bzip2");
    add_choice("lzo", "lzo");
    set_selected(0);
    register_name(changed);
    record_actor_on_event(this, html_form_select::changed);
}

libdar::compression html_compression::get_value() const
{
    switch(get_selected_num())
    {
    case 0:
	return libdar::compression::none;
    case 1:
	return libdar::compression::gzip;
    case 2:
	return libdar::compression::bzip2;
    case 3:
	return libdar::compression::lzo;
    default:
	throw WEBDAR_BUG;
    }
}

void html_compression::set_value(libdar::compression val)
{
    switch(val)
    {
    case libdar::compression::none:
	set_selected(0);
	break;
    case libdar::compression::gzip:
	set_selected(1);
	break;
    case libdar::compression::bzip2:
	set_selected(2);
	break;
    case libdar::compression::lzo:
	set_selected(3);
	break;
    default:
	throw WEBDAR_BUG;
    }
}
