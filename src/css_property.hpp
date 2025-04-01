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

#ifndef CSS_PROPERTY_HPP
#define CSS_PROPERTY_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "exceptions.hpp"

    /// class css_property hold the value of a single css property

    ///.  it does not only record the value but also the absence of value
    /// when a property is not defined.

class css_property
{
public:
    css_property() { clear(); };
    css_property(const css_property & ref) = default;
    css_property(css_property && ref) noexcept = default;
    css_property & operator = (const css_property & ref) = default;
    css_property & operator = (css_property && ref) noexcept = default;
    ~css_property() = default;

    void clear();
    void set_value(const std::string & val);
    const std::string & get_value() const { return value; };
    bool is_unset() const { return unset; };

	/// update current object with argument ignoring if the field is unset
    void update_from(const css_property & ref);

private:
    std::string value;
    bool unset;

};


#endif
