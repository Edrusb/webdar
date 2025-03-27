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
#include <libthreadar/libthreadar.hpp>
#include <dar/tools.hpp>

    // webdar headers
#include "tokens.hpp"
#include "exceptions.hpp"

    //
#include "webdar_css_style.hpp"

using namespace std;

namespace webdar_css_style
{
    static libthreadar::mutex init_lock;
    static bool initialized = false;

    static map<string, css> classes;

    static css_class c_width_full(width_full);     ///< full screen large buttons (html_derouleur for exmple)
    static css_class c_width_8em(width_8em);       ///< normal width buttons (html_button, html_double_button, html_tabs,...)

    static css_class c_btn_off(btn_off);           ///< used to assign CSS attributes: unselected item
    static css_class c_btn_on(btn_on);             ///< used to assign CSS attributes: selected item
    static css_class c_btn_void(btn_void);         ///< used to assign CSS attributes: separators
    static css_class c_btn_grey(btn_grey);         ///< used to assign CSS attributes: disabled or greyed
    static css_class c_url_selected(url_selected); ///< links when button selected
    static css_class c_url_normal(url_normal);     ///< links when button no selected
    static css_class c_url_void(url_void);         ///< links when button is a void
    static css_class c_url_grey(url_grey);         ///< links when button is disabled or greyed
    static css_class c_bold_text(bold_text);       ///< bold text
    static css_class c_red_border(red_border);     ///< red border
    static css_class c_indent(indent);             ///< indentation (4em shift to the right)

