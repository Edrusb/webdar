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
#include "tokens.hpp"


    //
#include "html_tabs.hpp"

using namespace std;

const char* html_tabs::tab_on = "html_tabs_on";
const char* html_tabs::tab_on_url = "html_tabs_on_url";
const char* html_tabs::tab_off = "html_tabs_off";
const char* html_tabs::tab_off_url = "html_tabs_off_url";
const char* html_tabs::menu_bar = "html_tabs_bar";
const char* html_tabs::tab_sep = "html_tabs_sep";

html_tabs::html_tabs(): html_table(1)
{
    css_border_collapsed(true);
    set_css_class_first_row(tab_sep);

    adopt(&tab_bar);
    adopt(&content);
    tab_bar.add_css_class(menu_bar);
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
    tmp->url_add_css_class(tab_off_url);

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

    set_mode(it->second);
}

void html_tabs::sub_adopt(body_builder *obj)
{
    if(content.size() >= corres.size())
	throw WEBDAR_BUG;

    content.adopt(obj);

    unsigned int cur = content.get_mode();
    set_mode(content.size() - 1); // set css stuff to the new tab
    set_mode(cur);  // keep back the same active tab
}

void html_tabs::has_adopted(body_builder *obj)
{
    if(obj == &tab_bar || obj == &content)
	html_table::has_adopted(obj); // to update html_div and above html_level datastructures
    else
	throw WEBDAR_BUG;
	// adopt() should only be used for our own private
	// field, sub_adopt() should be used for other objects
	// to fill the created tabs

}

string html_tabs::inherited_get_body_part(const chemin & path,
					   const request & req)
{

	// this will trigger events and change of display
	// but since some component may already have returned
	// their body part, their change will not reflect is
	// the return provided by this first call
    (void)get_body_part_from_all_children(path, req);

	// so we call it a second time
    return html_table::inherited_get_body_part(path, req);
}


void html_tabs::new_css_library_available()
{
    std::unique_ptr<css_library> & csslib = lookup_css_library();

    if(!csslib)
	throw WEBDAR_BUG;

    if(csslib->class_exists(tab_on))
    {
	if(! csslib->class_exists(tab_off))
	    throw WEBDAR_BUG;

	    // else nothing to do, class definitions have already been setup
    }
    else
    {
	if(csslib->class_exists(tab_off))
	    throw WEBDAR_BUG;

	css tmp;
	css_class forlink(tab_off_url);

	    // the tab / content separator
	tmp.css_border_width(css::bd_bottom, css::bd_thin, true);
	tmp.css_border_color(css::bd_bottom, COLOR_MENU_BORDER_OFF, true);
	tmp.css_border_style(css::bd_bottom, css::bd_solid, true);
	csslib->add(tab_sep, tmp);

	    // the selected tab (not their text)
	tmp.clear();
	tmp.css_float(css::fl_left);
	css::bd_width width = css::bd_thin;
	tmp.css_border_width(css::bd_top, width);
	tmp.css_border_width(css::bd_left, width);
	tmp.css_border_width(css::bd_right, width);
	tmp.css_background_color(COLOR_MENU_BACK_ON);
	tmp.css_border_style(css::bd_top, css::bd_solid);
	tmp.css_border_style(css::bd_left, css::bd_solid);
	tmp.css_border_style(css::bd_right, css::bd_solid);
	tmp.css_border_color(css::bd_all, COLOR_MENU_BORDER_ON);
	tmp.css_width("8em", true);
	tmp.css_padding("0.5em", true);
	tmp.css_margin_top("0.2em", true);
	tmp.css_margin_right("0.2em", true);
	tmp.css_margin_left("0.2em", true);
	tmp.css_margin_bottom("0", true);
	tmp.css_text_align(css::al_center, true);
	tmp.css_corner_radius("20%", "20%", "0", "0");
	csslib->add(tab_on, tmp);

	    // the unselected tabs (not their text)
	tmp.css_background_color(COLOR_MENU_BACK_OFF);
	tmp.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF);
	csslib->add(tab_off, tmp);

	    // the tab bar
	tmp.clear();
	tmp.css_width("100%", true);
	csslib->add(menu_bar, tmp);

	    // the text of unselected tabs
	tmp.clear();
	tmp.css_color(COLOR_MENU_FRONT_OFF, true);
	tmp.css_background_color(COLOR_MENU_BACK_OFF, true);
	tmp.css_text_decoration(css::dc_none, true);
	tmp.css_font_weight_bold(true);
	tmp.css_font_style_italic(true);
	forlink.set_selector(css_class::link, tmp);
	forlink.set_selector(css_class::visited, tmp);
	tmp.css_color(COLOR_MENU_FRONT_HOVER_OFF, true);
	tmp.css_text_decoration(css::dc_underline, true);
	forlink.set_selector(css_class::hover, tmp);
	tmp.css_color(COLOR_MENU_FRONT_ACTIVE_OFF, true);
	forlink.set_selector(css_class::active, tmp);
	csslib->add(forlink);

	    // the text of the selected tab
	forlink.change_name(tab_on_url);
	forlink.clear_selector(css_class::link);
	forlink.clear_selector(css_class::visited);
	forlink.clear_selector(css_class::hover);
	forlink.clear_selector(css_class::active);
	tmp.css_color(COLOR_MENU_FRONT_ON, true);
	tmp.css_font_style_normal(false);
	tmp.css_text_decoration(css::dc_none, true);
	tmp.css_background_color(COLOR_MENU_BACK_ON, true);
	forlink.set_selector(css_class::link, tmp);
	forlink.set_selector(css_class::visited, tmp);
	tmp.css_color(COLOR_MENU_FRONT_HOVER_ON, true);
	tmp.css_text_decoration(css::dc_underline, true);
	forlink.set_selector(css_class::hover, tmp);
	tmp.css_color(COLOR_MENU_FRONT_ACTIVE_ON, true);
	forlink.set_selector(css_class::active, tmp);
	csslib->add(forlink);
    }
}

void html_tabs::set_mode(unsigned int mode)
{
	// setting currently active tab to non-active
    if(content.size() > 0)
    {
	if(tabs.size() < content.get_mode())
	    throw WEBDAR_BUG;
	if(tabs[content.get_mode()] == nullptr)
	    throw WEBDAR_BUG;
	tabs[content.get_mode()]->clear_css_classes();
	tabs[content.get_mode()]->url_clear_css_classes();
	tabs[content.get_mode()]->add_css_class(tab_off);
	tabs[content.get_mode()]->url_add_css_class(tab_off_url);
    }
    else
	throw WEBDAR_BUG;

	// setting the new active tab to active
    if(mode >= tabs.size())
	throw WEBDAR_BUG;

    if(tabs[mode] == nullptr)
	throw WEBDAR_BUG;

    tabs[mode]->clear_css_classes();
    tabs[mode]->url_clear_css_classes();
    tabs[mode]->add_css_class(tab_on);
    tabs[mode]->url_add_css_class(tab_on_url);

    content.set_mode(mode);
}
