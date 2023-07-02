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
#include "exceptions.hpp"


    //
#include "html_table.hpp"

using namespace std;

html_table::html_table(unsigned int width)
{
    dim_x = width;
    border_collapsed = "";
    cells_title_set = false;
    css_class_title = "";
    css_class_cells = "";
}

void html_table::css_border_collapsed(bool mode)
{
    border_collapsed = string(" border-collapse: ") + (mode ? "collapse;" : "separate;");
}

string html_table::get_body_part(const chemin & path,
				 const request & req)
{
    string ret = "";
    bundle bdl;
    chemin sub_path = path;
    unsigned int line_length = 0;
    bool first_line = true;
    string table_css = "";

    if(border_collapsed != "")
	table_css = "style=\"" + border_collapsed + "\"";

    if(sub_path.size() > 0)
	sub_path.pop_front();

    ret = "<table " + get_css_classes() + " " + table_css  + ">\n";

    reset_read_next();
    while(read_next(bdl))
    {
	    /// the <tr> mark for each new line
	if(line_length == 0)
	    ret += "<tr>\n";

	    /// the <td> ... cell ... </td>
	ret += "<td";
	if(first_line)
	{
	    if(!css_class_title.empty())
		ret += " class=\"" + css_class_title +"\"";
	}
	else
	{
	    if(!css_class_cells.empty())
		ret += " class=\"" + css_class_cells +"\"";
	}
	ret += ">\n";

	if(bdl.obj != nullptr)
	    ret += bdl.obj->get_body_part(sub_path, req);
	else
	    ret += bdl.static_text;

	ret += "</td>\n";

	    /// counting cell to known when to close and start a new raw

	++line_length;
	if(line_length == dim_x)
	{
	    ret += "</tr>\n"; // end of raw
	    line_length = 0;
	    first_line = false;
	}
    }

	/// adding empty cells if necessary (well, ... that's a bit useless, but we can't know whether a particular browser will not cry on that)

    if(line_length != 0)
    {
	while(line_length < dim_x)
	{
	    ret += "<td></td>\n";
	    ++line_length;
	}
	ret += "<tr>\n";
    }
    ret += "</table>\n";

    return ret;
}

