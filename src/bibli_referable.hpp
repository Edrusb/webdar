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

#ifndef BIBLI_REFERABLE_HPP
#define BIBLI_REFERABLE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "reference.hpp"

    /// \file bibli_referable.hpp defines bibli_referable class



    /// class bibli_referable

    /// bibli_referable is an interface own by objects a bibliotheque
    /// is able to store and associate to a name. There can be referred
    /// by objects of type bibli_renvoi (sort of pointer with reference
    /// counter)

class bibli_referable : public reference
{
public:
    bibli_referable() = default;
    bibli_referable(const bibli_referable & ref) = default;
    bibli_referable(bibli_referable && ref) noexcept(false) = default;
    bibli_referable & operator = (const bibli_referable & ref) = default;
    bibli_referable & operator = (bibli_referable && ref) noexcept(false) = default;
    virtual ~bibli_referable() {};

	/// whether the current object is referred by a bibli_renvoi
    bool referred() const { return ! is_empty(); };
};

#endif
