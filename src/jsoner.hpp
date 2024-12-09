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

#ifndef JSONER_HPP
#define JSONER_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <string>

    // webdar headers


    /// \file jsoner.hpp defines jsoner class

    /// class jsoner

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

	/// setup the components from the json provided information

	/// \note exception exception_range should be throw if the
	/// provided data does not follow the expected structure
    virtual void load_json(const std::string & json) = 0;

	/// produce a json structure from the component configuration

	/// \note the json structure should contain:
	/// - a json format version
	/// - a json component identifier (name the class for example)
	/// - an arbitrary configuration under
	///  { "version": "<version">, "id": "<class name>", "config": {...} }
    virtual std::string save_json() const = 0;

protected:

	/// given a version, class_id and configuration generates the global and common json structure

    static std::string wrap_config_with_json_header(const std::string & version,
						    const std::string & class_id,
						    const std::string & config);


	/// from a given json global and common json structure split header parts and return the config part

	/// \note may throw exception upon format error regarding expected json fields.
    static std::string unwrap_config_from_json_header(const std::string & json,
						      std::string & version,
						      std::string & class_id);


};

#endif
