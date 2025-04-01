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
#include "webdar_css_style.hpp"
#include "tokens.hpp"

    //
#include "html_listing_page.hpp"

using namespace std;

const string html_listing_page::event_close = "html_listing_page::close";

static const char* css_page = "html_listing_page";
static const char* css_tree = "html_listing_page_tree";
static const char* css_title = "html_listing_page_title";
static const char* css_close = "html_listing_page_close";
static const char* css_focus = "html_listing_page_focus";


html_listing_page::html_listing_page():
    tree(""),
    close("Close", event_close)
{

    set_session_name("");

	// the body_builder tree
    adopt(&tree);
    adopt(&focus);
    adopt(&title);
    adopt(&close);

	// event binding
    register_name(event_close);
    close.record_actor_on_event(this, event_close);

	// linking tree with focus
    tree.set_drop_content(&focus);
    tree.set_drop_path(&title);

	// css

    webdar_css_style::normal_button(close);
    close.add_css_class(css_close);

    tree.add_css_class(css_tree);
    title.add_css_class(css_title);
    title.add_css_class(webdar_css_style::btn_off);
    focus.add_css_class(css_focus);
}

void html_listing_page::on_event(const string & event_name)
{
    if(event_name != event_close)
	throw WEBDAR_BUG;
    act(event_close); // propagate the event
	// not calling 	my_body_part_has_changed();
	// as inherited_get_body_part() will not produce something
	// different due to this event
}

void html_listing_page::set_session_name(const string & session_name)
{
    string title = "Webdar - ";

    if(session_name != "")
	title += session_name + " - ";

    title += "Archive listing";
    set_title(title);
}

void html_listing_page::new_css_library_available()
{
    css tmp;

    unique_ptr<css_library> & csslib = lookup_css_library();

    if(!csslib)
	throw WEBDAR_BUG;

    if(csslib->class_exists(css_tree))
	return;

	// left dir tree

    tree.clear_css_classes();
    tmp.clear();
    tmp.css_box_sizing(css::bx_border);
    tmp.css_float(css::fl_left);
    tmp.css_max_height("calc(100vh - 1em)");
    tmp.css_width("30%", false);
    tmp.css_padding("1em");
    tmp.css_overflow(css::ov_scroll);
    csslib->add(css_tree, tmp);

	// title

    title.clear_css_classes();
    tmp.clear();
    tmp.css_box_sizing(css::bx_border);
    tmp.css_position_type(css::pos_fixed);
    tmp.css_position_top("1em");
    tmp.css_position_right("1em");
    tmp.css_width("calc(70% - 1em)", false); // 1em is the margin on the right
    csslib->add(css_title, tmp);

	// focus area

    focus.clear_css_classes();
    tmp.clear();
    tmp.css_box_sizing(css::bx_border);
    tmp.css_margin_top("5em");
    tmp.css_margin_right("1em");
    tmp.css_float(css::fl_right);
    tmp.css_overflow(css::ov_scroll);
    tmp.css_max_height("calc(100vh - 6em)");
    tmp.css_width("calc(70% - 1em)", false); // 1em for the right margin
    csslib->add(css_focus, tmp);

	// close button

    close.clear_css_classes();
    tmp.clear();
    tmp.css_float(css::fl_right);
    tmp.css_float_clear(css::fc_both);
    tmp.css_position_type(css::pos_fixed);
    tmp.css_position_bottom("1em");
    tmp.css_position_right("1em");
    csslib->add(css_close, tmp);

    webdar_css_style::update_library(*csslib);
}
