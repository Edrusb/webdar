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
#include "html_form_overwrite_chain_cell.hpp"

using namespace std;

const string html_form_overwrite_chain_cell::changed = "hfo_chain_cell_changed";

html_form_overwrite_chain_cell::html_form_overwrite_chain_cell(unique_ptr<html_form_overwrite_action> & insert):
    incell(std::move(insert))
{

	// components setup
    if(!incell)
	throw WEBDAR_BUG;

	// adoption tree
    adopt(&line);
    adopt(incell.get());

	// events
    incell->record_actor_on_event(this, html_form_overwrite_action::changed);
    register_name(changed);

	// css
}

html_form_overwrite_action & html_form_overwrite_chain_cell::get_cell_action()
{
    html_form_overwrite_action* ptr = nullptr;

    if(!incell)
	throw WEBDAR_BUG;
    ptr = incell.get();
    if(ptr == nullptr)
	throw WEBDAR_BUG;

    return *ptr;
}

unique_ptr<libdar::crit_action> html_form_overwrite_chain_cell::get_overwriting_action() const
{
    return incell->get_overwriting_action();
}


void html_form_overwrite_chain_cell::on_event(const string & event_name)
{
    if(event_name == html_form_overwrite_action::changed)
	act(changed);
    else
	throw WEBDAR_BUG;
}
