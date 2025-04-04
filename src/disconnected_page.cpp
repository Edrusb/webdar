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
#include "disconnected_page.hpp"

using namespace std;

disconnected_page::disconnected_page():
    page("disconnected from Webdar"),
    redir(false)
{

	// component setup
    msg.add_text(3, "You are now disconnected from Webdar");

	// adoption tree
    page.adopt(&msg);

	// events


	// css
    static const char* text_css = "disconnected_page_text";
    css_class text_class(text_css);
    css tmp;

    tmp.css_text_h_align(css::al_center);
    tmp.css_text_v_align(css::al_middle);
    text_class.set_value(tmp);
    msg.add_css_class(text_css);

    page.define_css_class_in_library(text_class);
}

answer disconnected_page::give_answer(const request & req)
{
    answer ret;

    if(redir)
    {
	uri target(req.get_uri());
	target.trim_path(); // to avoid pointing on the disconnect and force the user to select a session
	page.set_refresh_redirection(0, target.get_string());
    }
    else
	page.set_refresh_redirection(0, "");
    ret.set_status(STATUS_CODE_OK);
    ret.set_reason("ok");
    ret.add_body(page.get_body_part(req.get_uri().get_path(), req));

    return ret;
}

void disconnected_page::prefix_has_changed()
{
    page.set_prefix(get_prefix());
}
