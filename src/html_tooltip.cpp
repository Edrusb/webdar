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
#include <dar/tools.hpp>

    // webdar headers
#include "tokens.hpp"
#include "css.hpp"
#include "css_class.hpp"
#include "webdar_css_style.hpp"

    //
#include "html_tooltip.hpp"

using namespace std;

void html_tooltip::set_tooltip(const std::string & helper)
{
    tooltip = helper;

    if(tooltip.empty())
    {
	if(has_css_class(css_tooltip))
	    remove_css_class(css_tooltip);
    }
    else
    {
	if(!has_css_class(css_tooltip))
	    add_css_class(css_tooltip);
    }
}

string html_tooltip::get_body_part() const
{
    string ret;

    if(!tooltip.empty())
    {
	ret += string("<span class=\"") + css_tooltiptext + "\">";
	ret += webdar_tools_html_display(tooltip);
	ret += "</span>";
    }

    return ret;
}

void html_tooltip::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    if(! csslib->class_exists(css_tooltip))
    {
	css tmp;
	css_class tip(css_tooltip);
	css_class tiptext(css_tooltiptext);

	if(csslib->class_exists(css_tooltiptext))
	    throw WEBDAR_BUG;

	    /////
	    // the dashes under the text having tooltip enabled
	    //

	tmp.css_position_type(css::pos_relative);
	tmp.css_display("inline-block");
	tmp.css_text_shadow("0.05em", "0.05em", "0.2em", COLOR_TEXT);
	tmp.css_font_style_italic();
	tmp.css_transition("0.5s");
	tip.set_selector(css_class::hover, tmp);

	    /////
	    // making the tooltip show upon hover on the dashed bar
	    //

	tmp.clear();
	tmp.css_visibility(true);
	tmp.css_opacity("1");
	tip.set_selector(css_class::hover, tmp, "." + string(css_tooltiptext));

	csslib->add(tip);

	    /////
	    // the box containing the helper message
	    //

	tmp.clear();
	tmp.css_visibility(false);
	tmp.css_opacity("0");
	tmp.css_font_size("smaller");
	tmp.css_background_color(COLOR_DAR_GREYBLUE);
	tmp.css_color(COLOR_TEXT);
	tmp.css_text_h_align(css::al_center);
	tmp.css_corner_radius("0.5em", "0", "0.5em", "0");
	tmp.css_position_type(css::pos_absolute);
	tmp.css_z_index(2);
	tmp.css_position_bottom("125%");
	tmp.css_position_right("0%");
	tmp.css_position_left("40%");
	tmp.css_min_width("10em");
	tmp.css_margin_left("-60px");
	    // css_transition manque
	tmp.css_border_style(css::bd_bottom, css::bd_solid);
	tmp.css_border_style(css::bd_right, css::bd_solid);
	tmp.css_border_color(css::bd_bottom, COLOR_MENU_BORDER_OFF);
	tmp.css_border_color(css::bd_right, COLOR_MENU_BORDER_OFF);
	tmp.css_font_style_italic();
	tmp.css_transition("0.7s", "1s");
	tiptext.set_value(tmp);

	    /////
	    // the arrow below the tooltip (this is the top border of an empty box)
	    //

	tmp.clear();
	tmp.css_content("");
	tmp.css_position_type(css::pos_absolute);
	tmp.css_position_bottom("-16px");
	tmp.css_position_left("30%");
	tmp.css_border_width(css::bd_top, "10px");
	tmp.css_border_style(css::bd_all, css::bd_solid);
	tmp.css_border_color(css::bd_top, COLOR_MENU_BORDER_OFF);
	tmp.css_border_color(css::bd_right, "transparent");
	tmp.css_border_color(css::bd_bottom, "transparent");
	tmp.css_border_color(css::bd_left, "transparent");
	tiptext.set_pseudo_element(css_class::after, tmp);

	csslib->add(tiptext);
    }
}
