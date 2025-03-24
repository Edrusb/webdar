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

    static css_class cadre_rouge(wcs_cadre_rouge);
    static css_class text_grey(wcs_text_grey);
    static css_class text_bold(wcs_text_bold);
    static css_class float_left(wcs_float_left);
    static css_class float_right(wcs_float_right);

    static css_class full_width(wcs_full_width);     ///< full screen large buttons (html_derouleur for exmple)
    static css_class width_8em(wcs_8em_width);       ///< normal width buttons (html_button, html_double_button, html_tabs,...)

    static css_class btn_off(wcs_btn_off);           ///< used to assign CSS attributes: unselected item
    static css_class btn_on(wcs_btn_on);             ///< used to assign CSS attributes: selected item
    static css_class btn_void(wcs_btn_void);         ///< used to assign CSS attributes: separators
    static css_class btn_grey(wcs_btn_grey);         ///< used to assign CSS attributes: disabled or greyed
    static css_class url_selected(wcs_url_selected); ///< links when button selected
    static css_class url_normal(wcs_url_normal);     ///< links when button no selected
    static css_class url_void(wcs_url_void);         ///< links when button is a void
    static css_class url_grey(wcs_url_grey);         ///< links when button is disabled or greyed
    static css_class bold_text(wcs_bold_text);       ///< bold text
    static css_class red_border(wcs_red_border);     ///< red border
    static css_class indent(wcs_indent);             ///< indentation (4em shift to the right)

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

		tmp.clear();
		    //
                cadre_rouge.set_value(tmp);

                tmp.clear();
                tmp.css_color(COLOR_MENU_BORDER_GREY);
                tmp.css_text_shadow("0.05em", "0.05em", "0.2em", "#888888");
                text_grey.set_value(tmp);

		tmp.clear();
		tmp.css_font_weight_bold();
                text_bold.set_value(tmp);

		tmp.clear();
		tmp.css_float(css::fl_left);
                float_left.set_value(tmp);

		tmp.clear();
		tmp.css_float(css::fl_right);
                float_right.set_value(tmp);


		     //////

		css box_off, box_on, box_void, box_grey;

                tmp.clear();
                tmp.css_box_sizing(css::bx_border);
                tmp.css_width("100%", true);
                full_width.set_value(tmp);

                tmp.clear();
                tmp.css_width("8em", false, true);
                width_8em.set_value(tmp);

                    // Common aspects
                box_off.css_border_style(css::bd_all, css::bd_solid, true);
                box_off.css_border_width(css::bd_all, css::bd_medium, true);
                box_off.css_padding("0.5em", true);
                box_off.css_margin("0.2em", true);
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
                url_normal.set_selector(css_class::link, tmp);
                url_normal.set_selector(css_class::visited, tmp);
                box_off.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF, true);

                    // Link Hover and Active in box_off
                tmp.css_color(COLOR_MENU_FRONT_HOVER_OFF, true);
                tmp.css_text_decoration(css::dc_underline, true);
                url_normal.set_selector(css_class::hover, tmp);
                tmp.css_color(COLOR_MENU_FRONT_ACTIVE_OFF, true);
                url_normal.set_selector(css_class::active, tmp);

                    // box_on COLORS
                tmp.css_color(COLOR_MENU_FRONT_ON, true);
                tmp.css_background_color(COLOR_MENU_BACK_ON, true);
                tmp.css_font_weight_bold(true);
                tmp.css_font_style_normal(true);
                tmp.css_text_decoration(css::dc_none, true);
                box_on.css_inherit_from(tmp);
                url_selected.set_selector(css_class::link, tmp);
                url_selected.set_selector(css_class::visited, tmp);
                box_on.css_border_color(css::bd_all, COLOR_MENU_BORDER_ON, true);

                    // Link Hover and Active in box_on
                tmp.css_color(COLOR_MENU_FRONT_HOVER_ON, true);
                tmp.css_text_decoration(css::dc_underline, true);
                url_selected.set_selector(css_class::hover, tmp);
                tmp.css_color(COLOR_MENU_FRONT_ACTIVE_ON, true);
                url_selected.set_selector(css_class::active, tmp);

                    // box_grey COLORS
                tmp.css_color(COLOR_MENU_FRONT_GREY, true);
                tmp.css_background_color(COLOR_MENU_BACK_GREY, true);
                tmp.css_font_weight_bold(true);
                tmp.css_font_style_normal(true);
                tmp.css_text_decoration(css::dc_none, true);
                box_grey.css_inherit_from(tmp);
                url_grey.set_selector(css_class::link, tmp);
                url_grey.set_selector(css_class::visited, tmp);
                box_grey.css_border_color(css::bd_all, COLOR_MENU_BORDER_GREY, true);

                    // Link Hover and Active in box_grey

                tmp.css_color(COLOR_MENU_FRONT_HOVER_GREY, true);
                tmp.css_text_decoration(css::dc_underline, true);
                url_grey.set_selector(css_class::hover, tmp);
                tmp.css_color(COLOR_MENU_FRONT_ACTIVE_GREY, true);
                url_grey.set_selector(css_class::active, tmp);

                    //

                btn_off.set_value(box_off);
                btn_on.set_value(box_on);
                btn_void.set_value(box_void);
                btn_grey.set_value(box_grey);

                    //

                tmp.clear();
                tmp.css_font_weight_bold();
                bold_text.set_value(tmp);

                    //
                tmp.clear();
                tmp.css_border_width(css::bd_all, css::bd_thick, false);
                tmp.css_border_color(css::bd_all, RED);
                red_border.set_value(tmp);

                    //
                tmp.clear();
                tmp.css_margin_left("4em");
                indent.set_value(tmp);

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

        if(!csslib.class_exists(btn_on.get_name()))
        {
            csslib.add(btn_on);
            csslib.add(btn_off);
            csslib.add(btn_void);
            csslib.add(btn_grey);
            csslib.add(url_selected);
            csslib.add(url_normal);
            csslib.add(url_void);
            csslib.add(url_grey);
            csslib.add(full_width);
            csslib.add(width_8em);
            csslib.add(bold_text);
            csslib.add(red_border);
            csslib.add(indent);

	    csslib.add(cadre_rouge);
	    csslib.add(text_grey);
	    csslib.add(text_bold);
	    csslib.add(float_left);
	    csslib.add(float_right);
        }
    }

}  // end of namespace
