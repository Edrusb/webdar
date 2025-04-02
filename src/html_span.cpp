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



    //
#include "html_span.hpp"

using namespace std;


string html_span::inherited_get_body_part(const chemin & path,
					 const request & req)
{
    string ret;
    string cssdef = get_css_classes();
    string sub = get_body_part_from_children_as_a_block(path, req);
    string css_grid;

    if(! grid_area.empty())
    {
	css tmp;

	tmp.css_grid_area_name(grid_area);
	css_grid = " style=\"" + tmp.css_get_raw_string() + ";";
    }

    if(cssdef.empty())
	if(sub.empty())
	    ret = "";
	else
	    ret = "<span" + css_grid +">\n" + sub + "</span>\n";
    else // cssdef not empty
	if(sub.empty())
	    ret = "<span " + cssdef + css_grid + ">" + "</span>\n";
	else
	    ret = "<span " + cssdef + css_grid + ">\n" + sub + "</span>\n";

    return ret;
}
