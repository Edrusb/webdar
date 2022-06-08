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

#ifndef CSS_CLASS_HPP
#define CSS_CLASS_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>


    // webdar headers
#include "css.hpp"

class css_class
{
public:

    css_class(const std::string & name);
    css_class(const std::string & name, const css & ref);
    css_class(const css_class & ref) = default;
    css_class(css_class && ref) noexcept = default;
    css_class & operator = (const css_class & ref) = default;
    css_class & operator = (css_class && ref) noexcept = default;
    virtual ~css_class() = default;

	/// get the css_class name
    const std::string & get_name() const { return class_name; };

	/// defines the css_class value from a css object
    void set_value(const css & ref) { class_value = ref.css_get_raw_string(); };

	/// clear css_class value
    void clear() { class_value = ""; };

	/// returns the css class definition
    std::string get_definition() const;

protected:
    virtual std::string convert_name(const std::string & name) const { return "." + name; };

private:
    std::string class_name;
    std::string class_value;

};

#endif
