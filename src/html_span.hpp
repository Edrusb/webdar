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

#ifndef HTML_SPAN_HPP
#define HTML_SPAN_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_level.hpp"

    /// class html_span is the implementation of <span> </span>

    /// objects get enclosed in the <span> </span> if they are
    /// adopted (from body_builder::adopt() method)

class html_span : public html_level
{
public:
    html_span() = default;
    html_span(const html_span & ref) = delete;
    html_span(html_span && ref) noexcept = delete;
    html_span & operator = (const html_span & ref) = delete;
    html_span & operator = (html_span && ref) noexcept = delete;
    ~html_span() = default;

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    std::string grid_area; ///< CSS grid area name (empty if not used)


};

#endif
