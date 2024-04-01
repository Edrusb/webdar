/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2023 Denis Corbin
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
#include "html_button.hpp"

using namespace std;

const string html_button::action = "click";

html_button::html_button(const std::string & label, const std::string & x_event_name):
    inside("", label)
{
    adopt(&inside);
    path_has_changed(); // to set link value of "inside" field

    event_name = x_event_name;
    register_name(event_name);
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

    return html_div::inherited_get_body_part(path, req);
}


void html_button::path_has_changed()
{
    chemin tmp = get_path();

    tmp.push_back(action);
    inside.change_url(tmp.display());
}
