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



    //
#include "html_disconnect.hpp"

using namespace std;

const string html_disconnect::event_disconn = "disconnect";
const string html_disconnect::css_disconn = "html_disco";

html_disconnect::html_disconnect():
    html_button("Disconnect", event_disconn)
{

	// css
    webdar_css_style::normal_button(*this, true);
    add_css_class(css_disconn);
}


void html_disconnect::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);

    if(!csslib->class_exists(css_disconn))
    {
	css tmp;
	css_class tmp_class(css_disconn);

	tmp.clear();
	tmp.css_float(css::fl_right);
	tmp.css_float_clear(css::fc_both);
	tmp.css_position_type(css::pos_fixed);
	tmp.css_position_top("0", false);
	tmp.css_position_right("0", false);
	tmp.css_box_shadow("5px", "5px");
	tmp.css_opacity("0.2");
	tmp.css_z_index(1);
	tmp_class.set_value(tmp);

	tmp.clear();
	tmp.css_opacity("1");
	tmp_class.set_selector(css_class::hover, tmp);

	csslib->add(tmp_class);
    }
}

