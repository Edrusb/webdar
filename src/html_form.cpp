/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2022 Denis Corbin
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
#include "html_form.hpp"

using namespace std;

string html_form::inherited_get_body_part(const chemin & path,
					  const request & req)
{
    string ret = "";

    ret += "<form method=\"post\" action=\"" + get_path().display() + "\">\n";
    if((get_path() != req.get_uri().get_path() && req.get_method() == "POST") || (!get_visible() && get_next_visible()))
    {
	request tmp = req;
	tmp.change_method("GET");
	ret += get_body_part_from_all_children(path, tmp);
    }
    else
	ret += get_body_part_from_all_children(path, req);
    ret += "<input " +  get_css_classes() + " type=\"submit\" value=\"" + go_mesg + "\" />\n";
    ret += "</form>\n";

    if(!get_next_visible())
	ret = "";
    ack_visible();

    return ret;
}
