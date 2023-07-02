/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2022 Denis Corbin
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
#include "html_text.hpp"
#include "tokens.hpp"
#include "exceptions.hpp"

    //
#include "html_menu.hpp"

using namespace std;

const string html_menu::changed = "html_menu_changed";

const string html_menu::box_off_class = "html_menu_box_off";
const string html_menu::box_on_class = "html_menu_box_on";
const string html_menu::box_void_class = "html_menu_box_void";
const string html_menu::url_selected_class = "url_selected";
const string html_menu::url_normal_class = "url_normal";

html_menu::html_menu()
{
    css tmp_set;

    current_mode = 0;
    previous_mode = 0;

    register_name(changed); // add the "html_menu_changed" event to this object
}

html_menu::~html_menu()
{
    vector<html_button*>::iterator it = item.begin();

    while(it != item.end())
    {
	if(*it != nullptr)
	    delete (*it);
	++it;
    }
    item.clear();
}

void html_menu::add_entry(const std::string & label)
{
    unsigned int num = item.size();
    string event_name = std::to_string(num);
    html_button *newone = new (nothrow) html_button(label, event_name);

    if(newone == nullptr)
	throw exception_memory();
    item.push_back(newone);

    newone->record_actor_on_event(this, event_name);

    if(label != "")
	newone->add_css_class(box_off_class);
    else
	newone->add_css_class(box_void_class);
    newone->url_add_css_class(url_normal_class);

	/// building the body_builder tree
    adopt(newone);

    if(item.size() == 1)
	 set_current_mode(0);
}

void html_menu::set_current_mode(unsigned int mode)
{
    unsigned int size = item.size();
    bool has_changed = (mode != current_mode);
    if(mode >= size)
	throw WEBDAR_BUG;

    if(current_mode >= size)
    	throw WEBDAR_BUG;

    if(item[current_mode] == nullptr)
	throw WEBDAR_BUG;

    if(item[mode] == nullptr)
	throw WEBDAR_BUG;

	/// all is fine, we can go on modifying the objects

    item[current_mode]->url_clear_css_classes();
    item[current_mode]->url_add_css_class(url_normal_class);
    if(item[current_mode]->get_label() == "")
    {
	item[current_mode]->clear_css_classes();
	item[current_mode]->add_css_class(box_void_class);
    }
    else
    {
	item[current_mode]->clear_css_classes();
	item[current_mode]->add_css_class(box_off_class);
    }


    item[mode]->url_clear_css_classes();
    item[mode]->url_add_css_class(url_selected_class);
    item[mode]->clear_css_classes();
    item[mode]->add_css_class(box_on_class);

    if(has_changed)
    {
	previous_mode = current_mode;
	current_mode = mode;
	act(changed); // trigger the "changed" event
    }
}

string html_menu::get_current_label() const
{
    const html_button* button = nullptr;

    if(current_mode >= item.size())
	throw WEBDAR_BUG;

    button = item[current_mode];
    if(button == nullptr)
	throw WEBDAR_BUG;

    return button->get_label();
}

void html_menu::set_current_label(const std::string & label)
{
    unsigned int i = 0;

    while(i < item.size() && item[i] != nullptr && item[i]->get_label() != label)
	++i;

    if(i < item.size())
    {
	if(item[i] == nullptr)
	    throw WEBDAR_BUG;
	else
	    set_current_mode(i);
    }
    else
	throw WEBDAR_BUG; // unknown label in this html_menu
}

void html_menu::on_event(const string & event_name)
{
	// we have registered on the event generated by each html_button
	// the event_name is the string of the decimal index of the button

    int num;

    try
    {
	num = webdar_tools_convert_to_int(event_name);
    }
    catch(exception_range & e)
    {
	throw WEBDAR_BUG;
	    // received an event we don't expect
    }

    set_current_mode(num);
}

void html_menu::new_css_library_available()
{
    css_class cl_off(box_off_class);
    css_class cl_on(box_on_class);
    css_class cl_void(box_void_class);
    css_class cl_url_sel(url_selected_class);
    css_class cl_url_norm(url_normal_class);

    css box_off;   ///< used to assign CSS attributes: unselected item
    css box_on;    ///< used to assign CSS attributes: selected item
    css box_void;  ///< used to assign CSS attributes: separators

    css tmp_set;   ///< common setup for the several css

	// Common aspects
    tmp_set.css_border_style(css::bd_all, css::bd_solid, true);
    tmp_set.css_border_width(css::bd_all, css::bd_medium, true);
    tmp_set.css_width("8em", true, true);
    tmp_set.css_padding("0.5em", true);
    tmp_set.css_margin("0.2em", true);
    tmp_set.css_text_align(css::al_center, true);

	// copy common aspects to box_on, box_off and box_void
    box_off.css_inherit_from(tmp_set);
    box_on.css_inherit_from(tmp_set);
    box_void.css_inherit_from(tmp_set);
    box_void.css_border_style(css::bd_all, css::bd_none);

	// box_off and tmp_norm COLORS
    tmp_set.css_clear_attributes();
    tmp_set.css_color(COLOR_MENU_FRONT_OFF, true);
    tmp_set.css_background_color(COLOR_MENU_BACK_OFF, true);
    tmp_set.css_font_weight_bold(true);
    tmp_set.css_font_style_italic(true);
    tmp_set.css_text_decoration(css::dc_none, true);

    cl_url_norm.set_selector(css_class::link, tmp_set);
    cl_url_norm.set_selector(css_class::visited, tmp_set);
    box_off.css_inherit_from(tmp_set);
    box_off.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF, true);

	// Link Hover and Active in box_off
    tmp_set.css_color(COLOR_MENU_FRONT_HOVER_OFF, true);
    tmp_set.css_text_decoration(css::dc_underline, true);
    cl_url_norm.set_selector(css_class::hover, tmp_set);
    tmp_set.css_color(COLOR_MENU_FRONT_ACTIVE_OFF, true);
    cl_url_norm.set_selector(css_class::active, tmp_set);

	// box_on and tmp_select COLORS
    tmp_set.css_color(COLOR_MENU_FRONT_ON, true);
    tmp_set.css_background_color(COLOR_MENU_BACK_ON, true);
    tmp_set.css_font_weight_bold(true);
    tmp_set.css_font_style_normal(true);
    tmp_set.css_text_decoration(css::dc_none, true);

    cl_url_sel.set_selector(css_class::link, tmp_set);
    cl_url_sel.set_selector(css_class::visited, tmp_set);
    box_on.css_inherit_from(tmp_set);
    box_on.css_border_color(css::bd_all, COLOR_MENU_BORDER_ON, true);

	// Link Hover and Active in box_on
    tmp_set.css_color(COLOR_MENU_FRONT_HOVER_ON, true);
    tmp_set.css_text_decoration(css::dc_underline, true);
    cl_url_sel.set_selector(css_class::hover, tmp_set);
    tmp_set.css_color(COLOR_MENU_FRONT_ACTIVE_ON, true);
    cl_url_sel.set_selector(css_class::active, tmp_set);

	// the selectors are now set,
	// setting the classes from the css boxes:

    cl_off.set_value(box_off);
    cl_on.set_value(box_on);
    cl_void.set_value(box_void);

	// recording those classes and selectors to the css_library

    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    csslib->add(cl_off);
    csslib->add(cl_on);
    csslib->add(cl_void);
    csslib->add(cl_url_sel);
    csslib->add(cl_url_norm);
}



