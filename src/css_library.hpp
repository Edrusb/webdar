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

#ifndef CSS_LIBRARY_HPP
#define CSS_LIBRARY_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "css_class.hpp"

    /// class css_library

    /// holds a list of css classes (css name and its associated set of css properties (aka css object))
    ///

class css_library
{
public:

    css_library() = default;
    css_library(const css_library & ref) = default;
    css_library(css_library && ref) noexcept = default;
    css_library & operator = (const css_library & ref) = default;
    css_library & operator = (css_library && ref) noexcept = default;
    ~css_library() = default;

	/// add a new class to the library (must not already exist in the library)
    void add(const css_class & value);

	/// add on-fly a new class to the library from its name and a css object
    void add(const std::string & name, const css & value);

	/// check whether a class has a definition in this library object
    bool class_exists(const std::string & name) const;

	/// obtain the definition of the given class
    bool get_value(const std::string & name, std::string & stored_value) const;

	/// remove a class from the library
    void del(const std::string & name);

	/// return the number of class in that library
    unsigned int size() const { return content.size(); };

	/// provide a css definition of all classes of the library suitable for HTML headers or CSS files
    std::string get_html_class_definitions() const;

private:
    std::map<std::string, std::string> content;

};

#endif
