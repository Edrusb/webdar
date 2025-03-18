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
#include "html_derouleur.hpp"

using namespace std;

const string html_derouleur::shrink_event = "shrink";

void html_derouleur::clear()
{
    switcher.clear();
    sections.clear();
    css_url.clear_css_classes();
}

void html_derouleur::add_section(const string & name, const string & title)
{
    switcher.add_section(name, title);
    sections[name] = section();

    try
    {
	map<string, section>::iterator it = sections.find(name);
	if(it == sections.end())
	    throw WEBDAR_BUG;

	if(it->second.title != nullptr)
	    throw WEBDAR_BUG;
	if(it->second.shrinker != nullptr)
	   throw WEBDAR_BUG;

	it->second.title = new (nothrow) html_button(title, name);
	it->second.shrinker = new (nothrow) html_button(title, shrink_event);

	if(it->second.title == nullptr || it->second.shrinker == nullptr)
	    throw exception_memory();

	adopt(it->second.title);
	adopt(it->second.shrinker);

	it->second.title->record_actor_on_event(this, name);
	it->second.shrinker->record_actor_on_event(this, shrink_event);

	it->second.shrinker->set_anchor(true);
	it->second.title->set_anchor_to(it->second.shrinker->get_anchor());

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

void html_derouleur::adopt_in_section(const std::string & section_name, body_builder* obj)
{
    if(obj == nullptr)
	throw WEBDAR_BUG;
    else
    {
	map<string, section>::iterator it = sections.find(section_name);
	if(it != sections.end()
	   && it->second.shrinker != nullptr)
	    obj->set_anchor_to(it->second.shrinker->get_anchor());
	else
	    throw WEBDAR_BUG; // should have a shrinker with a valid anchor
    }

    switcher.adopt_in_section(section_name, obj);
}

void html_derouleur::adopt_in_section(signed int num, body_builder* obj)
{
    adopt_in_section(switcher.num_to_section_name(num), obj);
}


void html_derouleur::remove_section(const string & section_name)
{
    map<string, section>::iterator it = sections.find(section_name);

    if(it == sections.end())
	throw WEBDAR_BUG; // unknown section !?!

    sections.erase(it);
    switcher.remove_section(section_name);
}

void html_derouleur::section_set_visible(const string & name, bool visible)
{
    map<string, section>::iterator it = sections.find(name);

    if(it == sections.end())
	throw WEBDAR_BUG; // unknown section

    it->second.visible = visible;
    if(!visible)
    {
	unsigned int active = switcher.get_active_section();
	unsigned int modified = switcher.section_name_to_num(name);

	if(active == modified)
	    switcher.set_active_section(html_aiguille::noactive);
	    // we hidded the current active section
	    // by coherence we set switcher with
	    // no active section
    }
}

void html_derouleur::url_add_css_class(const string & name)
{
    css_url.add_css_class(name); // record this new class for future sections
    map<string, section>::iterator it = sections.begin();

    while(it != sections.end())
    {
	if(it->second.title == nullptr)
	    throw WEBDAR_BUG;
	if(it->second.shrinker == nullptr)
	    throw WEBDAR_BUG;
	it->second.title->url_add_css_class(name);
	it->second.shrinker->url_add_css_class(name);
	++it;
    }

    my_body_part_has_changed();
}

void html_derouleur::url_add_css_class(const css_class_group & cg)
{
    css_url.add_css_class(cg); // record this new class for future sections
    map<string, section>::iterator it = sections.begin();

    while(it != sections.end())
    {
	if(it->second.title == nullptr)
	    throw WEBDAR_BUG;
	if(it->second.shrinker == nullptr)
	    throw WEBDAR_BUG;
	it->second.title->url_add_css_class(cg);
	it->second.shrinker->url_add_css_class(cg);
	++it;
    }

    my_body_part_has_changed();
}

void html_derouleur::on_event(const string & event_name)
{
    if(event_name == shrink_event)
	switcher.set_active_section(html_aiguille::noactive);
    else
	switcher.set_active_section(event_name);

	// my_body_part_has_changed() is called from html_aiguille::set_active_section()
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
    string ret = "";
    chemin sub_path = path;

    if(sub_path.size() > 0)
	sub_path.pop_front();

    return generate_html(sub_path, req);
}


string html_derouleur::generate_html(const chemin & path,
				     const request & req)
{
    const unsigned int num_sect = switcher.size();
    map<string, section>::iterator sect;
    string ret = "";

    for(unsigned int i = 0; i < num_sect; ++i)
    {
	sect = sections.find(switcher.num_to_section_name(i));
	if(sect == sections.end())
	    throw WEBDAR_BUG;

	    // display section title
	if(sect->second.title == nullptr)
	    throw WEBDAR_BUG;
	if(sect->second.shrinker == nullptr)
	    throw WEBDAR_BUG;

	    // if section is active, display its content
	if(sect->second.visible)
	{
	    if(i == switcher.get_active_section())
	    {
		ret += sect->second.shrinker->get_body_part(path, req);
		ret += switcher.get_body_part(path, req);
	    }
	    else
		ret += sect->second.title->get_body_part(path, req);
	}
	else
	{
	    if(i == switcher.get_active_section())
		throw WEBDAR_BUG;
		// active section is set as invisible!!!

	    // else we skip the section title and content
	}
    }

    return ret;
}
