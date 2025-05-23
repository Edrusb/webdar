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
    string next_border_collapsed = string(" border-collapse: ") + (mode ? "collapse;" : "separate;");

    if(border_collapsed != next_border_collapsed)
    {
	border_collapsed = next_border_collapsed;
	my_body_part_has_changed();
    }
}

void html_table::set_css_class_first_row(const string & val)
{
    if(!cells_title_set || css_class_title != val)
    {
	cells_title_set = true;
	css_class_title = val;
	my_body_part_has_changed();
    }
}

void html_table::set_css_class_first_row()
{
    if(cells_title_set)
    {
	cells_title_set = false;
	css_class_title = "";
	my_body_part_has_changed();
    }
}


void html_table::set_css_class_first_column(const string & val)
{
    if(!cells_first_column_set || css_class_column1 != val)
    {
	cells_first_column_set = true;
	css_class_column1 = val;
	my_body_part_has_changed();
    }
}

void html_table::set_css_class_first_column()
{
    if(cells_first_column_set)
    {
	cells_first_column_set = false;
	css_class_column1 = "";
	my_body_part_has_changed();
    }
}

void html_table::set_css_class_cells(const string & val)
{
    if(css_class_cells != val)
    {
	css_class_cells = val;
	my_body_part_has_changed();
    }
}

void html_table::set_css_class_cells()
{
    if(css_class_cells != "")
    {
	css_class_cells = "";
	my_body_part_has_changed();
    }
}

string html_table::inherited_get_body_part(const chemin & path,
					   const request & req)
{
    string ret = "";
    bundle bdl;
    chemin sub_path = path;
    unsigned int line_length = 0;
    bool first_line = true;
    string table_css = "";
    string class_set;

    if(border_collapsed != "")
	table_css = "style=\"" + border_collapsed + "\"";

    if(sub_path.size() > 0)
	sub_path.pop_front();

    ret = "<table " + get_css_classes();
    if(table_css != "")
	ret += " " + table_css;
    ret += ">\n";

    reset_read_next();
    while(read_next(bdl))
    {
	    /// the <tr> mark for each new line
	if(line_length == 0)
	    ret += "<tr>\n";

	    /// the <td> ... cell ... </td>
	ret += "<td";
	class_set = "";

	    // first line
	if(first_line)
	{
	    if(!css_class_title.empty())
		class_set = css_class_title;
	}
	else
	{
	    if(!css_class_cells.empty())
		class_set = css_class_cells;
	}

	    // first column
	if(line_length == 0 && cells_first_column_set && ! css_class_column1.empty())
	{
	    if(class_set.empty())
		class_set = css_class_column1;
	    else
		class_set += " " + css_class_column1;
	}

	if(!class_set.empty())
	    ret += " class=\"" + class_set + "\"";

	ret += ">\n";

	if(bdl.obj != nullptr)
	    ret += bdl.obj->get_body_part(sub_path, req);
	else
	    ret += bdl.static_text;

	ret += "</td>\n";

	    /// counting cell to known when to close and start a new row

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

