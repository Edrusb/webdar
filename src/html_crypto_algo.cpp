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
#include "html_crypto_algo.hpp"

using namespace std;

const string html_crypto_algo::changed = "html_crypto_algo_changed";

html_crypto_algo::html_crypto_algo(const string & title): html_form_select(title)
{
    add_choice("none", "None");
    add_choice("scram", "Scrambling (weak)");
    add_choice("blowfish", "Blowfish");
    add_choice("aes", "AES");
    add_choice("twofish", "Twofish");
    add_choice("serpent", "Serpent");
    add_choice("camellia", "Camellia");
    set_selected(0);
    register_name(changed);
    record_actor_on_event(this, html_form_select::changed);
}

libdar::crypto_algo html_crypto_algo::get_value() const
{
    switch(get_selected_num())
    {
    case 0:
	return libdar::crypto_algo::none;
    case 1:
	return libdar::crypto_algo::scrambling;
    case 2:
	return libdar::crypto_algo::blowfish;
    case 3:
	return libdar::crypto_algo::aes256;
    case 4:
	return libdar::crypto_algo::twofish256;
    case 5:
	return libdar::crypto_algo::serpent256;
    case 6:
	return libdar::crypto_algo::camellia256;
    default:
	throw WEBDAR_BUG;
    }
}

void html_crypto_algo::set_value(libdar::crypto_algo val)
{
    switch(val)
    {
    case libdar::crypto_algo::none:
	set_selected(0);
	break;
    case libdar::crypto_algo::scrambling:
	set_selected(1);
	break;
    case libdar::crypto_algo::blowfish:
	set_selected(2);
	break;
    case libdar::crypto_algo::aes256:
	set_selected(3);
	break;
    case libdar::crypto_algo::twofish256:
	set_selected(4);
	break;
    case libdar::crypto_algo::serpent256:
	set_selected(5);
	break;
    case libdar::crypto_algo::camellia256:
	set_selected(6);
	break;
    default:
	throw WEBDAR_BUG;
    }
    my_body_part_has_changed();
}

void html_crypto_algo::on_event(const string & event_name)
{
	// when html_form_select changes, we propagate the change event to our own event name
    act(changed);
}
