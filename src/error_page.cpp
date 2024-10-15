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


    // webdar headers
#include "html_div.hpp"
#include "tokens.hpp"
#include "html_menu.hpp"

    //
#include "error_page.hpp"

using namespace std;

error_page::error_page(unsigned int status_code,
                       const string & reason):
    page(reason)
{
    static const char* page_css_name = "page";
    static const char* body_css_name = "body";
    static const char* div_css_name = "div";
    static const char* text_css_name = "text";
    css mystyle;

        // objects's fields
    status = status_code;
    msg = reason;

        // building the body_builder tree

    div.adopt(&body);
    div.adopt(&text);
    page.adopt(&div);

        // setting css properties for each object

    mystyle.clear();
    mystyle.css_color(COLOR_TEXT);
    mystyle.css_background_color(COLOR_BACK);
    mystyle.css_padding("1em");
    page.define_css_class_in_library(page_css_name, mystyle);
    page.add_css_class(page_css_name);

    mystyle.clear();
    mystyle.css_font_weight_bold();
    mystyle.css_color(WHITE);
    mystyle.css_background_color(RED);
    mystyle.css_text_h_align(css::al_center);
    mystyle.css_padding("1em");
    mystyle.css_float(css::fl_left);
    mystyle.css_border_width(css::bd_all, css::bd_thick);
    mystyle.css_border_style(css::bd_all, css::bd_dashed);
    mystyle.css_border_color(css::bd_all, "rgb(100,0,0)");
    mystyle.css_width("6em", false);
    page.define_css_class_in_library(body_css_name, mystyle);
    body.add_css_class(body_css_name);

    mystyle.clear();
    mystyle.css_margin_left("13em");
    page.define_css_class_in_library(div_css_name, mystyle);
    div.add_css_class(div_css_name);

    mystyle.clear();
    mystyle.css_float(css::fl_left);
    mystyle.css_float_clear(css::fc_left);
    mystyle.css_margin_left("2em");
    page.define_css_class_in_library(text_css_name, mystyle);
    text.add_css_class(text_css_name);
}

void error_page::set_message_body(const string & message)
{
    body.clear();
    body.add_text(3, message);
}

void error_page::set_return_uri(const uri &ref, const string & label)
{
    text.clear();
    text.add_text(0, html_url(ref.get_string(), label).get_body_part());
}

answer error_page::give_answer(const request & req)
{
    answer ret;

    ret.set_status(status);
    ret.set_reason(msg);
    ret.add_body(page.get_body_part(req.get_uri().get_path(), req));

    return ret;
}

void error_page::prefix_has_changed()
{
    page.set_prefix(get_prefix());
}
