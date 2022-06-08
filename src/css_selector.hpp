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

#ifndef CSS_SELECTOR_HPP
#define CSS_SELECTOR_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>


    // webdar headers
#include "css_class.hpp"

class css_selector : public css_class
{
public:

    css_selector(const std::string & name, const std::string & selector_name);
    css_selector(const css_selector & ref) = default;
    css_selector(css_selector && ref) noexcept = default;
    css_selector & operator = (const css_selector & ref) = default;
    css_selector & operator = (css_selector && ref) noexcept = default;
    ~css_selector() = default;
};

    // including C++ generated code for classes inherited from css_selector
#include "css_selector_subtypes.hpp"

#endif
