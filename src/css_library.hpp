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

#ifndef CSS_LIBRARY_HPP
#define CSS_LIBRARY_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "css_class.hpp"

class css_library
{
public:

    css_library() = default;
    css_library(const css_library & ref) = default;
    css_library(css_library && ref) noexcept = default;
    css_library & operator = (const css_library & ref) = default;
    css_library & operator = (css_library && ref) noexcept = default;
    ~css_library() = default;

    void add(const css_class & value);
    void add(const std::string & name, const css & value);
    bool class_exists(const std::string & name) const;
    bool get_value(const std::string & name, std::string & stored_value) const;
    void del(const std::string & name);
    unsigned int size() const { return content.size(); };

    std::string get_html_class_definitions() const;

private:
    std::map<std::string, std::string> content;

};

#endif
