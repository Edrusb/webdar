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



    //
#include "html_size_unit.hpp"

using namespace std;

const string html_size_unit::changed = "html_size_unit_changed";

html_size_unit::html_size_unit() : unit(""),
				   SI_mode(""),
				   manual_change(false)
{
	// set components value
    SI_mode.add_choice("SI", "SI");
    SI_mode.add_choice("bin", "binary");
    SI_mode.set_selected(1); // defaulting to binary family units
    unit.set_no_CR();
    set_fields();

	// adoption tree
    adopt(&unit);
    adopt(&SI_mode);

	// events
    register_name(changed);
    SI_mode.set_change_event_name(mode_changed);
    SI_mode.record_actor_on_event(this, mode_changed);
    unit.record_actor_on_event(this, html_form_select::changed);
}

libdar::infinint html_size_unit::get_value() const
{
    libdar::infinint ret;
    libdar::infinint base = get_base_unit_value(SI_mode.get_selected_num());

    try
    {
	ret = base.power(unit.get_selected_num());
    }
    catch(libdar::Elimitint & e)
    {
	throw exception_libcall(e);
    }

    return ret;
}

libdar::infinint html_size_unit::get_base_unit_value(unsigned int index) const
{
    libdar::infinint base;

    switch(index)
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

    return base;
}

unsigned int html_size_unit::get_max_power_for_base_unit(unsigned int index) const
{
    return unit.num_choices() - 1;
	// for now the implementation
	// uses the same number of factors
	// for both SI and binary based units
	// if this change in the future
	// we will keep trace of the units
	// in a ~table~ field and read that table in
	// set_fields() method for we have the
	// exact number of option per base unit
	// at any time
}

void html_size_unit::set_unit_and_ratio_indexes(unsigned int base_index,
						unsigned int power_index)
{
    bool has_changed = false;

    manual_change = true;

    try
    {
	if(base_index <= get_max_base_unit_index())
	{
	    if(base_index != SI_mode.get_selected_num())
	    {
		SI_mode.set_selected(base_index);
		set_fields();
		has_changed = true;
	    }
		// else nothing changes here, thus nothing to do
	}
	else
	    throw WEBDAR_BUG;

	if(power_index <= get_max_power_for_base_unit(base_index))
	{
	if(power_index != unit.get_selected_num())
	{
	    unit.set_selected(power_index);
	    has_changed = true;
	}
	    // else nothing changes here, thus nothing to do
	}
	else
	    throw WEBDAR_BUG;
    }
    catch(...)
    {
	manual_change = false;
	throw;
    }
    manual_change = false;

    if(has_changed)
	act(changed);
}

void html_size_unit::on_event(const string & event_name)
{
    if(manual_change)
	return;

    if(event_name == mode_changed)
    {
	set_fields();
	    // no need to call my_body_part_has_changed()
	    // because changes done in on_event concern
	    // body_builder objects we have adopted
	act(changed);
    }
    else if(event_name == html_form_select::changed)
	act(changed);
    else
	throw WEBDAR_BUG;
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
	unit.add_choice("o", "o");
	unit.add_choice("ko", "ko");
	unit.add_choice("Mo", "Mo");
	unit.add_choice("Go", "Go");
	unit.add_choice("To", "To");
	unit.add_choice("Po", "Po");
	unit.add_choice("Eo", "Eo");
	unit.add_choice("Zo", "Zo");
	unit.add_choice("Yo", "Yo");
	break;
    case 1:
	unit.clear();
	unit.add_choice("o", "o");
	unit.add_choice("kio", "kio");
	unit.add_choice("Mio", "Mio");
	unit.add_choice("Gio", "Gio");
	unit.add_choice("Tio", "Tio");
	unit.add_choice("Pio", "Pio");
	unit.add_choice("Eio", "Eio");
	unit.add_choice("Zio", "Zio");
	unit.add_choice("Yio", "Yio");
	break;
    default:
	throw WEBDAR_BUG;
    }
}
