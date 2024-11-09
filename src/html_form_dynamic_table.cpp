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
#include "webdar_tools.hpp"


    //
#include "html_form_dynamic_table.hpp"

using namespace std;

const string html_form_dynamic_table::changed = "html_form_dynamic_table_changed";


html_form_dynamic_table::html_form_dynamic_table(bool has_left_labels,
						 bool selector_below,
						 const string & adder_label,
						 const string & adder_default_choice):
    my_provider(nullptr),
    left_label(has_left_labels),
    table(has_left_labels ? 3 : 2),
    adder(adder_label, new_line_to_add)
{

	// components configuration/
    adder.add_choice("", adder_default_choice);
    adder.set_selected(0);

	// adoption tree
    if(!selector_below)
	adopt(&adder);
    adopt(&table);
    if(selector_below)
	adopt(&adder);

	// events
    register_name(changed);
    adder.record_actor_on_event(this, new_line_to_add);

	// visibility

	// css stuff
}

void html_form_dynamic_table::set_obj_type_provider(const html_form_dynamic_table_object_provider* provider)
{
    if(provider == nullptr)
	throw WEBDAR_BUG;
    my_provider = provider;
}

void html_form_dynamic_table::add_obj_type(const string & label)
{
    if(my_provider == nullptr)
	throw WEBDAR_BUG;

    adder.add_choice(label, label);
    adder.set_selected(0);
}

void html_form_dynamic_table::on_event(const std::string & event_name)
{
    if(event_name == new_line_to_add)
    {
	if(adder.get_selected_num() != 0)
	{
		// there is a shift by one between adder which first position (index 0)
		// tells the user to select a filter and list_of_mask_types that have the
		// first valid mask at position 0.
	    add_line(adder.get_selected_num() - 1);
	    adder.set_selected(0); // resetting 'adder' to undefined
	    act(changed);
	}
    }
    else // check whether this is an event from a delete button
	del_line(event_name); // may throw exception if event_name is not a del event
	// act(changed) will be done by purge_to_delete() outside the on_event()
	// cascada calls
}

string html_form_dynamic_table::inherited_get_body_part(const chemin & path,
							const request & req)
{
    string ret = get_body_part_from_all_children(path, req);
    purge_to_delete();
    return ret;
}


void html_form_dynamic_table::add_line(unsigned int typenum)
{
    line newline;

    if(left_label)
    {
	newline.left_label.reset(new (nothrow) html_text());
	if(! newline.left_label)
	    throw exception_memory();
    }
    else
	newline.left_label.reset();

    newline.object_type_index = typenum;
    if(my_provider == nullptr)
	throw WEBDAR_BUG;
    newline.dynobj = my_provider->provide_object_of_type(typenum, context);
    if(! newline.dynobj)
	throw WEBDAR_BUG;

    newline.del.reset(new (nothrow) html_form_input("delete",
						    html_form_input::check,
						    "",
						    "1"));
    if(! newline.del)
	throw exception_memory();

    string event_name = webdar_tools_convert_to_string(event_del_count++);
    newline.del->set_change_event_name(event_name);
    newline.del->record_actor_on_event(this, event_name);
    if(newline.left_label)
	table.adopt(&(*newline.left_label));
    table.adopt(&(*newline.dynobj));
    table.adopt(&(*newline.del));
    list<line>::iterator pos = table_content.insert(table_content.end(),
						    std::move(newline));
    del_event_to_content[event_name] = pos;
}

void html_form_dynamic_table::del_line(const string & event_name)
{
    map<string, list<line>::iterator>::iterator mit = del_event_to_content.find(event_name);

    if(mit == del_event_to_content.end())
	throw WEBDAR_BUG; // event_name absent from the map!

    list<line>::iterator it = mit->second;

    if(left_label && !it->left_label)
	throw WEBDAR_BUG;
    if(!it->dynobj)
	throw WEBDAR_BUG;
    if(!it->del)
	throw WEBDAR_BUG;

    if(it->left_label)
	table.foresake(&(*(it->left_label)));
    table.foresake(&(*(it->dynobj)));
    table.foresake(&(*(it->del)));
    events_to_delete.push_back(event_name);
    	// postponing the object deletion
	// as we may get from here from the it->del pointed to actor event
	// we would not be able to return from this call if deleted from here
}

void html_form_dynamic_table::purge_to_delete()
{
    deque<string>::iterator evit = events_to_delete.begin();
    bool action = ! events_to_delete.empty();

    while(evit != events_to_delete.end())
    {
	map<string, list<line>::iterator>::iterator mit = del_event_to_content.find(*evit);

	if(mit == del_event_to_content.end())
	    throw WEBDAR_BUG;  // event_name absent from the map!

	list<line>::iterator it = mit->second;

	(void)del_event_to_content.erase(mit);
	(void)table_content.erase(it);

	++evit;
    }

    events_to_delete.clear();
    if(action)
	act(changed);
}
