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
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_double_button.hpp"

using namespace std;

html_double_button::html_double_button(const string & label, const string & x_event_name):
    one(label, x_event_name),
    two(label, x_event_name),
    one_visible(false)
{
    adopt(&one);
    adopt(&two);
    swap();

    register_name(x_event_name);
    one.record_actor_on_event(this, x_event_name);
    two.record_actor_on_event(this, x_event_name);
}

void html_double_button::on_event(const std::string & event_name)
{
    act(event_name); // propagate the event
}

string html_double_button::inherited_get_body_part(const chemin & path,
					    const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_double_button::css_classes_have_changed()
{
    css_class_group myset = get_css_class_group();

    one.clear_css_classes();
    two.clear_css_classes();
    one.add_css_class(myset);
    two.add_css_class(myset);
}

void html_double_button::swap()
{
    one_visible = ! one_visible;

    one.set_visible(one_visible);
    two.set_visible(!one_visible);
}


