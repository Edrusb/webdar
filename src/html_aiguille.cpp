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
#include <algorithm>

    // webdar headers


    //
#include "html_aiguille.hpp"

using namespace std;

void html_aiguille::clear()
{
    while(order.begin() != order.end())
	remove_section(*(order.begin()));
    active_section = noactive;
    selfcleaning = true;
    my_body_part_has_changed();
}

void html_aiguille::add_section(const string & name, const string & title)
{
    if(find(order.begin(), order.end(), name) != order.end())
	throw exception_range("section name already used in this html_aiguille");

    order.push_back(name);
    try
    {
	sections[name] = section();

	try
	{
	    map<string, section>::iterator it = sections.find(name);
	    if(it == sections.end())
		throw WEBDAR_BUG;

	    it->second.title = title;
	    adopt(&(it->second.global_visibility));
	    it->second.global_visibility.set_visible(false);
	}
	catch(...)
	{
	    sections.erase(name);
	    throw;
	}
    }
    catch(...)
    {
	order.pop_back();
	throw;
    }

    section_added(name, title); // eventually inform inherited class if applicable

	// adding a section has no influence on the return
	// of inherited_get_body_part()
	// no need to fire my_body_part_has_changed()
}

void html_aiguille::adopt_in_section(const string & section_name, body_builder* obj)
{
    map<string, section>::iterator it = sections.find(section_name);
    unsigned int sect_num = section_name_to_num(section_name);

    if(it == sections.end())
	throw exception_range("unknown section named: " + section_name);

    if(obj == nullptr)
	throw WEBDAR_BUG;

    obj_to_section[obj] = section_name;
    it->second.adopted.push_back(obj);
    it->second.global_visibility.adopt(obj); // this will trigger body_builder::my_body_part_has_changed()
}

void html_aiguille::adopt_in_section(signed int num, body_builder* obj)
{
    if(num >= size())
	throw exception_range("invalid section number given to html_aiguille object");
    adopt_in_section(order[num], obj);
}

void html_aiguille::clear_section(const string & section_name)
{
    map<string, section>::iterator it = sections.find(section_name);

    selfcleaning = true;
    try
    {
	if(it == sections.end())
	    throw exception_range("unknown section to clear named: " + section_name);

	list<body_builder*>::iterator objt = it->second.adopted.begin();
	map<body_builder*, string>::iterator revt;

	while(objt != it->second.adopted.end())
	{
	    if(*objt == nullptr)
		throw WEBDAR_BUG;
	    revt = obj_to_section.find(*objt);
	    if(revt == obj_to_section.end())
		throw WEBDAR_BUG;
	    obj_to_section.erase(revt);
	    it->second.global_visibility.foresake(*objt);
	    *objt = nullptr;
	    ++objt;
	}

	it->second.adopted.clear();
    }
    catch(...)
    {
	selfcleaning = false;
	throw;
    }
    selfcleaning = false;
}

void html_aiguille::clear_section(signed int num)
{
    if(num >= size())
	throw exception_range("invalid section number given to html_aiguille object");
    clear_section(order[num]);
}

void html_aiguille::remove_section(const string & section_name)
{
    clear_section(section_name);

    map<string, section>::iterator it = sections.find(section_name);

    if(it == sections.end())
	throw WEBDAR_BUG; /// clear_section succeeded, and we don't ???

    sections.erase(it);

    deque<string>::iterator ut = find(order.begin(), order.end(), section_name);
    if(ut == order.end())
	throw WEBDAR_BUG;

    order.erase(ut);

    if(active_section >= order.size())
	active_section = noactive;

    section_removed(section_name); // eventually inform inherited class

    my_body_part_has_changed(); // necessary if the removed section had no object to foresake
}

void html_aiguille::remove_section(signed int num)
{
    if(num >= size())
	throw exception_range("invalid section number given to html_aiguille object");
    remove_section(order[num]);
}

void html_aiguille::set_active_section(const string & name)
{
   unsigned int i = 0;

   while(i < order.size() && name != order[i])
       ++i;

   if(i < order.size())
       set_active_section(i);
   else
       throw exception_range("invalid section name given to html_aiguille object");
}

void html_aiguille::set_active_section(signed int num)
{
    if((num >= order.size() || num < 0) && num != noactive)
	throw exception_range("invalid section number given to html_aiguille object");

    if(active_section != num)
    {
	if(active_section != noactive)
	    set_visibility(active_section, false);
	if(num != noactive)
	    set_visibility(num, true);
	active_section = num;
	my_body_part_has_changed();
    }
}

unsigned int html_aiguille::section_name_to_num(const string & name) const
{
    unsigned int size = order.size();
    unsigned int found = 0;

    while(found < size && order[found] != name)
	++found;

    if(found >= size)
	throw exception_range("Unknown section name in html_aiguille");
    else
	return found;
}

void html_aiguille::void_child_will_foresake(body_builder* voidobj, body_builder *obj)
{
    if(selfcleaning)
	return;

    map<body_builder*, string>::iterator it = obj_to_section.find(obj);

    if(it == obj_to_section.end())
	throw WEBDAR_BUG;

    map<string, section>::iterator sect = sections.find(it->second);
    if(sect == sections.end())
	throw WEBDAR_BUG;

    list<body_builder*>::iterator objt = sect->second.adopted.begin();
    while(objt != sect->second.adopted.end() && *objt != obj)
	++objt;

    if(objt == sect->second.adopted.end())
	throw WEBDAR_BUG;
    sect->second.adopted.erase(objt);

    obj_to_section.erase(it);
}

string html_aiguille::inherited_get_body_part(const chemin & path,
					       const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_aiguille::set_visibility(signed int section_num, bool visible)
{
    map<string, section>::iterator objects = sections.find(order[section_num]);

    if(objects == sections.end())
	throw WEBDAR_BUG;

    objects->second.global_visibility.set_visible(visible);
}
