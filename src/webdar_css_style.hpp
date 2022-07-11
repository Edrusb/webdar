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
    const std::string class_button_normal = "webdar_css_style_btn_n";
    const std::string class_button_normal_link = "webdar_css_style_btn_nl";
    const std::string class_button_normal_active = "webdar_css_style_btn_na";
    const std::string class_button_normal_hover = "webdar_css_style_btn_nh";
    const std::string class_button_normal_visited = "webdar_css_style_btn_nv";
    const std::string class_button_selected = "webdar_css_style_btn_s";
    const std::string class_button_selected_link = "webdar_css_style_btn_sl";
    const std::string class_button_selected_active = "webdar_css_style_btn_sa";
    const std::string class_button_selected_hover = "webdar_css_style_btn_sh";
    const std::string class_button_selected_visited = "webdar_css_style_btn_sv";

    void update_library(css_library & ref);
    void assign_normal_classes(html_button & bt);
    void assign_active_classes(html_button & bt);

} // namespace end

#endif
