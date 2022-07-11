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

	// set css classes
    add_css_class(class_global);
    tree.add_css_class(class_tree);
    title.add_css_class(class_title);
    webdar_css_style::assign_normal_classes(close);
    close.add_css_class(class_close);
    bt_class.add_css_class(class_url_link);
    bt_class.add_css_class(class_url_visited);
    bt_class.add_css_class(class_url_hover);
    focus.add_css_class(class_focus);

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


void html_listing_page::new_css_library_available()
{
    css tmp;
    css_selector_link tmp_link(class_url_link);
    css_selector_visited tmp_visited(class_url_visited);
    css_selector_hover tmp_hover(class_url_hover);

    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    if(! csslib->class_exists(class_global))
    {
	try
	{
	    tmp.css_clear_attributes();
	    tmp.css_height("100%",true);
	    csslib->add(class_global, tmp);

	    tmp.css_clear_attributes();
	    tmp.css_float(css::fl_left);
	    tmp.css_height("100%", false);
	    tmp.css_width("32.6%", false);
	    tmp.css_padding("0.2%");
	    tmp.css_overflow(css::ov_scroll);
	    tmp.css_margin_right("1%");
	    csslib->add(class_tree, tmp);

	    tmp.css_clear_attributes();
	    tmp.css_width("64%", false);
	    tmp.css_padding_top("1em");
	    tmp.css_padding_bottom("1em");
	    tmp.css_font_weight_bold();
	    tmp.css_font_style_italic();
	    tmp.css_text_align(css::al_center);
	    tmp.css_border_width(css::bd_all, css::bd_medium);
	    tmp.css_border_style(css::bd_all, css::bd_solid);
	    tmp.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF);
	    tmp.css_color(COLOR_MENU_FRONT_OFF);
	    tmp.css_background_color(COLOR_MENU_BACK_OFF);
	    tmp.css_position_type(css::pos_fixed);
	    tmp.css_position_top("1em");
	    tmp.css_position_right("1%");
	    csslib->add(class_title, tmp);

	    tmp.css_clear_attributes();
	    tmp.css_float(css::fl_right);
	    tmp.css_float_clear(css::fc_both);
	    tmp.css_position_type(css::pos_fixed);
	    tmp.css_position_bottom("1em");
	    tmp.css_position_right("1em");
	    csslib->add(class_close, tmp);

	    tmp.css_clear_attributes();
	    tmp.css_color(COLOR_TEXT);
	    tmp.css_text_align(css::al_center);
	    tmp.css_text_decoration(css::dc_none);
	    tmp_link.set_value(tmp);
	    tmp_visisted.set_value(tmp);
	    csslib->add(tmp_link);
	    csslib->add(rmp_visited);
	    tmp.css_text_decoration(css::dc_underline);
	    tmp_hover.set_value(tmp);
	    csslib->add(tmp_hover);

	    tmp.css_clear_attributes();
	    tmp.css_margin_top("5em");
	    tmp.css_width("66%", false);
	    csslib->add(class_focus, tmp);
	}
	catch(exception_range & e)
	{
	    throw WEBDAR_BUG;
	}
    }
    else
    {
	if(! csslib->class_exists(class_tree)
	   || ! csslib->class_exists(class_title)
	   || ! csslib->class_exists(class_close)
	   || ! csslib->class_exists(class_url_link)
	   || ! csslib->class_exists(class_url_visited)
	   || ! csslib->class_exists(class_url_hover)
	   || ! csslib->class_exists(class_focus))
	    throw WEBDAR_BUG;
    }

    webdar_css_style::update_library(*csslib.get());
}
