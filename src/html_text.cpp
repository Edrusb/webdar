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
#include "webdar_tools.hpp"


    //
#include "html_text.hpp"

using namespace std;


void html_text::add_text(unsigned int level, const string & text)
{
    if(level == 0)
	txt += text + "\n";
	// the \n is only inside html code, it does not show in the rendering
    else
    {
	string h = webdar_tools_convert_to_string(level);
	txt += "<h" + h +">" + text + "</h" + h + ">\n";
    }
    my_body_part_has_changed();
}

string html_text::get_body_part() const
{
    string css_val = get_css_classes();

    if(css_val.empty())
	return txt;
    else
	if(!txt.empty())
	    return "<div " + css_val + ">" + txt + get_tooltip_body_part() + "</div>\n";
	else
	    return "";
}

string html_text::inherited_get_body_part(const chemin & path,
					  const request & req)
{
    return get_body_part(); // our own method inherited from static_body_builder
}
