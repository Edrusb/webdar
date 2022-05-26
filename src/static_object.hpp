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

#ifndef STATIC_OBJECT_HPP
#define STATIC_OBJECT_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "answer.hpp"

    /// common ancestor to all static object, this makes easier to add new object type in the future
class static_object
{
public:
    static_object() = default;
    static_object(const static_object & ref) = default;
    static_object(static_object && ref) noexcept = default;
    static_object & operator = (const static_object & ref) = default;
    static_object & operator = (static_object && ref) noexcept = default;
    virtual ~static_object() {};

    virtual answer give_answer() const = 0;
};


    /// static_object to return plain text as an answer

class static_object_text : public static_object
{
public:
    static_object_text(const char *text) { data = text; };
    static_object_text(const static_object_text & ref) = default;
    static_object_text(static_object_text && ref) noexcept = default;
    static_object_text & operator = (const static_object_text & ref) = default;
    static_object_text & operator = (static_object_text && ref) noexcept = default;
    ~static_object_text() = default;

	/// inherited from static_object
    virtual answer give_answer() const;

private:
    const char *data;
};

#endif
