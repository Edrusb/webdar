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

#ifndef STATIC_BODY_BUILDER_HPP
#define STATIC_BODY_BUILDER_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers

    /// base class of html components that are static

    /// \note the difference from dynamic html components resided in the
    /// get_body_part() method that here dot receive any argument,
    /// while for dynamic html objects (see class body_builder) the
    /// same method receive the request received from the browser

class static_body_builder
{
public:
    static_body_builder() = default;
    static_body_builder(const static_body_builder & ref) = default;
    static_body_builder(static_body_builder && ref) noexcept = default;
    static_body_builder & operator = (const static_body_builder & ref) = default;
    static_body_builder & operator = (static_body_builder && ref) noexcept = default;
    virtual ~static_body_builder() {};

    virtual std::string get_body_part() const = 0;
};

#endif
