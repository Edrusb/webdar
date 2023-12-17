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


    //
#include "html_derouleur.hpp"

using namespace std;

const string html_derouleur::shrink_event = "shrink";

void html_derouleur::clear()
{
    while(order.begin() != order.end())
	remove_section(*(order.begin()));
    active_section = noactive;
    selfcleaning = false;
    ignore_events = false;
    css_url.clear_css_classes();
}

void html_derouleur::add_section(const std::string & name, const std::string & title)
{
    if(find(order.begin(), order.end(), name) != order.end())
	throw exception_range("section name already used in this html_derouleur");

    order.push_back(name);
    try
    {
	sections[name] = section();

	try
	{
	    map<string, section>::iterator it = sections.find(name);
	    if(it == sections.end())
		throw WEBDAR_BUG;

	    if(it->second.title != nullptr)
		throw WEBDAR_BUG;

	    it->second.title = new (nothrow) html_button(title, name);
	    it->second.shrinker = new (nothrow) html_button(title, shrink_event);

	    if(it->second.title == nullptr || it->second.shrinker == nullptr)
		throw exception_memory();

	    adopt(it->second.title);
	    adopt(it->second.shrinker);

	    it->second.title->record_actor_on_event(this, name);
	    it->second.shrinker->record_actor_on_event(this, shrink_event);

	    if(! css_url.is_empty())
	    {
		it->second.title->url_clear_css_classes();
		it->second.title->url_add_css_class(css_url);
		it->second.shrinker->url_clear_css_classes();
		it->second.shrinker->url_add_css_class(css_url);
	    }

	    css_class_group css_box = get_css_class_group();
	    if(! css_box.is_empty())
	    {
		it->second.title->clear_css_classes();
		it->second.title->add_css_class(css_box);
		it->second.shrinker->clear_css_classes();
		it->second.shrinker->add_css_class(css_box);
	    }
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

void html_derouleur::set_active_section(signed int num)
{
    if(num >= order.size() && num != noactive)
	throw exception_range("invalid section number given to html_derouleur object");

    active_section = num;
}

void html_derouleur::url_add_css_class(const std::string & name)
{
    css_url.add_css_class(name); // record this new class for future sections
    map<string, section>::iterator it = sections.begin();

    while(it != sections.end())
    {
	if(it->second.title == nullptr)
	    throw WEBDAR_BUG;
	it->second.title->url_add_css_class(name);
	it->second.shrinker->url_add_css_class(name);
	++it;
    }
}

void html_derouleur::url_add_css_class(const css_class_group & cg)
{
    css_url.add_css_class(cg); // record this new class for future sections
    map<string, section>::iterator it = sections.begin();

    while(it != sections.end())
    {
	if(it->second.title == nullptr)
	    throw WEBDAR_BUG;
	it->second.title->url_add_css_class(cg);
	it->second.shrinker->url_add_css_class(cg);
	++it;
    }
}

void html_derouleur::on_event(const std::string & event_name)
{
    unsigned int size = order.size();
    unsigned int found = 0;

    if(ignore_events)
	return;

    if(event_name == shrink_event)
    {
	active_section = noactive;
	return;
    }

    while(found < size && order[found] != event_name)
	++found;

    if(found >= size)
	throw WEBDAR_BUG; // unexpected event_name received;

    active_section = found;
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

void html_derouleur::css_classes_have_changed()
{
    map<string, section>::iterator it = sections.begin();
    css_class_group updated_set = get_css_class_group();

    while(it != sections.end())
    {
	if(it->second.title == nullptr)
	    throw WEBDAR_BUG;
	it->second.title->clear_css_classes();
	it->second.title->add_css_class(updated_set);
	it->second.shrinker->clear_css_classes();
	it->second.shrinker->add_css_class(updated_set);
	++it;
    }
}

string html_derouleur::inherited_get_body_part(const chemin & path,
					       const request & req)
{
    string ret;
    chemin sub_path = path;

    if(sub_path.size() > 0)
	sub_path.pop_front();

	// first thing, have the classes generating the events
	// and updating objects accordingly
    (void)generate_html(sub_path, req);

	// now freezing the even for it does not change a second time
	// but as all components have their status we can now
	// have a correct status for all components (even those
	// that would have provided their content before an next event
	// would have changed it

    ignore_events = true;
    try
    {
	ret = generate_html(sub_path, req);
    }
    catch(...)
    {
	ignore_events = false;
	throw;
    }
    ignore_events = false;

    return ret;
}


string html_derouleur::generate_html(const chemin & path,
				     const request & req)
{
    const unsigned int size = order.size();
    map<string, section>::iterator sect;
    list<body_builder*>::iterator objt;
    string ret = "";

    for(unsigned int i = 0; i < size; ++i)
    {
	sect = sections.find(order[i]);
	if(sect == sections.end())
	    throw WEBDAR_BUG;

	    // display section title
	if(sect->second.title == nullptr)
	    throw WEBDAR_BUG;

	    // if section is active display its content
	if(i == active_section)
	{
	    ret += sect->second.shrinker->get_body_part(path, req);

	    objt = sect->second.adopted.begin();
	    while(objt != sect->second.adopted.end())
	    {
		if(*objt == nullptr)
		    throw WEBDAR_BUG;
		ret += (*objt)->get_body_part(path, req);
		++objt;
	    }
	}
	else
	    ret += sect->second.title->get_body_part(path, req);

    }

    return ret;
}
