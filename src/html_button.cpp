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
#include "html_page.hpp"


    //
#include "html_button.hpp"

using namespace std;

const string html_button::action = "click";

html_button::html_button(const string & x_label, const string & x_event_name):
    label(x_label),
    inside(""),
    event_name(x_event_name)
{
    change_label(x_label);
    outside.adopt(&text);
    inside.adopt(&outside); // yes, historically the url was inside the box... :)
    adopt(&inside);
    path_has_changed(); // to set link value of "inside" field

    register_name(event_name);

}

void html_button::change_label(const std::string & x_label)
{
    label = x_label;
    text.clear();
    text.add_text(0, label);
    my_body_part_has_changed();
}


string html_button::inherited_get_body_part(const chemin & path,
					    const request & req)
{
    chemin target = req.get_uri().get_path();
    string choice;

    if(!target.empty())
    {
	choice = target.back();
	target.pop_back();
    }
    else
	choice = "";

    if(target == get_path() && choice == action)
	act(event_name);

    return get_body_part_from_all_children(path, req);
}

void html_button::path_has_changed()
{
    chemin tmp = get_path();

    tmp.push_back(action);
    inside.change_url(tmp.display());
}

void html_button::css_classes_have_changed()
{
    css_class_group assigned_css = get_css_class_group();

	// we transfer to 'outside' the css classes assigned to 'this'
    outside.clear_css_classes();
    outside.add_css_class(assigned_css);
}
