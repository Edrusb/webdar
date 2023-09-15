/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2023 Denis Corbin
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

#ifndef JSON_CHILD_HPP
#define JSON_CHILD_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "reference.hpp"

    /// \file json_child.hpp defines json_child class
    ///
    /// json_child class is the base class of all others that
    /// are able to save their status and read their status
    /// from a configuration file (json formated)

class json_child : public reference
{
public:
    json_child() = default;
    json_child(const json_child & ref) = default;
    json_child(json_child && ref) noexcept(false) = default;
    json_child & operator = (const json_child & ref) = default;
    json_child & operator = (json_child && ref) noexcept(false) = default;
    virtual ~json_child() {};

	/// whether the current object has a parent
    bool orphaned() const { return is_empty(); };

	/// return the parent object

	/// throw exception if the object is orphaned
    reference* get_parent() const;

};

#endif
