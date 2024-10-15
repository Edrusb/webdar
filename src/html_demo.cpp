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
#include "webdar_tools.hpp"
#include "webdar_css_style.hpp"
#include "tokens.hpp"

    //
#include "html_demo.hpp"

using namespace std;

html_demo::html_demo():
    table(2),
    left_input("Enter some text here",
	       html_form_input::text,
	       "",
	       30),
    left_fs("Information at Browser level"),
    form("Update"),
    btn_incr("Increment", event_incr),
    btn_clear("Clear", event_clear),
    right_input("Entered text",
		html_form_input::text,
		"",
		30),
    right_fs("Information known by Webdar"),
    counter("Counter", html_form_input::number, "0", 10)
{

	// components configuration
    description.add_text(2, "By Design");
    description.add_text(0, "By design, Webdar does not use any script but only pure ");
    description.add_text(0, "HTML directives. This has the advantage of security but ");
    description.add_text(0, "the drawback of HTML and HTTP(S) limitations.");
    description.add_text(0, "In particular the information typed in the browser");

    description.add_text(0, "is not immediately sent and available on the server side, where");
    description.add_text(0, "Webdar is located. The below components illustrates the ");
    description.add_text(0, "HTML/HTTP limitations, so you can familiarize yourself with them.");
    description.add_nl();
    description.add_text(2, "How to use this playground");
    description.add_text(0, "On the left, you can enter text, select components and click on the different buttons.");
    description.add_nl();
    description.add_text(0, "On the right, you will see when and what information Webdar will receive.");
    description.add_nl();

    static const char* choice1 = "choice 1";
    static const char* choice2 = "choice 2";
    static const char* choice3 = "choice 3";

    left_radio.add_choice("1", choice1);
    left_radio.add_choice("2", choice2);
    left_radio.add_choice("3", choice3);
    left_radio.set_selected(0);

    right_radio.add_choice("1", choice1);
    right_radio.add_choice("2", choice2);
    right_radio.add_choice("3", choice3);
    right_radio.set_selected(0);

	// adoption tree

    adopt(&desc_div);
    desc_div.adopt(&description);
    adopt(&table);

    table.adopt(&form);
    form.adopt(&left_fs);
    left_fs.adopt(&left_input);
    left_fs.adopt(&left_radio);

    table.adopt(&right_fs);
    right_fs.adopt(&right_input);
    right_fs.adopt(&right_radio);

    table.adopt(&btn_div);
    btn_div.adopt(&btn_incr);
    btn_div.adopt(&btn_clear);

    table.adopt(&counter);

	// events

    left_input.record_actor_on_event(this, html_form_input::changed);
    left_radio.record_actor_on_event(this, html_form_radio::changed);
    btn_incr.record_actor_on_event(this, event_incr);
    btn_clear.record_actor_on_event(this, event_clear);

	// visibility and enablement

    right_input.set_enabled(false);
    counter.set_enabled(false);

	// css

    desc_div.add_css_class(css_table);

    table.set_css_class_first_row(css_cells);
    table.set_css_class_cells(css_cells);
    table.css_border_collapsed(true);
    table.add_css_class(css_table);
}


void html_demo::on_event(const string & event_name)
{
    if(event_name == html_form_input::changed)
	right_input.set_value(left_input.get_value());
    else if(event_name == html_form_radio::changed)
	right_radio.set_selected(left_radio.get_selected_num());
    else if(event_name == event_incr)
	counter.set_value(webdar_tools_convert_to_string(webdar_tools_convert_to_int(counter.get_value()) + 1));
    else if(event_name == event_clear)
	counter.set_value("0");
    else
	throw WEBDAR_BUG;
}


string html_demo::inherited_get_body_part(const chemin & path,
					  const request & req)
{
    return get_body_part_from_all_children(path, req);
}


void html_demo::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
    webdar_css_style::normal_button(btn_incr);
    webdar_css_style::normal_button(btn_clear);
    btn_incr.add_css_class(css_btn);
    btn_clear.add_css_class(css_btn_clear);

    if(! csslib->class_exists(css_table))
    {
	css tmp;
	tmp.css_text_v_align(css::al_top);
	tmp.css_text_h_align(css::al_center);
	tmp.css_border_width(css::bd_all, css::bd_thin);
	tmp.css_border_style(css::bd_all, css::bd_solid);
	tmp.css_border_color(css::bd_all, COLOR_MENU_BORDER_GREY);
	csslib->add(css_cells, tmp);

	tmp.clear();
	tmp.css_width("80%", true);
	tmp.css_margin_top("2em");
	csslib->add(css_table, tmp);

	tmp.clear();
	tmp.css_float(css::fl_left);
	csslib->add(css_btn, tmp);

	tmp.clear();
	tmp.css_float(css::fl_right);
	csslib->add(css_btn_clear, tmp);
    }
}
