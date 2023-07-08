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

#ifndef CSS_CLASS_GROUP_HPP
#define CSS_CLASS_GROUP_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>


    // webdar headers

    /// this is only a way to manipulate a list of existing class names
    /// no class definition is done here just a list of names as a set

class css_class_group
{
public:

    css_class_group() { clear_css_classes(); };
    css_class_group(const css_class_group & ref) = default;
    css_class_group(css_class_group && ref) noexcept = default;
    css_class_group & operator = (const css_class_group & ref) = default;
    css_class_group & operator = (css_class_group && ref) noexcept = default;
    ~css_class_group() = default;

	/// add a class name to the list
    void add_css_class(const std::string & name);

	/// remove a class from the list
    void remove_css_class(const std::string & name);

	/// remove all classes
    void clear_css_classes();

	/// restart reading from the first element
    void reset_read() const;

	/// read the next element or return false
    bool read_next(std::string & next_class) const;

private:
    std::set<std::string> content;
    mutable std::set<std::string>::iterator reader;

};

#endif
