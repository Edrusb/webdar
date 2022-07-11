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
#include "tokens.hpp"
#include "exceptions.hpp"

    //
#include "webdar_css_style.hpp"

using namespace std;

namespace webdar_css_style
{

    void update_library(css_library & csslib)
    {
	css box_off, box_on, tmp;

	if(! csslib.class_exists(class_button_normal))
	{
	    try
	    {
		    // this part defines classes for
		    // html_buttons and their internal urls

		    // Common aspects
		box_off.css_clear_attributes();
		box_off.css_border_style(css::bd_all, css::bd_solid, true);
		box_off.css_border_width(css::bd_all, css::bd_medium, true);
		box_off.css_width("8em", true, true);
		box_off.css_padding("0.5em", true);
		box_off.css_margin("0.2em", true);
		box_off.css_text_align(css::al_center, true);

		    // copy common aspects to box_off and box_void
		box_on.css_inherit_from(box_off);

		    // box_off and tmp_norm COLORS
		tmp.css_clear_attributes();
		tmp.css_color(COLOR_MENU_FRONT_OFF, true);
		tmp.css_background_color(COLOR_MENU_BACK_OFF, true);
		tmp.css_font_weight_bold(true);
		tmp.css_font_style_italic(true);
		tmp.css_text_decoration(css::dc_none, true);

		    // complement to box_off
		box_off.css_inherit_from(tmp);

		    // link in box_off
		csslib.add(class_button_normal_link, tmp);
		csslib.add(class_button_normal_visited, tmp);

		box_off.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF, true);
		csslib.add(class_button_normal, box_off);

		    // Link Hover and Active in box_off
		tmp.css_color(COLOR_MENU_FRONT_HOVER_OFF, true);
		tmp.css_text_decoration(css::dc_underline, true);
		csslib.add(class_button_normal_hover, tmp);
		tmp.css_color(COLOR_MENU_FRONT_ACTIVE_OFF, true);
		csslib.add(class_button_normal_active, tmp);

		    // box_on and tmp_select COLORS
		tmp.css_color(COLOR_MENU_FRONT_ON, true);
		tmp.css_background_color(COLOR_MENU_BACK_ON, true);
		tmp.css_font_weight_bold(true);
		tmp.css_font_style_normal(true);
		tmp.css_text_decoration(css::dc_none, true);
		box_on.css_inherit_from(tmp);
		csslib.add(class_button_selected_link, tmp);
		csslib.add(class_button_selected_visited, tmp);
		box_on.css_border_color(css::bd_all, COLOR_MENU_BORDER_ON, true);
		csslib.add(class_button_selected, box_on);

		    // Link Hover and Active in box_on
		tmp.css_color(COLOR_MENU_FRONT_HOVER_ON, true);
		tmp.css_text_decoration(css::dc_underline, true);
		csslib.add(class_button_selected_hover, tmp);
		tmp.css_color(COLOR_MENU_FRONT_ACTIVE_ON, true);
		csslib.add(class_button_selected_active, tmp);
	    }
	    catch(exception_range & e)
	    {
		throw WEBDAR_BUG;
	    }
	}
	else
	{
	    if(! csslib.class_exists(class_button_normal)
	       || ! csslib.class_exists(class_button_normal_link)
	       || ! csslib.class_exists(class_button_normal_active)
	       || ! csslib.class_exists(class_button_normal_hover)
	       || ! csslib.class_exists(class_button_normal_visited)
	       || ! csslib.class_exists(class_button_selected)
	       || ! csslib.class_exists(class_button_selected_link)
	       || ! csslib.class_exists(class_button_selected_active)
	       || ! csslib.class_exists(class_button_selected_hover)
	       || ! csslib.class_exists(class_button_selected_visited))
		throw WEBDAR_BUG;
	}
    }


    void assign_normal_classes(html_button & bt)
    {
	bt.add_css_class(webdar_css_style::class_button_normal);
	bt.url_add_css_class(webdar_css_style::class_button_normal_link);
	bt.url_add_css_class(webdar_css_style::class_button_normal_active);
	bt.url_add_css_class(webdar_css_style::class_button_normal_hover);
	bt.url_add_css_class(webdar_css_style::class_button_normal_visited);
    }


    void assign_active_classes(html_button & bt)
    {
	bt.add_css_class(webdar_css_style::class_button_selected);
	bt.url_add_css_class(webdar_css_style::class_button_selected_link);
	bt.url_add_css_class(webdar_css_style::class_button_selected_active);
	bt.url_add_css_class(webdar_css_style::class_button_selected_hover);
	bt.url_add_css_class(webdar_css_style::class_button_selected_visited);
    }

}
