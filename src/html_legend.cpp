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
#include "html_legend.hpp"

using namespace std;

void html_legend::change_label(const string & label)
{
    if(x_label != label)
    {
	x_label = label;
	my_body_part_has_changed();
    }
}

string html_legend::inherited_get_body_part(const chemin & path,
						   const request & req)
{
    string ret = "";

    if(! x_label.empty())
    {
	ret += "<legend " + get_css_classes() + ">" + webdar_tools_html_display(x_label);
	ret += get_tooltip_body_part();
	ret += "</legend>\n";
    }
    ret += get_body_part_from_all_children(path, req);

    return ret;
}
