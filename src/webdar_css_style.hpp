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
	/// update a css_library with webdar global css_class definitions
    extern void update_library(css_library & ref);


    constexpr const char* wcs_cadre_rouge = "wcs_cadre_rouge";
    constexpr const char* wcs_grey_text = "wcs_grey_text";


	/// CSS used for button width in normal_button()/active_button()/void_button() routines
    constexpr const char* wcs_full_width = "fullwidth";

	/// CSS used for button width in normal_button()/active_button()/void_button() routines
    constexpr const char* wcs_8em_width = "width8em";

	// class used by the *_button templates below (should not be used directly, it is publicly
	// available because template body need to be stored in header file")
    constexpr const char* wcs_btn_off = "wcs_btn_off";
    constexpr const char* wcs_btn_on = "wcs_btn_on";
    constexpr const char* wcs_btn_void = "wcs_btn_void";
    constexpr const char* wcs_btn_grey = "wcs_btn_grey";
    constexpr const char* wcs_url_selected = "wcs_url_selected";
    constexpr const char* wcs_url_normal = "wcs_url_normal";
    constexpr const char* wcs_url_void = "wcs_url_void";
    constexpr const char* wcs_url_grey = "wcs_url_grey";
    constexpr const char* wcs_bold_text = "wcs_bold_text";
    constexpr const char* wcs_red_border = "wcs_red_border";
    constexpr const char* wcs_indent = "wcs_indent";


	/// apply to the given button the css_classe names defined by update_library to get a normal button style
    template <class T> void normal_button(T & obj, bool fullwidth = false)
    {
        obj.clear_css_classes();
        obj.add_css_class(wcs_btn_off);
        obj.url_clear_css_classes();
        obj.url_add_css_class(wcs_url_normal);
	if(!fullwidth)
	    obj.add_css_class(wcs_8em_width);
    }

    	/// apply to the given button the css_classe names defined by update_library to get an active button style
    template <class T> void active_button(T & obj, bool fullwidth = false)
    {
	obj.clear_css_classes();
        obj.add_css_class(wcs_btn_on);
        obj.url_clear_css_classes();
        obj.url_add_css_class(wcs_url_selected);
	if(!fullwidth)
	    obj.add_css_class(wcs_8em_width);
    }

    	/// apply to the given button the css_classe names defined by update_library to get an void/invisible button style
    template <class T> void void_button(T & obj, bool fullwidth = false)
    {
	obj.clear_css_classes();
        obj.add_css_class(wcs_btn_void);
        obj.url_clear_css_classes();
        obj.url_add_css_class(wcs_url_void);
	if(!fullwidth)
	    obj.add_css_class(wcs_8em_width);
    }

    template <class T> void grey_button(T & obj, bool fullwidth = false)
    {
	obj.clear_css_classes();
        obj.add_css_class(wcs_btn_grey);
        obj.url_clear_css_classes();
        obj.url_add_css_class(wcs_url_grey);
	if(!fullwidth)
	    obj.add_css_class(wcs_8em_width);
    }


} // namespace end

#endif