    static void init()
    {
        if(initialized)
            return; // bypassing mutex as this field once true stays true forever

        init_lock.lock();
        try
        {
            if(!initialized)
            {
                css tmp;

		classes.clear();

                tmp.clear();
                tmp.css_color(COLOR_MENU_BORDER_GREY);
                tmp.css_text_shadow("0.05em", "0.05em", "0.2em", "#888888");
                classes[text_grey] = tmp;

		tmp.clear();
		tmp.css_font_weight_bold();
		classes[text_bold] = tmp;

		tmp.clear();
		tmp.css_text_shadow("0.05em", "0.05em", "0.2em", "#888888");
                classes[text_shadow_dark] = tmp;

		tmp.clear();
		    ///<<< a faire
                classes[text_shadow_bright] = tmp;

		tmp.clear();
		tmp.css_color(RED);
		tmp.css_font_weight_bold();
                classes[text_color_red] = tmp;

		tmp.clear();
		tmp.css_color(GREEN);
		tmp.css_font_style_italic();
                classes[text_color_green] = tmp;

		tmp.clear();
		tmp.css_text_h_align(css::al_right);
		tmp.css_text_v_align(css::al_top);
		tmp.css_font_weight_bold();
		classes[text_top_right] = tmp;

		tmp.clear();
		tmp.css_font_weight_bold();
		classes[text_right] = tmp;

		tmp.clear();
		tmp.css_float(css::fl_left);
		tmp.css_margin_right("1em");
                classes[float_left] = tmp;

		tmp.clear();
		tmp.css_float(css::fl_right);
		tmp.css_margin_left("1em");
                classes[float_right] = tmp;

		tmp.clear();
		tmp.css_margin_top("0.2em");
		tmp.css_margin_bottom("0.2em");
                classes[spacing_vertical] = tmp;

		tmp.clear();
		tmp.css_margin_left("0.2em");
		tmp.css_margin_right("0.2em");
                classes[spacing_horizontal] = tmp;

		tmp.clear();
		tmp.css_max_width("100vw");
		tmp.css_width("100%", false);
                classes[width_100vw] = tmp;

		tmp.clear();
		tmp.css_max_width("80vw");
		tmp.css_width("100%", false); // not a typo: 100%
                classes[width_80vw] = tmp;

		tmp.clear();
		tmp.css_max_width("60vw");
		tmp.css_width("100%", false); // not a typo: 100%
                classes[width_60vw] = tmp;

		     //////

		css box_off, box_on, box_void, box_grey;

                tmp.clear();
                tmp.css_box_sizing(css::bx_border);
                tmp.css_width("100%", true);
                c_width_full.set_value(tmp);

                tmp.clear();
                tmp.css_width("8em", false, true);
                c_width_8em.set_value(tmp);

                    // Common aspects
                box_off.css_border_style(css::bd_all, css::bd_solid, true);
                box_off.css_border_width(css::bd_all, css::bd_medium, true);
                box_off.css_padding("0.5em", true);
                box_off.css_text_h_align(css::al_center, true);

                    // copy common aspects to box_off and box_void
                box_on.css_inherit_from(box_off);
                box_void.css_inherit_from(box_off);
                box_grey.css_inherit_from(box_off);
                box_void.css_border_style(css::bd_all, css::bd_none);

                    // box_off COLORS
                tmp.clear();
                tmp.css_color(COLOR_MENU_FRONT_OFF, true);
                tmp.css_background_color(COLOR_MENU_BACK_OFF, true);
                tmp.css_font_weight_bold(true);
                tmp.css_font_style_italic(true);
                tmp.css_text_decoration(css::dc_none, true);
                box_off.css_inherit_from(tmp);
                c_url_normal.set_selector(css_class::link, tmp);
                c_url_normal.set_selector(css_class::visited, tmp);
                box_off.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF, true);

                    // Link Hover and Active in box_off
                tmp.css_color(COLOR_MENU_FRONT_HOVER_OFF, true);
                tmp.css_text_decoration(css::dc_underline, true);
                c_url_normal.set_selector(css_class::hover, tmp);
                tmp.css_color(COLOR_MENU_FRONT_ACTIVE_OFF, true);
                c_url_normal.set_selector(css_class::active, tmp);

                    // box_on COLORS
                tmp.css_color(COLOR_MENU_FRONT_ON, true);
                tmp.css_background_color(COLOR_MENU_BACK_ON, true);
                tmp.css_font_weight_bold(true);
                tmp.css_font_style_normal(true);
                tmp.css_text_decoration(css::dc_none, true);
                box_on.css_inherit_from(tmp);
                c_url_selected.set_selector(css_class::link, tmp);
                c_url_selected.set_selector(css_class::visited, tmp);
                box_on.css_border_color(css::bd_all, COLOR_MENU_BORDER_ON, true);

                    // Link Hover and Active in box_on
                tmp.css_color(COLOR_MENU_FRONT_HOVER_ON, true);
                tmp.css_text_decoration(css::dc_underline, true);
                c_url_selected.set_selector(css_class::hover, tmp);
                tmp.css_color(COLOR_MENU_FRONT_ACTIVE_ON, true);
                c_url_selected.set_selector(css_class::active, tmp);

                    // box_grey COLORS
                tmp.css_color(COLOR_MENU_FRONT_GREY, true);
                tmp.css_background_color(COLOR_MENU_BACK_GREY, true);
                tmp.css_font_weight_bold(true);
                tmp.css_font_style_normal(true);
                tmp.css_text_decoration(css::dc_none, true);
                box_grey.css_inherit_from(tmp);
                c_url_grey.set_selector(css_class::link, tmp);
                c_url_grey.set_selector(css_class::visited, tmp);
                box_grey.css_border_color(css::bd_all, COLOR_MENU_BORDER_GREY, true);

                    // Link Hover and Active in box_grey

                tmp.css_color(COLOR_MENU_FRONT_HOVER_GREY, true);
                tmp.css_text_decoration(css::dc_underline, true);
                c_url_grey.set_selector(css_class::hover, tmp);
                tmp.css_color(COLOR_MENU_FRONT_ACTIVE_GREY, true);
                c_url_grey.set_selector(css_class::active, tmp);

                    //

                c_btn_off.set_value(box_off);
                c_btn_on.set_value(box_on);
                c_btn_void.set_value(box_void);
                c_btn_grey.set_value(box_grey);

                    //

                tmp.clear();
                tmp.css_font_weight_bold();
                c_bold_text.set_value(tmp);

                    //
                tmp.clear();
                tmp.css_border_width(css::bd_all, css::bd_thick, false);
                tmp.css_border_color(css::bd_all, RED);
                c_red_border.set_value(tmp);

                    //
                tmp.clear();
                tmp.css_margin_left("4em");
                c_indent.set_value(tmp);

                initialized = true;
            }
        }
        catch(...)
        {
            init_lock.unlock();
            throw;
        }
        init_lock.unlock();
    }

    void update_library(css_library & csslib)
    {
        init();

        if(!csslib.class_exists(c_btn_on.get_name()))
        {
            csslib.add(c_btn_on);
            csslib.add(c_btn_off);
            csslib.add(c_btn_void);
            csslib.add(c_btn_grey);
            csslib.add(c_url_selected);
            csslib.add(c_url_normal);
            csslib.add(c_url_void);
            csslib.add(c_url_grey);
            csslib.add(c_width_full);
            csslib.add(c_width_8em);
            csslib.add(c_bold_text);
            csslib.add(c_red_border);
            csslib.add(c_indent);


	    for(map<string, css>::iterator it = classes.begin();
		it != classes.end();
		++it)
		csslib.add(it->first, it->second);
        }
    }

    const css & get_css_class(const std::string & name)
    {
	map<string, css>::iterator it = classes.find(name);

	if(it == classes.end())
	    throw exception_range(libdar::tools_printf("Unknown css class name %s in webdar_css_style module", name.c_str()));

	return it->second;
    }

}  // end of namespace
