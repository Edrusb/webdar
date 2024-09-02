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
#include "webdar_css_style.hpp"



    //
#include "html_form_overwrite_chain_action.hpp"

using namespace std;

html_form_overwrite_chain_action::html_form_overwrite_chain_action(const std::string & label):
    event_del_count(0),
    fs(label),
    table(2),
    adder("Add new action", event_add)
{

	// components setup

	// adoption tree
    fs.adopt(&table);
    fs.adopt(&adder);
    adopt(&fs);

	// events
    adder.record_actor_on_event(this, event_add);

	// css
    webdar_css_style::normal_button(adder, false);
}


unique_ptr<libdar::crit_action> html_form_overwrite_chain_action::get_overwriting_action() const
{
    unique_ptr<libdar::crit_chain> ret;
    list<entry>::const_iterator it = table_content.begin();
    unique_ptr<libdar::crit_action> tmp;

    ret.reset(new (nothrow) libdar::crit_chain());
    if(!ret)
	throw exception_memory();

    while(it != table_content.end())
    {
	if(!it->action)
	    throw WEBDAR_BUG;
	tmp = it->action->get_overwriting_action();
	if(!tmp)
	    throw WEBDAR_BUG;
	ret->add(*tmp);

	++it;
    }

    return ret;
}

void html_form_overwrite_chain_action::on_event(const std::string & event_name)
{
    if(event_name == event_add)
    {
	entry new_action;

	new_action.action.reset(new (nothrow) html_form_overwrite_action(""));
	if(!new_action.action)
	    throw exception_memory();

	new_action.del.reset(new (nothrow) html_form_input("delete",
							   html_form_input::check,
							   "",
							   1));
	if(!new_action.del)
	    throw exception_memory();

	string counter_event_name = webdar_tools_convert_to_string(event_del_count++);
	new_action.del->set_change_event_name(counter_event_name);
	new_action.del->record_actor_on_event(this, counter_event_name);

	table.adopt(&(*new_action.action));
	table.adopt(&(*new_action.del));
	list<entry>::iterator pos = table_content.insert(table_content.end(),
							 std::move(new_action));

	del_event_to_content[counter_event_name] = pos;
    }
    else
	del_action(event_name);
}

string html_form_overwrite_chain_action::inherited_get_body_part(const chemin & path,
							       const request & req)
{
    string ret = get_body_part_from_all_children(path, req);
    purge_to_delete();
    return ret;
}

void html_form_overwrite_chain_action::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}

void html_form_overwrite_chain_action::del_action(const std::string & event_name)
{
    map<string, list<entry>::iterator>::iterator mit = del_event_to_content.find(event_name);

    if(mit == del_event_to_content.end())
	throw WEBDAR_BUG; // event name absent from the map!

    list<entry>::iterator it = mit->second;
    if(!it->action)
	throw WEBDAR_BUG;
    if(!it->del)
	throw WEBDAR_BUG;

    table.foresake(&(*(it->action)));
    table.foresake(&(*(it->del)));
    events_to_delete.push_back(event_name);
	// postponing the object deletion
	// as we may get from here from the it->del pointed to actor event
	// we would not be able to return from this call if deleted from here
}


void html_form_overwrite_chain_action::purge_to_delete()
{
    deque<string>::iterator evit = events_to_delete.begin();

    while(evit != events_to_delete.end())
    {
	map<string, list<entry>::iterator>::iterator mit = del_event_to_content.find(*evit);

	if(mit == del_event_to_content.end())
	    throw WEBDAR_BUG; // event_name absent from the map!

	list<entry>::iterator it = mit->second;

	(void)del_event_to_content.erase(mit);
	(void)table_content.erase(it);

	++evit;
    }

    events_to_delete.clear();
}
