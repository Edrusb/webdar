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


    // webdar headers



    //
#include "html_tabs.hpp"

using namespace std;


html_tabs::html_tabs()
{
    adopt(&tab_bar);
    adopt(&content);
}

html_tabs::~html_tabs()
{
    deque<html_button*>::iterator it = tabs.begin();

    while(it != tabs.end())
    {
	if(*it != nullptr)
	{
	    delete *it;
	    *it = nullptr;
	}

	++it;
    }
}

void html_tabs::add_tab(const std::string & label)
{
    unsigned int tab_num = tabs.size();
    string event_name = to_string(tab_num);

    html_button* tmp = new (nothrow) html_button(label, event_name);
    if(tmp == nullptr)
	throw exception_memory();
    tabs.push_back(tmp);
    tab_bar.adopt(tmp);
    tmp->record_actor_on_event(this, event_name);

    try
    {
	map<string, unsigned int>::iterator it = corres.find(event_name);
	if(it != corres.end())
	    throw WEBDAR_BUG; // event name already exisits!?!
	corres[event_name] = tab_num;

    }
    catch(...)
    {
	if(tabs.back() != nullptr)
	    delete tabs.back();
	tabs.pop_back();
	map<string, unsigned int>::iterator it = corres.find(event_name);
	if(it != corres.end())
	    corres.erase(it);
	throw;
    }
}

void html_tabs::on_event(const std::string & event_name)
{
    map<string, unsigned int>::iterator it = corres.find(event_name);

    if(it == corres.end())
	throw WEBDAR_BUG;

    content.set_mode(it->second);
}

void html_tabs::sub_adopt(body_builder *obj)
{
    if(content.size() >= corres.size())
	throw WEBDAR_BUG;

    content.adopt(obj);
}

void html_tabs::has_adopted(body_builder *obj)
{
    if(obj == &tab_bar || obj == &content)
	return;
    else
	throw WEBDAR_BUG;
	// adopt() should only be used for our own private
	// field, sub_adopt() should be used for other objects
	// to fill the created tabs
}

string html_tabs::inherited_get_body_part(const chemin & path,
					   const request & req)
{
    return get_body_part_from_all_children(path, req);
}
