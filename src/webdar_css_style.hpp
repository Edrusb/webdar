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

#ifndef WEBDAR_CSS_STYLE_HPP
#define WEBDAR_CSS_STYLE_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "css_library.hpp"

    /// the webdar_css_style namespace defines a set of global css objets and some routines to use them

namespace webdar_css_style
{
        /// update a css_library with all css definitions of this webdar_css_style module
    extern void update_library(css_library & ref);

    constexpr const char* text_grey = "wcs_tg";
    constexpr const char* text_bold = "wcs_tb";
    constexpr const char* text_shadow_dark = "wcs_tsd";
    constexpr const char* text_shadow_bright = "wcs_tsb";
    constexpr const char* text_color_normal = "wcs_cn";
    constexpr const char* text_color_red = "wcs_cr";
    constexpr const char* text_color_green = "wcs_cg";
    constexpr const char* text_top_right = "wcs_ttr";
    constexpr const char* text_right = "wcs_tr";
    constexpr const char* float_left = "wcs_fll";
    constexpr const char* float_right = "wcs_flr";
    constexpr const char* float_flush = "wcs_flf";
    constexpr const char* spacing_vertical = "wcs_spcv";
    constexpr const char* spacing_horizontal = "wcs_spch";
    constexpr const char* width_100vw = "wcs_w100";
    constexpr const char* width_100vw_3em = "wcs_w100_3em"; // 100% - 3em
    constexpr const char* width_80vw = "wcs_w80";
    constexpr const char* width_60vw = "wcs_w60";
    constexpr const char* red_border = "wcs_red_border";
    constexpr const char* indent = "wcs_indent";


	/// obtain the css_class object corresponding to the given name

	/// \param[in] name must correspond to an existing class name defined above (text_grey, text_bold,...)
	/// \exception exception throw if the povided name does not exist
    const css_class & get_css_class(const std::string & name);

	// class used by the *_button templates below (should not be used directly, it is publicly
        // available because template body need to be stored in header file")
    constexpr const char* width_8em = "wcs_w8em";
    constexpr const char* width_full = "fullwidth";
    constexpr const char* btn_off = "wcs_btn_off";
    constexpr const char* btn_small = "wcs_btn_small";
    constexpr const char* btn_on = "wcs_btn_on";
    constexpr const char* btn_void = "wcs_btn_void";
    constexpr const char* btn_grey = "wcs_btn_grey";
    constexpr const char* url_selected = "wcs_url_selected";
    constexpr const char* url_normal = "wcs_url_normal";
    constexpr const char* url_void = "wcs_url_void";
    constexpr const char* url_grey = "wcs_url_grey";


        /// apply to the given button the css_classe names defined by update_library to get a normal button style
    template <class T> void normal_button(T & obj, bool fullwidth = false)
    {
        obj.clear_css_classes();
        obj.add_css_class(btn_off);
        obj.url_clear_css_classes();
        obj.url_add_css_class(url_normal);
        if(!fullwidth)
            obj.add_css_class(width_8em);
    }

    template <class T> void small_button(T & obj)
    {
        obj.clear_css_classes();
        obj.add_css_class(btn_small);
        obj.url_clear_css_classes();
        obj.url_add_css_class(url_normal);
    }


        /// apply to the given button the css_classe names defined by update_library to get an active button style
    template <class T> void active_button(T & obj, bool fullwidth = false)
    {
        obj.clear_css_classes();
        obj.add_css_class(btn_on);
        obj.url_clear_css_classes();
        obj.url_add_css_class(url_selected);
        if(!fullwidth)
            obj.add_css_class(width_8em);
    }

        /// apply to the given button the css_classe names defined by update_library to get an void/invisible button style
    template <class T> void void_button(T & obj, bool fullwidth = false)
    {
        obj.clear_css_classes();
        obj.add_css_class(btn_void);
        obj.url_clear_css_classes();
        obj.url_add_css_class(url_void);
        if(!fullwidth)
            obj.add_css_class(width_8em);
    }

    template <class T> void grey_button(T & obj, bool fullwidth = false)
    {
        obj.clear_css_classes();
        obj.add_css_class(btn_grey);
        obj.url_clear_css_classes();
        obj.url_add_css_class(url_grey);
        if(!fullwidth)
            obj.add_css_class(width_8em);
    }


} // namespace end

#endif
