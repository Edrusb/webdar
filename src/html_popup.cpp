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



    //
#include "html_popup.hpp"

using namespace std;

const string html_popup::outerclassname = "html_popup_outer";

html_popup::html_popup(unsigned int width_pct, unsigned int height_pct)
{
    if(width_pct > 100 || height_pct > 100)
	throw exception_range("html_popup constructor arguments must be less or equal to 100 (%)");
    width = width_pct;
    height = height_pct;
    innerclassname = "html_popup_" + to_string(width) + "_" + to_string(height);

    	// outerbox must not be adopted, it lives outside any parent/child relationship
    outerbox.add_css_class(outerclassname);
    add_css_class(innerclassname);
}


string html_popup::inherited_get_body_part(const chemin & path,
					 const request & req)
{
    string ret = outerbox.get_body_part(path, req); // should be invoked with sub_path where sub_path  is path.pop_front()
    ret += html_div::inherited_get_body_part(path, req);
    return ret;
}

void html_popup::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();

    if(!csslib)
	throw WEBDAR_BUG;

    if(!csslib->class_exists(innerclassname))
    {
	css innerdef;

	innerdef.css_position_type(css::pos_fixed);
	innerdef.css_box_sizing(css::bx_border);
	innerdef.css_position_top(to_string((100-height)/2)+"vh");
	innerdef.css_position_left(to_string((100-width)/2)+"vw");
	innerdef.css_width(to_string(width)+"vw", true);
	innerdef.css_height(to_string(height)+"vh", true);
	innerdef.css_corner_radius("10px");
	innerdef.css_box_shadow("10px", "10px", "5px");
	innerdef.css_overflow(css::ov_auto);

	innerdef.css_z_index(2);

	innerdef.css_border_width(css::bd_all, css::bd_thin);
	innerdef.css_border_style(css::bd_all, css::bd_double);
	innerdef.css_border_color(css::bd_all, "rgb(0,0,0)");
	innerdef.css_background_color("#FFFFFF");

	innerdef.css_opacity("1");
	csslib->add(innerclassname, innerdef);
    }

    if(!csslib->class_exists(outerclassname))
    {
	css outerdef;

	    // have the outerbox span the whole page
	outerdef.css_position_type(css::pos_fixed);
	outerdef.css_position_top("0");
	outerdef.css_position_bottom("0");
	outerdef.css_position_left("0");
	outerdef.css_position_right("0");

	    // visible transparent color (should be parametrable in the future)
	outerdef.css_opacity("0.2");
	outerdef.css_background_color("rgb(0, 90, 200)");

	    // and on top of the existing page
	outerdef.css_z_index(1);

	csslib->add(outerclassname,outerdef);
    }
}
