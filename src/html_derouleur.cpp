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
#include <algorithm>

    // webdar headers
#include "html_text.hpp"


    //
#include "html_derouleur.hpp"

using namespace std;


void html_derouleur::clear()
{
    while(order.begin() != order.end())
	remove_section(*(order.begin()));
    active_section = noactive;
}

void html_derouleur::add_section(const std::string & name, const std::string & title)
{
    section tmp;

    tmp.title = title;
    tmp.adopted.clear();

    order.push_back(name);
    try
    {
	sections[name] = tmp;
    }
    catch(...)
    {
	order.pop_back();
	throw;
    }
}

void html_derouleur::adopt_in_section(const std::string & section_name, body_builder *obj)
{
    map<string, section>::iterator it = sections.find(section_name);

    if(it == sections.end())
	throw exception_range("unknown section named: " + section_name);

    if(obj == nullptr)
	throw WEBDAR_BUG;

    it->second.adopted.push_back(obj);
    obj_to_section[obj] = section_name;
    adopt(obj);
}

void html_derouleur::clear_section(const std::string & section_name)
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
	    foresake(*objt);
	    revt = obj_to_section.find(*objt);
	    if(revt == obj_to_section.end())
		throw WEBDAR_BUG;
	    obj_to_section.erase(revt);
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

void html_derouleur::remove_section(const std::string & section_name)
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
}


void html_derouleur::will_foresake(body_builder *obj)
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

string html_derouleur::inherited_get_body_part(const chemin & path,
					       const request & req)
{
    const unsigned int size = order.size();
    map<string, section>::iterator sect;
    list<body_builder*>::iterator objt;
    chemin sub_path = path;
    html_text section_title;
    string ret = "";

    if(sub_path.size() > 0)
	sub_path.pop_front();

    section_title.add_css_class(get_css_class_group());

    for(unsigned int i = 0; i < size; ++i)
    {
	sect = sections.find(order[i]);
	if(sect == sections.end())
	    throw WEBDAR_BUG;

	    // display section title
	section_title.clear();
	section_title.add_text(1, sect->second.title);
	ret += section_title.get_body_part();

	    // if section is active display its content
	if(i == active_section)
	{
	    objt = sect->second.adopted.begin();
	    while(objt != sect->second.adopted.end())
	    {
		if(*objt == nullptr)
		    throw WEBDAR_BUG;
		ret += (*objt)->get_body_part(sub_path, req);
		++objt;
	    }
	}
    }

    return ret;
}
