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
#include "html_form_overwrite_chain_cell.hpp"
#include "html_form_overwrite_action.hpp"
#include "tooltip_messages.hpp"

    //
#include "html_form_overwrite_chain_action.hpp"

using namespace std;

const string html_form_overwrite_chain_action::changed = "hfo_chain_action_changed";

html_form_overwrite_chain_action::html_form_overwrite_chain_action(const string & label):
    fs(label),
    table(false, true, "New action", "--- select ---")
{

	// components setup
    table.set_obj_type_provider(this);
    table.add_obj_type("Add a new cell");

	// adoption tree
    fs.adopt(&table);
    adopt(&fs);

	// events
    table.record_actor_on_event(this, html_form_dynamic_table::changed);
    register_name(changed);

	// css

	// tooltips
    if(!label.empty())
	fs.set_tooltip(TOOLTIP_HFOCA_FS);
}

html_form_overwrite_action & html_form_overwrite_chain_action::get_last_added()
{
    html_form_dynamic_table::iterator tmp;
    html_form_overwrite_chain_cell* ptr = nullptr;

    if(table.empty())
	throw WEBDAR_BUG;
    tmp = table.last();

    if(!table.last().get_object())
	throw WEBDAR_BUG;
    if(table.last().get_object().get() == nullptr)
	throw WEBDAR_BUG;

    ptr = dynamic_cast<html_form_overwrite_chain_cell*>(table.last().get_object().get());
    if(ptr == nullptr)
	throw WEBDAR_BUG;

    return ptr->get_cell_action();
}

unique_ptr<libdar::crit_action> html_form_overwrite_chain_action::get_overwriting_action() const
{
    unique_ptr<libdar::crit_chain> ret;
    html_form_dynamic_table::iterator it = table.begin();
    shared_ptr<body_builder> obj;
    html_overwrite_action *obj_ptr;
    unique_ptr<libdar::crit_action> tmp;

    ret.reset(new (nothrow) libdar::crit_chain());
    if(!ret)
	throw exception_memory();

    while(it != table.end())
    {
	obj = it.get_object();
	if(!obj)
	    throw WEBDAR_BUG;
	obj_ptr = dynamic_cast<html_overwrite_action*>(obj.get());
	if(obj_ptr == nullptr)
	    throw WEBDAR_BUG;
	tmp = obj_ptr->get_overwriting_action();
	if(!tmp)
	    throw WEBDAR_BUG;

	ret->add(*tmp);

	++it;
    }

    return ret;
}

unique_ptr<body_builder> html_form_overwrite_chain_action::provide_object_of_type(unsigned int num,
										  const string & context,
										  string & changed_event) const
{
    unique_ptr<body_builder> ret;
    unique_ptr<html_form_overwrite_action> obj;

    switch(num)
    {
    case 0:
	obj.reset(new (nothrow) html_form_overwrite_action(""));
	if(!obj)
	    throw exception_memory();
	ret.reset(new (nothrow) html_form_overwrite_chain_cell(obj));
	if(!ret)
	    throw exception_memory();
	changed_event = html_form_overwrite_chain_cell::changed;
	break;
    default:
	throw WEBDAR_BUG;
    }

    return ret;
}


void html_form_overwrite_chain_action::on_event(const string & event_name)
{
    if(event_name == html_form_dynamic_table::changed)
	act(changed);
    else
	throw WEBDAR_BUG;
}
