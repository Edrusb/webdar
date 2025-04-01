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

    static map<string, css_class> classes;

    static css_class c_width_full(width_full);     ///< full screen large buttons (html_derouleur for exmple)
    static css_class c_width_8em(width_8em);       ///< normal width buttons (html_button, html_double_button, html_tabs,...)

    static css_class c_btn_off(btn_off);           ///< used to assign CSS attributes: unselected item
    static css_class c_btn_small(btn_small);         ///< used to assign CSS attributes: small unselected box
    static css_class c_btn_on(btn_on);             ///< used to assign CSS attributes: selected item
    static css_class c_btn_void(btn_void);         ///< used to assign CSS attributes: separators
    static css_class c_btn_grey(btn_grey);         ///< used to assign CSS attributes: disabled or greyed
    static css_class c_url_selected(url_selected); ///< links when button selected
    static css_class c_url_normal(url_normal);     ///< links when button no selected
    static css_class c_url_void(url_void);         ///< links when button is a void
    static css_class c_url_grey(url_grey);         ///< links when button is disabled or greyed

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
		css_class tmp_class;

		classes.clear();

                tmp.clear();
		tmp_class.clear();
                tmp.css_color(COLOR_MENU_BORDER_GREY);
                tmp.css_text_shadow("0.05em", "0.05em", "0.2em", "#888888");
		tmp_class.set_value(tmp);
                tmp_class.change_name(text_grey);
		tmp_class.set_value(tmp);
		classes[text_grey] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_font_weight_bold();
		tmp_class.set_value(tmp);
		tmp_class.change_name(text_bold);
		tmp_class.set_value(tmp);
		classes[text_bold] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_text_shadow("0.05em", "0.05em", "0.2em", "#888888");
		tmp_class.set_value(tmp);
                tmp_class.change_name(text_shadow_dark);
		tmp_class.set_value(tmp);
		classes[text_shadow_dark] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		    ///<<< a faire
		tmp_class.set_value(tmp);
                tmp_class.change_name(text_shadow_bright);
		tmp_class.set_value(tmp);
		classes[text_shadow_bright] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_color(RED);
		tmp.css_font_weight_bold();
		tmp_class.set_value(tmp);
                tmp_class.change_name(text_color_red);
		tmp_class.set_value(tmp);
		classes[text_color_red] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_color(GREEN);
		tmp.css_font_style_italic();
		tmp_class.set_value(tmp);
                tmp_class.change_name(text_color_green);
		tmp_class.set_value(tmp);
		classes[text_color_green] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_text_h_align(css::al_right);
		tmp.css_text_v_align(css::al_top);
		tmp.css_font_weight_bold();
		tmp_class.set_value(tmp);
		tmp_class.change_name(text_top_right);
		tmp_class.set_value(tmp);
		classes[text_top_right] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_font_weight_bold();
		tmp_class.set_value(tmp);
		tmp_class.change_name(text_right);
		tmp_class.set_value(tmp);
		classes[text_right] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_float(css::fl_left);
		tmp.css_margin_right("1em");
		tmp_class.set_value(tmp);
                tmp_class.change_name(float_left);
		tmp_class.set_value(tmp);
		classes[float_left] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_float(css::fl_right);
		tmp.css_margin_left("1em");
		tmp_class.set_value(tmp);
                tmp_class.change_name(float_right);
		tmp_class.set_value(tmp);
		classes[float_right] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_float_clear(css::fc_both);
		tmp_class.set_value(tmp);
		tmp_class.change_name(float_flush);
		tmp_class.set_value(tmp);
		classes[float_flush] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_margin_top("0.2em");
		tmp.css_margin_bottom("0.2em");
		tmp_class.set_value(tmp);
                tmp_class.change_name(spacing_vertical);
		tmp_class.set_value(tmp);
		tmp_class.set_selector(css_class::hover, tmp);
		classes[spacing_vertical] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_margin_left("0.2em");
		tmp.css_margin_right("0.2em");
		tmp_class.set_value(tmp);
                tmp_class.change_name(spacing_horizontal);
		tmp_class.set_value(tmp);
		tmp_class.set_selector(css_class::hover, tmp);
		classes[spacing_horizontal] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_max_width("100vw");
		tmp.css_width("100%", false);
		tmp_class.set_value(tmp);
                tmp_class.change_name(width_100vw);
		tmp_class.set_value(tmp);
		classes[width_100vw] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_max_width("100vw");
		tmp.css_width("calc(100% - 3em)", false);
		tmp_class.set_value(tmp);
                tmp_class.change_name(width_100vw_3em);
		tmp_class.set_value(tmp);
		classes[width_100vw_3em] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_max_width("80vw");
		tmp.css_width("80%", false);
		tmp_class.set_value(tmp);
                tmp_class.change_name(width_80vw);
		tmp_class.set_value(tmp);
		classes[width_80vw] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_max_width("60vw");
		tmp.css_width("60%", false);
		tmp_class.set_value(tmp);
                tmp_class.change_name(width_60vw);
		tmp_class.set_value(tmp);
		classes[width_60vw] = tmp_class;

		tmp.clear();
		tmp_class.clear();
                tmp.css_border_width(css::bd_all, css::bd_thick);
                tmp.css_border_color(css::bd_all, RED);
		tmp_class.set_value(tmp);
                tmp_class.change_name(red_border);
		tmp_class.set_value(tmp);
		classes[red_border] = tmp_class;

		tmp.clear();
		tmp_class.clear();
                tmp.css_margin_left("4em");
		tmp_class.set_value(tmp);
		tmp_class.change_name(indent);
		tmp_class.set_value(tmp);
		classes[indent] = tmp_class;

		     // Button sizing

                tmp.clear();
                tmp.css_box_sizing(css::bx_border);
		tmp.css_width("100%", true);
                c_width_full.set_value(tmp);

                tmp.clear();
                tmp.css_box_sizing(css::bx_border);
                tmp.css_width("9em", false);
                c_width_8em.set_value(tmp);

                    // Button CSS classes

		    // temporary variables used to setup c_btn_* static classes of this module
		css box_off, box_on, box_void, box_grey, box_small;

		    // void box

		tmp.clear();
		tmp.css_padding("0.5em");
		c_btn_void.set_value(tmp);

		    // common aspect of all other boxes

		tmp.clear();
		tmp.css_box_sizing(css::bx_border);
                tmp.css_text_h_align(css::al_center);
		tmp.css_corner_radius("0.5em","0", "0.5em", "0");
		tmp.css_padding("0.5em");
		tmp.css_border_style(css::bd_all, css::bd_outset);
		tmp.css_border_width(css::bd_all, css::bd_thin);

		    // normal box

		box_off = tmp;
		box_off.css_background_color(COLOR_DAR_GREYBLUE);
		box_off.css_border_color(css::bd_all, COLOR_DAR_GREYBLUE);

		box_small = box_off;

		c_btn_off.set_value(box_off);
		c_btn_off.set_selector(css_class::active, box_off);
		c_btn_off.set_selector(css_class::visited, box_off);
		c_btn_off.set_selector(css_class::link, box_off);
		box_off.css_box_shadow("0.25em", "0.25em", "0.2em");
		c_btn_off.set_selector(css_class::hover, box_off);


		    // small box

		    // box_small is set a few lines above from box_off
		box_small.css_width("2em", true);
		box_small.css_padding("0");
		box_small.css_margin("0");
		c_btn_small.set_value(box_small);
		c_btn_small.set_selector(css_class::active, box_small);
		c_btn_small.set_selector(css_class::visited, box_small);
		c_btn_small.set_selector(css_class::link, box_small);
		box_small.css_box_shadow("0.25em", "0.25em", "0.2em");
		c_btn_small.set_selector(css_class::hover, box_small);

		    // active box

		box_on = tmp;

		box_on.css_box_shadow("0.1em", "0.1em", "0.1em");
		box_on.css_background_color(COLOR_MENU_BACK_ON);
		box_on.css_border_color(css::bd_all, COLOR_MENU_BACK_ON);
		c_btn_on.set_value(box_on);
		c_btn_on.set_selector(css_class::active, box_on);
		c_btn_on.set_selector(css_class::hover, box_on);
		c_btn_on.set_selector(css_class::visited, box_on);
		c_btn_on.set_selector(css_class::link, box_on);

		    // grey box

		box_grey = tmp;
		box_grey.css_background_color(COLOR_MENU_BACK_GREY);
		box_grey.css_border_color(css::bd_all, COLOR_MENU_BACK_GREY);
                c_btn_grey.set_value(box_grey);
		c_btn_grey.set_value(box_grey);
		c_btn_grey.set_selector(css_class::active, box_grey);
		c_btn_grey.set_selector(css_class::visited, box_grey);
		c_btn_grey.set_selector(css_class::link, box_grey);
		box_grey.css_box_shadow("0.25em", "0.25em", "0.2em");
		c_btn_grey.set_selector(css_class::hover, box_grey);


		    // URL common
		    // we recycle the box_off, box_on, box_grey local variable to setup the button URL:

		    // common property to all URLs

		map<string, css_class>::iterator it = classes.find(text_shadow_dark);
		if(it == classes.end())
		    throw WEBDAR_BUG;

		tmp.clear();
		tmp.update_from(it->second.get_value());
		tmp.css_font_weight_bold();
		tmp.css_font_style_italic();
		tmp.css_text_decoration(css::dc_none);

		    // void URL

		c_url_void.clear_value(); // empty class

		    // Normal URL

		box_off = tmp;

		box_off.css_color(COLOR_MENU_FRONT_OFF);
		c_url_normal.set_value(box_off);
                c_url_normal.set_selector(css_class::link, box_off);
                c_url_normal.set_selector(css_class::visited, box_off);

		box_off.css_color(COLOR_MENU_FRONT_ACTIVE_OFF);
                c_url_normal.set_selector(css_class::active, box_off);

		box_off.css_color(COLOR_MENU_FRONT_HOVER_OFF);
		box_off.css_font_style_normal();
		box_off.css_text_decoration(css::dc_underline);
		box_off.css_text_shadow("0", "0", "2em", "yellow");
                c_url_normal.set_selector(css_class::hover, box_off);


		    // Active URL

		box_on = tmp;

		box_on.css_color(COLOR_MENU_FRONT_ON);
		box_on.css_font_style_normal();
		c_url_selected.set_selector(css_class::link, box_on);
                c_url_selected.set_selector(css_class::visited, box_on);

                box_on.css_color(COLOR_MENU_FRONT_ACTIVE_ON);
                c_url_selected.set_selector(css_class::active, box_on);

                box_on.css_color(COLOR_MENU_FRONT_HOVER_ON);
                box_on.css_text_decoration(css::dc_underline);
		box_on.css_text_shadow("0", "0", "2em", "yellow");
                c_url_selected.set_selector(css_class::hover, box_on);


		    // Grey URL

		box_grey = tmp;

                box_grey.css_color(COLOR_MENU_FRONT_GREY);
                c_url_grey.set_selector(css_class::link, box_grey);
                c_url_grey.set_selector(css_class::visited, box_grey);

                box_grey.css_color(COLOR_MENU_FRONT_ACTIVE_GREY);
                c_url_grey.set_selector(css_class::active, box_grey);

		box_grey.css_color(COLOR_MENU_FRONT_HOVER_GREY);
		box_grey.css_font_style_normal();
		box_grey.css_text_decoration(css::dc_underline);
		box_grey.css_text_shadow("0", "0", "2em", "yellow");
                c_url_grey.set_selector(css_class::hover, box_grey);

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
	    csslib.add(c_btn_small);
            csslib.add(c_btn_off);
            csslib.add(c_btn_void);
            csslib.add(c_btn_grey);
            csslib.add(c_url_selected);
            csslib.add(c_url_normal);
            csslib.add(c_url_void);
            csslib.add(c_url_grey);
            csslib.add(c_width_full);
            csslib.add(c_width_8em);

	    for(map<string, css_class>::iterator it = classes.begin();
		it != classes.end();
		++it)
		csslib.add(it->second);
        }
    }

    const css_class & get_css_class(const std::string & name)
    {
	map<string, css_class>::iterator it = classes.find(name);

	if(it == classes.end())
	    throw exception_range(libdar::tools_printf("Unknown css class name %s in webdar_css_style module", name.c_str()));

	return it->second;
    }

}  // end of namespace
