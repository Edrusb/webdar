/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013 Denis Corbin
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


    // webdar headers
#include "webdar_css_style.hpp"
#include "tokens.hpp"

    //
#include "html_listing_page.hpp"

using namespace std;

const string html_listing_page::event_close = "html_listing_page::close";

html_listing_page::html_listing_page():
    tree(""),
    close("Close", event_close)
{
    css tmp;

    set_session_name("");

	// set css properties
    css_height("100%",true); // applied to the html_page assign this CSS to the <body>

    tree.css_float(css::fl_left);
    tree.css_height("100%", false);
    tree.css_width("32.6%", false);
    tree.css_padding("0.2%");
    tree.css_overflow(css::ov_scroll);
    tree.css_margin_right("1%");


    title.css_width("64%", false);
    title.css_padding_top("1em");
    title.css_padding_bottom("1em");
    title.css_font_weight_bold();
    title.css_font_style_italic();
    title.css_text_align(css::al_center);
    title.css_border_width(css::bd_all, css::bd_medium);
    title.css_border_style(css::bd_all, css::bd_solid);
    title.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF);
    title.css_color(COLOR_MENU_FRONT_OFF);
    title.css_background_color(COLOR_MENU_BACK_OFF);
    title.css_position_type(css::pos_fixed);
    title.css_position_top("1em");
    title.css_position_right("1%");

    webdar_style_normal_button(close);
    close.css_float(css::fl_right);
    close.css_float_clear(css::fc_both);
    close.css_position_type(css::pos_fixed);
    close.css_position_bottom("1em");
    close.css_position_right("1em");

    tmp.css_clear_attributes();
    tmp.css_color(COLOR_TEXT);
    tmp.css_text_align(css::al_center);
    tmp.css_text_decoration(css::dc_none);
    bt_class.set_style_link(tmp);
    bt_class.set_style_visited(tmp);
    tmp.css_text_decoration(css::dc_underline);
    bt_class.set_style_hover(tmp);

    focus.css_margin_top("5em");
    focus.css_width("66%", false);

	// the body_builder tree
    adopt(&tree);
    adopt(&focus);
    adopt(&title);
    adopt(&close);
    adopt(&bt_class);

	//
    tree.set_css_classid(bt_class.get_class_id());

	// event binding
    register_name(event_close);
    close.record_actor_on_event(this, event_close);

	// linking tree with focus
    tree.set_drop_content(&focus);
    tree.set_drop_path(&title);
}

void html_listing_page::on_event(const std::string & event_name)
{
    if(event_name != event_close)
	throw WEBDAR_BUG;
    act(event_close); // propagate the event
}

void html_listing_page::set_session_name(const string & session_name)
{
    string title = "Webdar - ";

    if(session_name != "")
	title += session_name + " - ";

    title += "Archive listing";
    set_title(title);
}
