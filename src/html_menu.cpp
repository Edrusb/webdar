/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013 Denis Corbin
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

html_menu::html_menu()
{
    css tmp_set;

    current_mode = 0;
    previous_mode = 0;

	// Common aspects
    box_off.css_border_style(css::bd_all, css::bd_solid, true);
    box_off.css_border_width(css::bd_all, css::bd_medium, true);
    box_off.css_width("8em", true, true);
    box_off.css_padding("0.5em", true);
    box_off.css_margin("0.2em", true);
    box_off.css_text_align(al_center, true);

	// copy common aspects to box_off and box_void
    box_on.css_inherit_from(box_off);
    box_void.css_inherit_from(box_off);
    box_void.css_border_style(css::bd_all, css::bd_none);

	// box_off and tmp_norm COLORS
    tmp_set.css_clear_attributes();
    tmp_set.css_color(COLOR_MENU_FRONT_OFF, true);
    tmp_set.css_background_color(COLOR_MENU_BACK_OFF, true);
    tmp_set.css_font_weight_bold(true);
    tmp_set.css_font_style_italic(true);
    tmp_set.css_text_decoration(css::dc_none, true);
    box_off.css_inherit_from(tmp_set);
    url_normal.set_style_link(tmp_set);
    url_normal.set_style_visited(tmp_set);
    box_off.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF, true);

	// Link Hover and Active in box_off
    tmp_set.css_color(COLOR_MENU_FRONT_HOVER_OFF, true);
    tmp_set.css_text_decoration(dc_underline, true);
    url_normal.set_style_hover(tmp_set);
    tmp_set.css_color(COLOR_MENU_FRONT_ACTIVE_OFF, true);
    url_normal.set_style_active(tmp_set);

	// box_on and tmp_select COLORS
    tmp_set.css_color(COLOR_MENU_FRONT_ON, true);
    tmp_set.css_background_color(COLOR_MENU_BACK_ON, true);
    tmp_set.css_font_weight_bold(true);
    tmp_set.css_font_style_normal(true);
    tmp_set.css_text_decoration(css::dc_none, true);
    box_on.css_inherit_from(tmp_set);
    url_selected.set_style_link(tmp_set);
    url_selected.set_style_visited(tmp_set);
    box_on.css_border_color(css::bd_all, COLOR_MENU_BORDER_ON, true);

	// Link Hover and Active in box_on
    tmp_set.css_color(COLOR_MENU_FRONT_HOVER_ON, true);
    tmp_set.css_text_decoration(dc_underline, true);
    url_selected.set_style_hover(tmp_set);
    tmp_set.css_color(COLOR_MENU_FRONT_ACTIVE_ON, true);
    url_selected.set_style_active(tmp_set);

    tmp_set.css_clear_attributes();

    global.set_no_CR();

    adopt(&global);
    adopt(&url_normal);
    adopt(&url_selected);

    register_name(changed); // add the "html_menu_changed" event to this object
}

html_menu::~html_menu()
{
    vector<boite *>::iterator it = item.begin();

    while(it != item.end())
    {
	if(*it != NULL)
	    delete (*it);
	++it;
    }
    item.clear();
}

void html_menu::add_entry(const std::string & reference, const std::string & label)
{
    chemin tmp = get_path();
    unsigned int i = 0;
    unsigned int s = item.size();
    boite *box = NULL;

    while(i < s && item[i] != NULL && item[i]->value != reference)
	++i;

    if(i < s)
	throw WEBDAR_BUG; // duplicated reference

    tmp.push_back(reference);

    box = new (nothrow) boite(tmp.display(), label);
    if(box == NULL)
	throw exception_memory();
    item.push_back(box);

    box->value = reference;
    box->inside.css_border_style(css::bd_all, css::bd_none);
    box->inside.set_class(url_normal.get_class_id());
    if(label != "")
	box->surround.css_inherit_from(box_off);
    else
	box->surround.css_inherit_from(box_void);
    box->surround.set_no_CR();

	/// building the body_builder tree
    box->surround.adopt(&(box->inside));
    global.adopt(&(box->surround));

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

    if(item[current_mode] == NULL)
	throw WEBDAR_BUG;

    if(item[mode] == NULL)
	throw WEBDAR_BUG;

	/// all is fine, we can go on modifying the objects

    if(item[current_mode]->inside.get_label() == "")
	item[current_mode]->surround.css_inherit_from(box_void, false, true);
    else
    {
	item[current_mode]->surround.css_inherit_from(box_off, false, true);
	item[current_mode]->inside.set_class(url_normal.get_class_id());
    }

    item[mode]->surround.css_inherit_from(box_on, false, true);
    item[mode]->inside.set_class(url_selected.get_class_id());
    if(has_changed)
    {
	previous_mode = current_mode;
	current_mode = mode;
	act(changed); // trigger the "changed" event
    }
}

string html_menu::get_current_label() const
{
    boite *box = NULL;

    if(current_mode >= item.size())
	throw WEBDAR_BUG;

    box = item[current_mode];
    if(box == NULL)
	throw WEBDAR_BUG;

    return box->value;
}

void html_menu::set_current_label(const std::string & label)
{
    unsigned int i = 0;

    while(i < item.size() && item[i] != NULL && item[i]->value != label)
	++i;

    if(i < item.size())
    {
	if(item[i] == NULL)
	    throw WEBDAR_BUG;
	else
	    set_current_mode(i);
    }
    else
	throw WEBDAR_BUG; // unknown label in this html_menu
}

string html_menu::get_body_part(const chemin & path,
			   const request & req)
{
	// reading the requested path to determin
	// whether a change of mode is required
    chemin target = req.get_uri().get_path();
    string choice;

    if(!target.empty())
    {
	choice = target.back();
	target.pop_back();
    }
    else
	choice = "";

    if(target == get_path() && choice != "")
    {
	    // the requested link is us
	    // except the last part that we tailed out
	    // and which we ignore if empty
	unsigned int i = 0;
	unsigned int size = item.size();
	while(i < size && item[i] != NULL && item[i]->value != choice)
	    ++i;
	if(i < size)
	    set_current_mode(i);
    }

    return get_body_part_from_all_children(path, req);
}

void html_menu::path_has_changed()
{
    vector<boite *>::iterator it = item.begin();

    while(it != item.end())
    {
	chemin chem = get_path();

	if(*it == NULL)
	    throw WEBDAR_BUG;
	chem.push_back((*it)->value);
	(*it)->inside.change_url(chem.display(false));
	(*it)->inside.set_class(url_normal.get_class_id());
	++it;
    }

    if(item.size() > 0)
	set_current_mode(current_mode);
}

void html_menu::css_updated(bool inherit)
{
    global.css_inherit_from(*this, true, true);
}


