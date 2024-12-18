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

#ifndef XXX_HPP
#define XXX_HPP

    // C system header files
#include "my_config.h"

extern "C"
{

}

    // C++ system header files

    // webdar headers

    /// class XXX description

    /// class XXX long description
    ///

class XXX
{
public:
    XXX(const std::string & title);
    XXX(const XXX & ref) = default;
    XXX(XXX && ref) noexcept = default;
    XXX & operator = (const XXX & ref) = default;
    XXX & operator = (XXX && ref) noexcept = default;
    ~XXX() = default;
};

#endif
