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

#ifndef HTML_TABLE_HPP
#define HTML_TABLE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <vector>
#include <string>

    // webdar headers
#include "html_level.hpp"
#include "webdar_tools.hpp"
#include "exceptions.hpp"

class html_table: public html_level
{
public:
    html_table(unsigned int width);
    html_table(const html_table & ref)  = delete;
    html_table(html_table && ref) noexcept = delete;
    const html_table & operator = (const html_table & ref) = delete;
    html_table & operator = (html_table && ref) noexcept = delete;
    ~html_table() { clear_children(); };

	/// whether border are collapsed or separated (collasped by default)
    void css_border_collapsed(bool mode);

	/// define a specific css object for the _cells_ first row
    void set_css_class_first_row(const std::string & val) { cells_title_set = true; css_class_title = val; };

	/// clear apreviously assigned css object to the title row
    void set_css_class_first_row() { cells_title_set = false; css_class_title = ""; };

	/// assigned to all celles except those of the first raw if set_css_cells_first_raw was set
    void set_css_class_cells(const std::string & val) { css_class_cells = val; };

	/// clear apreviously assigned css object to the title raw
    void set_css_class_cells() { css_class_cells = ""; };

	/// clear table content
    void clear() { clear_children(); };

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    unsigned int dim_x;
    std::string border_collapsed;
    bool cells_title_set;
    std::string css_class_title;
    std::string css_class_cells;
};

#endif
