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



    //
#include "html_size_unit.hpp"

using namespace std;

html_size_unit::html_size_unit() : unit(""),
				   SI_mode("")
{
    SI_mode.add_choice("SI", "SI");
    SI_mode.add_choice("bin", "binary");
    SI_mode.set_selected(0);
    SI_mode.record_actor_on_event(this, html_form_select::changed);
    unit.set_no_CR();
    set_fields();
    adopt(&unit);
    adopt(&SI_mode);
}

libdar::infinint html_size_unit::get_value() const
{
    libdar::infinint ret;
    libdar::infinint base;

    switch(SI_mode.get_selected_num())
    {
    case 0:
	base = 1000;
	break;
    case 1:
	base = 1024;
	break;
    default:
	throw WEBDAR_BUG;
    }

    ret = base.power(unit.get_selected_num());

    return ret;
}

void html_size_unit::on_event(const string & event_name)
{
    set_fields();
	// no need to call my_body_part_has_changed()
	// because changed done in on_event concern
	// body_builder objects we have adopted
}

string html_size_unit::inherited_get_body_part(const chemin & path,
					       const request & req)
{
    return get_body_part_from_all_children(path, req);
}


void html_size_unit::set_fields()
{
    switch(SI_mode.get_selected_num())
    {
    case 0:
	unit.clear();
	unit.add_choice("octet", "o");
	unit.add_choice("kilo", "ko");
	unit.add_choice("mega", "Mo");
	unit.add_choice("giga", "Go");
	unit.add_choice("tera", "To");
	unit.add_choice("peta", "Po");
	unit.add_choice("exa", "Eo");
	unit.add_choice("zetta", "Zo");
	unit.add_choice("yotta", "Yo");
	break;
    case 1:
	unit.clear();
	unit.add_choice("octet", "o");
	unit.add_choice("kilo", "kio");
	unit.add_choice("mega", "Mio");
	unit.add_choice("giga", "Gio");
	unit.add_choice("tera", "Tio");
	unit.add_choice("peta", "Pio");
	unit.add_choice("exa", "Eio");
	unit.add_choice("zetta", "Zio");
	unit.add_choice("yotta", "Yio");
	break;
    default:
	throw WEBDAR_BUG;
    }
}
