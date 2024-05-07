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
#include "webdar_tools.hpp"


    //
#include "html_text.hpp"

using namespace std;


void html_text::add_text(unsigned int level, const string & text)
{
    string css_val = get_css_classes();

    if(level == 0)
	if(css_val.empty())
	    txt += text + "\n";
	else
	    txt += "<div " + css_val + ">" + text + "</div>\n";
    else
    {
	string h = webdar_tools_convert_to_string(level);
	txt += "<h" + h + " " + css_val + ">" + text + "</h" + h + ">\n";
    }
    my_body_part_has_changed();
}

string html_text::inherited_get_body_part(const chemin & path,
					  const request & req)
{
    return get_body_part(); // our own method inherited from static_body_builder
}
