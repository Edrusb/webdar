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

#ifndef JSONER_HPP
#define JSONER_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers


    /// \file jsoner.hpp defines jsoner class
    ///
    /// jsoner class is the base class of all others that
    /// are able to save their status and read their status
    /// from a configuration file (json formated)

class jsoner
{
public:
    jsoner() = default;
    jsoner(const jsoner & ref) = default;
    jsoner(jsoner && ref) noexcept(false) = default;
    jsoner & operator = (const jsoner & ref) = default;
    jsoner & operator = (jsoner && ref) noexcept(false) = default;
    virtual ~jsoner() = default;

	/// read from an already open file the part that will provide the object status

	/// \note in case of unexpected data exception_range should be thrown
    virtual void read_json(const libdar::fichier_global & ref) = 0;

	/// write at its turn in the already open file data that will let it read from read_json in the future
    virtual void write_json(const libdar::fichier_global & ref) const = 0;

};

#endif
