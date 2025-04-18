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

#ifndef HTML_MASK_HPP
#define HTML_MASK_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <memory>
#include <dar/libdar.hpp>

    // webdar headers
#include "body_builder.hpp"

    /// class html_mask defines the interface of all masks

class html_mask
{
public:
    html_mask() {};
    html_mask(const html_mask & ref) = default;
    html_mask(html_mask && ref) noexcept = default;
    html_mask & operator = (const html_mask & ref) = default;
    html_mask & operator = (html_mask && ref) noexcept = default;
    ~html_mask() = default;

	/// obtain the mask for libdar
    virtual std::unique_ptr<libdar::mask> get_mask() const = 0;

};

#endif
