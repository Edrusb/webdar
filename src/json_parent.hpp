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

#ifndef JSON_PARENT_HPP
#define JSON_PARENT_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "reference.hpp"
#include "json_child.hpp"

    /// \file json_parent.hpp defines json_parent class
    ///
    /// json_parent class keep trace of dependencies a object
    /// has toward another jsoner object (though the class is not
    /// linked to jsoner but to the json_child class)


class json_parent: protected reference
{
public:
    json_parent() = default;
    json_parent(const json_parent & ref) = default;
    json_parent(json_parent && ref) noexcept(false) = default;
    json_parent & operator = (const json_parent & ref) = default;
    json_parent & operator = (json_parent && ref) noexcept(false) = default;
    virtual ~json_parent() {};

	/// add a new dependency on a child object
    void record_dependency_on(json_child* obj);

	/// remove a dependency from a child object
    void remove_dependency_on(json_child* obj);

};

#endif
