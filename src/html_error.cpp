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
#include "webdar_css_style.hpp"
#include "css.hpp"
    //
#include "html_error.hpp"

using namespace std;

const string fixed_title1 = "Webdar - ";
const string fixed_title2 = "Libdar message";

const char* html_error::acknowledged = "html_error_acknowledged";
const char* html_error::class_message = "html_error_mesg";
const char* html_error::class_global = "html_error_glb";
const char* html_error::class_button = "html_error_btn";
const char* html_error::close_event = "html_error_button_close";

html_error::html_error():
    html_page(fixed_title1+fixed_title2),
    close("Close", close_event)
{
    css tmp;

    the_error.add_css_class(class_message);
    the_error.add_css_class(webdar_css_style::text_color_red);
    global.add_css_class(webdar_css_style::red_border);
    global.add_css_class(class_global);
    webdar_css_style::normal_button(close);
    close.add_css_class(class_button);

    global.adopt(&the_error);
    adopt(&global);
    adopt(&close);

    close.record_actor_on_event(this, close_event);

    register_name(acknowledged);
}

void html_error::set_message(const string & msg)
{
    the_error.clear();
    the_error.add_text(3, msg);
}

void html_error::on_event(const string & event_name)
{
    act(acknowledged);
	// we propagate the event, but it does not
	// change the result of inherited_get_body_part()
	// thus we dont call my_body_part_has_changed();
}

void html_error::set_session_name(const string & sessname)
{
    set_title(fixed_title1 + sessname + " - " + fixed_title2);
}


void html_error::new_css_library_available()
{
    css tmp;

    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    if(! csslib->class_exists(class_message))
    {
	try
	{
	    tmp.clear();
	    tmp.css_text_h_align(css::al_center);
	    csslib->add(class_message, tmp);

	    tmp.clear();
	    tmp.css_padding("1em");
	    tmp.css_margin("1em");
	    tmp.css_border_width(css::bd_all, css::bd_medium);
	    tmp.css_border_style(css::bd_all, css::bd_inset);
	    csslib->add(class_global, tmp);

	    tmp.clear();
	    tmp.css_float(css::fl_right);
	    tmp.css_margin_right("1em");
	    csslib->add(class_button, tmp);
	}
	catch(exception_range & e)
	{
	    throw WEBDAR_BUG;
	}
    }
    else
    {
	if(! csslib->class_exists(class_global)
	   || ! csslib->class_exists(class_button))
	    throw WEBDAR_BUG;
    }

    webdar_css_style::update_library(*csslib);
}
