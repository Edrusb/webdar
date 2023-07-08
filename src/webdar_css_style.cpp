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
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "tokens.hpp"
#include "exceptions.hpp"

    //
#include "webdar_css_style.hpp"

using namespace std;

namespace webdar_css_style
{


    static bool initialized = false;

    static libthreadar::mutex init_lock;
    static css_class btn_off("wcs_btn_off");           ///< used to assign CSS attributes: unselected item
    static css_class btn_on("wcs_btn_on");             ///< used to assign CSS attributes: selected item
    static css_class btn_void("wcs_btn_void");         ///< used to assign CSS attributes: separators
    static css_class url_selected("wcs_url_selected"); ///< links when button selected
    static css_class url_normal("wcs_url_normal");     ///< links when button no selected
    static css_class url_void("wcs_url_void");         ///< links when button is a void

    static void init()
    {
        if(initialized)
            return; // bypassing mutex as this field once true stays true forever

        init_lock.lock();
        try
        {
            if(!initialized)
            {
                css tmp_set;
                css box_off, box_on, box_void;

                    // Common aspects
                box_off.css_border_style(css::bd_all, css::bd_solid, true);
                box_off.css_border_width(css::bd_all, css::bd_medium, true);
                box_off.css_width("8em", true, true);
                box_off.css_padding("0.5em", true);
                box_off.css_margin("0.2em", true);
                box_off.css_text_align(css::al_center, true);

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
                url_normal.set_selector(css_class::link, tmp_set);
                url_normal.set_selector(css_class::visited, tmp_set);
                box_off.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF, true);

                    // Link Hover and Active in box_off
                tmp_set.css_color(COLOR_MENU_FRONT_HOVER_OFF, true);
                tmp_set.css_text_decoration(css::dc_underline, true);
                url_normal.set_selector(css_class::hover, tmp_set);
                tmp_set.css_color(COLOR_MENU_FRONT_ACTIVE_OFF, true);
                url_normal.set_selector(css_class::active, tmp_set);

                    // box_on and tmp_select COLORS
                tmp_set.css_color(COLOR_MENU_FRONT_ON, true);
                tmp_set.css_background_color(COLOR_MENU_BACK_ON, true);
                tmp_set.css_font_weight_bold(true);
                tmp_set.css_font_style_normal(true);
                tmp_set.css_text_decoration(css::dc_none, true);
                box_on.css_inherit_from(tmp_set);
                url_selected.set_selector(css_class::link, tmp_set);
                url_selected.set_selector(css_class::visited, tmp_set);
                box_on.css_border_color(css::bd_all, COLOR_MENU_BORDER_ON, true);

                    // Link Hover and Active in box_on
                tmp_set.css_color(COLOR_MENU_FRONT_HOVER_ON, true);
                tmp_set.css_text_decoration(css::dc_underline, true);
                url_selected.set_selector(css_class::hover, tmp_set);
                tmp_set.css_color(COLOR_MENU_FRONT_ACTIVE_ON, true);
                url_selected.set_selector(css_class::active, tmp_set);

                btn_off.set_value(box_off);
                btn_on.set_value(box_on);
                btn_void.set_value(box_void);

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
	if(!csslib.class_exists(btn_on.get_name()))
	{
	    csslib.add(btn_on);
	    csslib.add(btn_off);
	    csslib.add(btn_void);
	    csslib.add(url_selected);
	    csslib.add(url_normal);
	    csslib.add(url_void);
	}
	else
	{
	    if(!csslib.class_exists(btn_off.get_name()))
		throw WEBDAR_BUG;
	    if(!csslib.class_exists(btn_void.get_name()))
		throw WEBDAR_BUG;
	    if(!csslib.class_exists(url_selected.get_name()))
		throw WEBDAR_BUG;
	    if(!csslib.class_exists(url_normal.get_name()))
		throw WEBDAR_BUG;
	    if(!csslib.class_exists(url_void.get_name()))
		throw WEBDAR_BUG;
	}
    }

    void normal_button(html_button & obj)
    {
        if(!initialized)
            init();

        obj.clear_css_classes();
        obj.add_css_class(btn_off.get_name());
        obj.url_clear_css_classes();
        obj.url_add_css_class(url_normal.get_name());
    }

    void active_button(html_button & obj)
    {
        if(!initialized)
            init();

        obj.clear_css_classes();
        obj.add_css_class(btn_on.get_name());
        obj.url_clear_css_classes();
        obj.url_add_css_class(url_selected.get_name());
    }

    void void_button(html_button & obj)
    {
        if(!initialized)
            init();

        obj.clear_css_classes();
        obj.add_css_class(btn_void.get_name());
        obj.url_clear_css_classes();
        obj.url_add_css_class(url_void.get_name());
    }

}  // end of namespace
