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



    //
#include "html_hash_algo.hpp"

using namespace std;

const string html_hash_algo::changed = "html_hash_algo_changed";

html_hash_algo::html_hash_algo(const string & title):
    html_form_select(title)
{
    add_choice("none", "none");
    add_choice("md5", "md5");
    add_choice("sha1", "sha1");
    add_choice("sha512", "sha512");
    add_choice("whirlpool", "whirlpool");
    set_selected_num(0);
    register_name(changed);
    record_actor_on_event(this, html_form_select::changed);
}

libdar::hash_algo html_hash_algo::get_value() const
{
    switch(get_selected_num())
    {
    case 0:
	return libdar::hash_algo::none;
    case 1:
	return libdar::hash_algo::md5;
    case 2:
	return libdar::hash_algo::sha1;
    case 3:
	return libdar::hash_algo::sha512;
    case 4:
	return libdar::hash_algo::whirlpool;
    default:
	throw WEBDAR_BUG;
    }
}

void html_hash_algo::set_value(libdar::hash_algo val)
{
    switch(val)
    {
    case libdar::hash_algo::none:
	set_selected_num(0);
	break;
    case libdar::hash_algo::md5:
	set_selected_num(1);
	break;
    case libdar::hash_algo::sha1:
	set_selected_num(2);
	break;
    case libdar::hash_algo::sha512:
	set_selected_num(3);
	break;
    case libdar::hash_algo::whirlpool:
	set_selected_num(4);
	break;
    default:
	throw WEBDAR_BUG;
    }
    my_body_part_has_changed();
}
