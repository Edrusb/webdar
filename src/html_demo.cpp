/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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
#include "tooltip_messages.hpp"

    //
#include "html_demo.hpp"

using namespace std;

html_demo::html_demo():
    html_popup(80, 80),
    table(2),
    left_input("Enter some text here",
	       html_form_input::text,
	       "",
	       "",
	       ""),
    left_fs("Information at Browser level"),
    form("Update"),
    btn_calc("Calculate", event_calc),
    right_input("Entered text",
		html_form_input::text,
		"",
		"",
		""),
    right_fs("Information known by Webdar"),
    counter("Provided string size", html_form_input::number, "0", "10", ""),
    label("- this text is provided without additional information"),
    labelplus("- while this text is provided with helper information", TOOLTIP_D_LABEL),
    close("Close", event_close)
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
    description.add_paragraph();
    description.add_text(0, "Note also that positionning your pointer on some text for more than one second will show helper information as illustrated just below:");
    description.add_nl();

    labelsep.add_nl();

    static const char* choice1 = "hidden";
    static const char* choice2 = "disabled";
    static const char* choice3 = "enabled";

    left_radio.add_choice("0", choice1);
    left_radio.add_choice("1", choice2);
    left_radio.add_choice("2", choice3);
    left_radio.set_selected_num(2);

    right_radio.add_choice("0", choice1);
    right_radio.add_choice("1", choice2);
    right_radio.add_choice("2", choice3);
    right_radio.set_selected_num(0);

	// adoption tree

    adopt(&desc_div);
    desc_div.adopt(&description);
    desc_div.adopt(&label);
    desc_div.adopt(&labelsep);
    desc_div.adopt(&labelplus);
    adopt(&table);

    table.adopt(&form);
    form.adopt(&left_fs);
    left_fs.adopt(&left_input);
    left_fs.adopt(&left_radio);

    table.adopt(&right_fs);
    right_fs.adopt(&right_input);
    right_fs.adopt(&right_radio);

    table.adopt(&btn_div);
    btn_div.adopt(&btn_calc);

    table.adopt(&counter);
    adopt(&close);

	// events

    left_input.record_actor_on_event(this, html_form_input::changed);
    left_radio.record_actor_on_event(this, html_form_radio::changed);
    btn_calc.record_actor_on_event(this, event_calc);
    close.record_actor_on_event(this, event_close);

	// visibility and enablement

    right_input.set_enabled(false);
    counter.set_enabled(false);
    right_radio.set_enabled(false);

	// css

    desc_div.add_css_class(css_table);

    table.set_css_class_first_row(css_cells);
    table.set_css_class_cells(css_cells);
    table.css_border_collapsed(true);
    table.add_css_class(css_table);

    webdar_css_style::normal_button(btn_calc);
    webdar_css_style::normal_button(close);
    btn_calc.add_css_class(webdar_css_style::float_right);
    close.add_css_class(css_btn_close);


	// visibility & coherence
    on_event(html_form_input::changed);
    on_event(html_form_radio::changed);

	// tooltips

    left_radio.set_tooltip(0, TOOLTIP_D_LEFT0);
    left_radio.set_tooltip(1, TOOLTIP_D_LEFT1);
    left_radio.set_tooltip(2, TOOLTIP_D_LEFT2);
    left_input.set_tooltip(TOOLTIP_D_LI);
    right_input.set_tooltip(TOOLTIP_D_RI);
    counter.set_tooltip(TOOLTIP_D_COUNTER);
}


void html_demo::on_event(const string & event_name)
{
    if(event_name == html_form_input::changed)
	right_input.set_value(left_input.get_value());
    else if(event_name == html_form_radio::changed)
    {
	right_radio.set_selected_num(left_radio.get_selected_num());
	switch(left_radio.get_selected_num())
	{
	case 0: // hidden
	    left_input.set_visible(false);
	    break;
	case 1: // disabled
	    left_input.set_visible(true);
	    left_input.set_enabled(false);
	    break;
	case 2: // enabled
	    left_input.set_visible(true);
	    left_input.set_enabled(true);
	    break;
	default:
	    throw WEBDAR_BUG;
	}
    }
    else if(event_name == event_calc)
    {
	counter.set_value(webdar_tools_convert_to_string(right_input.get_value().size()));
    }
    else if(event_name == event_close)
	set_visible(false);
    else
	throw WEBDAR_BUG;
}

void html_demo::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);


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
	tmp.css_width("95%", true);
	tmp.css_margin_top("2em");
	csslib->add(css_table, tmp);

	tmp.clear();
	tmp.css_position_type(css::pos_sticky);
	tmp.css_float(css::fl_right);
	tmp.css_position_bottom("1em");
	tmp.css_position_right("1em");
	tmp.css_margin_top("1em");
	csslib->add(css_btn_close, tmp);
    }

    html_popup::new_css_library_available();
}
