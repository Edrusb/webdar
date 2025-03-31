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
#include "tokens.hpp"
#include "webdar_css_style.hpp"

    //
#include "html_tabs.hpp"

using namespace std;

const char* html_tabs::tab_shape = "html_tabs_shape";
const char* html_tabs::menu_bar = "html_tabs_bar";
const char* html_tabs::tab_sep = "html_tabs_sep";
const char* html_tabs::css_content_wrapper = "html_tabs_wrapper";

html_tabs::html_tabs(): current_mode(1) // to allow change to take place upon mode change
{

	// adoption tree
    adopt(&tab_bar);
    adopt(&line);
    content_wrapper.adopt(&content);
    adopt(&content_wrapper);

	// css
    tab_bar.add_css_class(menu_bar);
    line.add_css_class(css_content_wrapper);
    content_wrapper.add_css_class(css_content_wrapper);
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

void html_tabs::add_tab(const string & label, const string & tag)
{
    unsigned int tab_num = tabs.size();
    string event_name = to_string(tab_num);

    html_button* tmp = new (nothrow) html_button(label, event_name);
    if(tmp == nullptr)
	throw exception_memory();
    tabs.push_back(tmp);
    tab_bar.adopt(tmp);
    tmp->record_actor_on_event(this, event_name);
    webdar_css_style::normal_button(*tmp);
    tmp->url_add_css_class(tab_shape);

    try
    {
	map<string, unsigned int>::iterator it = corres.find(event_name);
	if(it != corres.end())
	    throw WEBDAR_BUG; // event name already exisits!?!
	corres[event_name] = tab_num;
	content.add_section(tag, label);
	if(content.size() == 1) // by default no section is active
	    content.set_active_section(0);  // we activate the first tab content at its creation

	unsigned int cur = content.get_active_section();
	set_mode(content.size() - 1); // set css stuff to the new tab
	set_mode(cur);  // keep back the same active tab
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

    my_body_part_has_changed();
}

void html_tabs::on_event(const string & event_name)
{
    map<string, unsigned int>::iterator it = corres.find(event_name);

    if(it == corres.end())
	throw WEBDAR_BUG;

    set_mode(it->second);
}

void html_tabs::adopt_in_section(const string & tag, body_builder *obj)
{
    content.adopt_in_section(tag, obj);
}

void html_tabs::adopt_in_section(signed int num, body_builder* obj)
{
    content.adopt_in_section(num, obj);
}

string html_tabs::inherited_get_body_part(const chemin & path,
					   const request & req)
{
    return get_body_part_from_all_children(path, req);
}


void html_tabs::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();

    if(!csslib)
	throw WEBDAR_BUG;
    webdar_css_style::update_library(*csslib);

    if(!csslib->class_exists(tab_shape))
    {
	css tmp;

	    // the tab / content separator
	tmp.css_border_width(css::bd_bottom, css::bd_thin);
	tmp.css_border_color(css::bd_bottom, COLOR_MENU_BORDER_OFF);
	tmp.css_border_style(css::bd_bottom, css::bd_solid);
	csslib->add(tab_sep, tmp);

	    // the selected tab (not their text)
	tmp.clear();
	tmp.css_float(css::fl_left);
	css::bd_width width = css::bd_thin;
	tmp.css_border_width(css::bd_top, width);
	tmp.css_border_width(css::bd_left, width);
	tmp.css_border_width(css::bd_right, width);
	tmp.css_margin_top("0.2em");
	tmp.css_margin_right("0.2em");
	tmp.css_margin_left("0.2em");
	tmp.css_margin_bottom("0");
	tmp.css_corner_radius("20%", "20%", "0", "0");
	tmp.css_height("4em", false);
	csslib->add(tab_shape, tmp);

	    // the tab bar
	tmp.clear();
	tmp.css_width("100%", true);
	csslib->add(menu_bar, tmp);

	tmp.clear();
	tmp.css_width("100%", false);
	tmp.css_box_sizing(css::bx_border);
	csslib->add(css_content_wrapper, tmp);
    }
}

void html_tabs::set_mode(unsigned int mode)
{
    if(mode == current_mode)
	return;

	// setting currently active tab to non-active
    if(current_mode < tabs.size())
    {
	if(tabs.size() < content.size())
	    throw WEBDAR_BUG;
	if(tabs[current_mode] == nullptr)
	    throw WEBDAR_BUG;
	webdar_css_style::normal_button(*(tabs[current_mode]));
	tabs[current_mode]->add_css_class(tab_shape);

    }
    else
    {
	if(mode != 0 || current_mode != 1)
	    throw WEBDAR_BUG;
	    // else this is the initialization context
    }

    current_mode = mode;

	// setting the new active tab to active

    if(mode >= tabs.size())
	throw WEBDAR_BUG;

    if(tabs[mode] == nullptr)
	throw WEBDAR_BUG;

    webdar_css_style::active_button(*(tabs[mode]));
    tabs[mode]->add_css_class(tab_shape);

    content.set_active_section(mode);
	// html_aiguille::set_active_section will trigger my_body_part_has_changed()
}


