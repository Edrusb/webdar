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

#ifndef WEBDAR_CSS_STYLE_HPP
#define WEBDAR_CSS_STYLE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "css_library.hpp"
#include "html_button.hpp"

namespace webdar_css_style
{
	/// update a css_library with css_class definitions used by the three following routines
    extern void update_library(css_library & ref);

	/// apply to the given button the css_classe names defined by update_library to get a normal button style
    extern void normal_button(html_button & obj);

    	/// apply to the given button the css_classe names defined by update_library to get an active button style
    extern void active_button(html_button & obj);

    	/// apply to the given button the css_classe names defined by update_library to get an void/invisible button style
    extern void void_button(html_button & obj);


} // namespace end

#endif
