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
		classes[text_grey] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_font_weight_bold();
		tmp_class.set_value(tmp);
		tmp_class.change_name(text_bold);
		classes[text_bold] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_text_shadow("0.05em", "0.05em", "0.2em", "#888888");
		tmp_class.set_value(tmp);
                tmp_class.change_name(text_shadow_dark);
		classes[text_shadow_dark] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		    ///<<< a faire
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
                tmp_class.change_name(text_shadow_bright);
		classes[text_shadow_bright] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_color(BLACK);
		tmp_class.set_value(tmp);
		tmp_class.change_name(text_color_normal);
		classes[text_color_normal] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_color(RED);
		tmp.css_font_weight_bold();
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
                tmp_class.change_name(text_color_red);
		classes[text_color_red] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_color(GREEN);
		tmp.css_font_style_italic();
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
                tmp_class.change_name(text_color_green);
		classes[text_color_green] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_text_h_align(css::al_right);
		tmp.css_text_v_align(css::al_top);
		tmp.css_font_weight_bold();
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
		tmp_class.change_name(text_top_right);
		classes[text_top_right] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_font_weight_bold();
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
		tmp_class.change_name(text_right);
		classes[text_right] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_float(css::fl_left);
		tmp.css_margin_right("1em");
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
                tmp_class.change_name(float_left);
		classes[float_left] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_float(css::fl_right);
		tmp.css_margin_left("1em");
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
                tmp_class.change_name(float_right);
		classes[float_right] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_float_clear(css::fc_both);
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
		tmp_class.change_name(float_flush);
		classes[float_flush] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_margin_top("0.2em");
		tmp.css_margin_bottom("0.2em");
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
                tmp_class.change_name(spacing_vertical);
		classes[spacing_vertical] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_margin_left("0.2em");
		tmp.css_margin_right("0.2em");
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
                tmp_class.change_name(spacing_horizontal);
		classes[spacing_horizontal] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_max_width("100vw");
		tmp.css_width("100%", false);
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
                tmp_class.change_name(width_100vw);
		classes[width_100vw] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_max_width("100vw");
		tmp.css_width("calc(100% - 3em)", false);
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
                tmp_class.change_name(width_100vw_3em);
		classes[width_100vw_3em] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_max_width("100vw");
		tmp.css_width("calc(100% - 8em)", false);
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
                tmp_class.change_name(width_100vw_8em);
		classes[width_100vw_8em] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_max_width("100vw");
		tmp.css_width("calc(100% - 5em)", false);
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
                tmp_class.change_name(width_100vw_5em);
		classes[width_100vw_5em] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_max_width("80vw");
		tmp.css_width("80%", false);
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
                tmp_class.change_name(width_80vw);
		classes[width_80vw] = tmp_class;

		tmp.clear();
		tmp_class.clear();
		tmp.css_max_width("60vw");
		tmp.css_width("60%", false);
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
                tmp_class.change_name(width_60vw);
		classes[width_60vw] = tmp_class;

		tmp.clear();
		tmp_class.clear();
                tmp.css_border_width(css::bd_all, css::bd_thick);
                tmp.css_border_color(css::bd_all, RED);
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
                tmp_class.change_name(red_border);
		classes[red_border] = tmp_class;

		tmp.clear();
		tmp_class.clear();
                tmp.css_margin_left("4em");
		tmp_class.set_value(tmp);
		tmp_class.set_value(tmp);
		tmp_class.change_name(indent);
		classes[indent] = tmp_class;

		     // Button sizing

                tmp.clear();
		tmp_class.clear();
                tmp.css_box_sizing(css::bx_border);
		tmp.css_width("100%", true);
                tmp_class.set_value(tmp);
		tmp_class.change_name(width_full);
		classes[width_full] = tmp_class;


                tmp.clear();
		tmp_class.clear();
                tmp.css_box_sizing(css::bx_border);
                tmp.css_width("9em", false);
                tmp_class.set_value(tmp);
		tmp_class.change_name(width_8em);
		classes[width_8em] = tmp_class;

                    // Button CSS classes

		    // temporary variables used to setup c_btn_* static classes of this module
		css box_off, box_on, box_void, box_grey, box_small;

		    // void box

		tmp_class.clear();
		tmp.clear();
		tmp.css_padding("0.5em");
		tmp_class.set_value(tmp);
		tmp_class.change_name(btn_void);
		classes[btn_void] = tmp_class;

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

		tmp_class.clear();
		tmp_class.set_value(box_off);
		tmp_class.set_selector(css_class::active, box_off);
		tmp_class.set_selector(css_class::visited, box_off);
		tmp_class.set_selector(css_class::link, box_off);
		box_off.css_box_shadow("0.25em", "0.25em", "0.2em");
		tmp_class.set_selector(css_class::hover, box_off);
		tmp_class.change_name(btn_off);
		classes[btn_off] = tmp_class;

		    // small box

		    // box_small is set a few lines above from box_off
		tmp_class.clear();

		box_small.css_width("2em", false);
		box_small.css_padding("0");
		box_small.css_min_width("2em");
		box_small.css_min_height("1.5em");
		tmp_class.set_value(box_small);
		tmp_class.set_selector(css_class::active, box_small);
		tmp_class.set_selector(css_class::visited, box_small);
		tmp_class.set_selector(css_class::link, box_small);
		box_small.css_box_shadow("0.25em", "0.25em", "0.2em");
		tmp_class.set_selector(css_class::hover, box_small);
		tmp_class.change_name(btn_small);
		classes[btn_small] = tmp_class;

		    // active box

		box_on = tmp;
		tmp_class.clear();

		box_on.css_box_shadow("0.1em", "0.1em", "0.1em");
		box_on.css_background_color(COLOR_MENU_BACK_ON);
		box_on.css_border_color(css::bd_all, COLOR_MENU_BACK_ON);
		tmp_class.set_value(box_on);
		tmp_class.set_selector(css_class::active, box_on);
		tmp_class.set_selector(css_class::hover, box_on);
		tmp_class.set_selector(css_class::visited, box_on);
		tmp_class.set_selector(css_class::link, box_on);
		tmp_class.change_name(btn_on);
		classes[btn_on] = tmp_class;

		    // grey box

		box_grey = tmp;
		tmp_class.clear();

		box_grey.css_background_color(COLOR_MENU_BACK_GREY);
		box_grey.css_border_color(css::bd_all, COLOR_MENU_BACK_GREY);
                tmp_class.set_value(box_grey);
		tmp_class.set_value(box_grey);
		tmp_class.set_selector(css_class::active, box_grey);
		tmp_class.set_selector(css_class::visited, box_grey);
		tmp_class.set_selector(css_class::link, box_grey);
		box_grey.css_box_shadow("0.25em", "0.25em", "0.2em");
		tmp_class.set_selector(css_class::hover, box_grey);
		tmp_class.change_name(btn_grey);
		classes[btn_grey] = tmp_class;


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

		tmp_class.clear(); // empty class
		tmp_class.change_name(url_void);
		classes[url_void] = tmp_class;

		    // Normal URL

		box_off = tmp;

		box_off.css_color(COLOR_MENU_FRONT_OFF);
		tmp_class.clear();
		tmp_class.set_value(box_off);
                tmp_class.set_selector(css_class::link, box_off);
                tmp_class.set_selector(css_class::visited, box_off);

		box_off.css_color(COLOR_MENU_FRONT_ACTIVE_OFF);
                tmp_class.set_selector(css_class::active, box_off);

		box_off.css_color(COLOR_MENU_FRONT_HOVER_OFF);
		box_off.css_font_style_normal();
		box_off.css_text_decoration(css::dc_underline);
		box_off.css_text_shadow("0", "0", "2em", "yellow");
                tmp_class.set_selector(css_class::hover, box_off);
		tmp_class.change_name(url_normal);
		classes[url_normal] = tmp_class;


		    // Active URL

		box_on = tmp;
		tmp_class.clear();

		box_on.css_color(COLOR_MENU_FRONT_ON);
		box_on.css_font_style_normal();
		tmp_class.set_selector(css_class::link, box_on);
                tmp_class.set_selector(css_class::visited, box_on);

                box_on.css_color(COLOR_MENU_FRONT_ACTIVE_ON);
                tmp_class.set_selector(css_class::active, box_on);

                box_on.css_color(COLOR_MENU_FRONT_HOVER_ON);
                box_on.css_text_decoration(css::dc_underline);
		box_on.css_text_shadow("0", "0", "2em", "yellow");
                tmp_class.set_selector(css_class::hover, box_on);
		tmp_class.change_name(url_selected);
		classes[url_selected] = tmp_class;

		    // Grey URL

		box_grey = tmp;
		tmp_class.clear();

                box_grey.css_color(COLOR_MENU_FRONT_GREY);
                tmp_class.set_selector(css_class::link, box_grey);
                tmp_class.set_selector(css_class::visited, box_grey);

                box_grey.css_color(COLOR_MENU_FRONT_ACTIVE_GREY);
                tmp_class.set_selector(css_class::active, box_grey);

		box_grey.css_color(COLOR_MENU_FRONT_HOVER_GREY);
		box_grey.css_font_style_normal();
		box_grey.css_text_decoration(css::dc_underline);
		box_grey.css_text_shadow("0", "0", "2em", "yellow");
                tmp_class.set_selector(css_class::hover, box_grey);
		tmp_class.change_name(url_grey);
		classes[url_grey] = tmp_class;

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

	if(!csslib.class_exists(btn_on))
	{
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
