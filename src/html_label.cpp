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
#include "html_label.hpp"

using namespace std;

html_label::html_label(const std::string & x_label):
    label(x_label),
    tooltip(""),
    for_field(""),
    emphase(false)
{
}

html_label::html_label(const std::string & x_label, const std::string & x_tooltip):
    label(x_label),
    tooltip(x_tooltip),
    for_field(""),
    emphase(false)
{
}

string html_label::inherited_get_body_part(const chemin & path,
						const request & req)
{
    string ret;
    string extra = emphase ? webdar_css_style::text_bold : "";
    string cssdef = get_css_classes(extra);
    string forstring = for_field.empty() ? "" : " for=\"" + for_field + "\"";

    ret = "<label " + cssdef + forstring + ">\n";
    if(!tooltip.empty())
	ret += string("<span class=") + css_tooltip + ">\n";
    ret += label;
    if(!tooltip.empty())
    {
	ret += string("<span class=") + css_tooltiptext + ">";
	ret += tooltip;
	ret += "</span>\n</span>\n";
    }
    ret += "</label>";

    return ret;
}

void html_label::new_css_library_available()
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

	tmp.css_position_type(css::pos_relative);
	tmp.css_display("inline-block");
	tmp.css_border_width(css::bd_bottom, css::bd_thin);
	tmp.css_border_style(css::bd_bottom, css::bd_dotted);
	tmp.css_border_color(css::bd_bottom, COLOR_DAR_GREYBLUE);
	tip.set_value(tmp);

	tmp.clear();
	tmp.css_visibility("visible");
	tmp.css_opacity("1");
	tip.set_selector(css_class::hover, tmp, "." + string(css_tooltiptext));

	tmp.clear();
	tmp.css_visibility("hidden");
	tmp.css_width("120px", false);
	tmp.css_background_color("cyan");
	tmp.css_color(WHITE);
	tmp.css_text_h_align(css::al_center);
	tmp.css_corner_radius("6px");
	tmp.css_position_type(css::pos_absolute);
	tmp.css_z_index(2);
	tmp.css_position_bottom("125%");
	tmp.css_position_left("50%");
	tmp.css_margin_left("-60px");
	tmp.css_opacity("0");
	    // css_transition manque
	tiptext.set_value(tmp);

	tmp.clear();
	tmp.css_content("");
	tmp.css_color("Blue");
	tmp.css_position_type(css::pos_absolute);
	tmp.css_position_top("100%");
	tmp.css_position_left("50%");
	tmp.css_border_width(css::bd_top, css::bd_thick);
	tmp.css_border_color(css::bd_top, COLOR_DAR_GREYBLUE);
	tmp.css_border_color(css::bd_right, "transparent");
	tmp.css_border_color(css::bd_bottom, "transparent");
	tmp.css_border_color(css::bd_left, "transparent");
	tiptext.set_pseudo_element(css_class::after, tmp);
    }
}
