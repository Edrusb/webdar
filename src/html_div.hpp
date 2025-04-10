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

#ifndef HTML_DIV_HPP
#define HTML_DIV_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_level.hpp"

    /// class html_div is the implementation of <div> </div>

    /// objects get enclosed in the <div> </div> if they are
    /// adopted (from body_builder::adopt() method)

class html_div : public html_level
{
public:
    html_div() = default;
    html_div(const html_div & ref) = delete;
    html_div(html_div && ref) noexcept = delete;
    html_div & operator = (const html_div & ref) = delete;
    html_div & operator = (html_div && ref) noexcept = delete;
    ~html_div() = default;

	/// to be used when the component is adopted by another having a grid-template css property set

	/// \note set to an empty string to clear/disable this property
    void set_grid_area_name(const std::string & area_name) { grid_area = area_name; };

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    std::string grid_area; ///< CSS grid area name (empty if not used)


};

#endif
