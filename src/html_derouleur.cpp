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
    html_aiguille::clear();
    ignore_events = false;
    css_url.clear_css_classes();
}

void html_derouleur::url_add_css_class(const std::string & name)
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
}

void html_derouleur::on_event(const std::string & event_name)
{
    if(ignore_events)
	return;

    if(event_name == shrink_event)
	set_active_section(noactive);
    else
	set_active_section(event_name);
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

	// first thing, have the classes generating the events
	// and updating objects accordingly
    if(get_visible())
	(void)generate_html(sub_path, req);

    ack_visible();
    if(get_visible())
    {
	    // now freezing the even for it does not change a second time
	    // but as all components have their status we can now
	    // have a correct status for all components (even those
	    // that would have provided their content before an event
	    // would have changed it)

	ignore_events = true;
	try
	{
	    request tmp = req;

	    tmp.post_to_get();
	    ret = generate_html(sub_path, tmp);
	}
	catch(...)
	{
	    ignore_events = false;
	    throw;
	}
	ignore_events = false;
    }

    return ret;
}


void html_derouleur::section_added(const std::string & name, const std::string & title)
{
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

void html_derouleur::section_removed(const std::string & section_name)
{
    map<string, section>::iterator it = sections.find(section_name);

    if(it == sections.end())
	throw WEBDAR_BUG; // unknown section !?!

    sections.erase(it);
}

string html_derouleur::generate_html(const chemin & path,
				     const request & req)
{
    const unsigned int num_sect = html_aiguille::size();
    map<string, section>::iterator sect;
    string ret = "";

    for(unsigned int i = 0; i < num_sect; ++i)
    {
	sect = sections.find(num_to_section_name(i));
	if(sect == sections.end())
	    throw WEBDAR_BUG;

	    // display section title
	if(sect->second.title == nullptr)
	    throw WEBDAR_BUG;
	if(sect->second.shrinker == nullptr)
	    throw WEBDAR_BUG;

	    // if section is active display its content
	if(i == get_active_section())
	{
	    ret += sect->second.shrinker->get_body_part(path, req);
	    ret += html_aiguille::inherited_get_body_part(path, req);
	}
	else
	    ret += sect->second.title->get_body_part(path, req);
    }

    return ret;
}
