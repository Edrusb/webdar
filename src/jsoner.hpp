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
#include <nlohmann/json.hpp>

    // webdar headers
#include "exceptions.hpp"

    /// \file jsoner.hpp defines jsoner class and class exception_json

using json = nlohmann::json;


    /// class exception_json

    /// cast exception from nlohmann::json library under the umbrella of our own exception types

class exception_json: public exception_base
{
public:
    exception_json(const json::exception & e): exception_base(e.what()) {};
    exception_json(const std::string & context, const json::exception & e):
	exception_base(context + ": " + e.what()) {};
    exception_json(const std::string & s): exception_base(s) {};

    virtual exception_base *clone() const override { return cloner<exception_json>((void *)this); };
};


    /// class jsoner

    /// jsoner class is the base class of all others that
    /// are able to save their status and read their status
    /// from a json formated configuration.

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

	/// \param[in] source json formated configuration to use for configuration
	/// \note exception exception_json should be throw if the
	/// provided data does not follow the expected structure
    virtual void load_json(const json & source) = 0;

	/// produce a json structure from the component configuration

	/// \return the current component configuration as a json object
	/// \note the json structure should contain:
	/// - a json format version
	/// - a json component identifier (name the class for example)
	/// - an arbitrary configuration under
	///  { "version": <num>, "id": "<class name>", "config": {...} }
	/// see the protected static methods that implement this.
    virtual json save_json() const = 0;

protected:

	/// given a version, class_id and configuration generates the global and common json structure

	/// \param[in] version define the expected field and structure of the config json parameter
	/// \param[in] class_id is expected to be the class name that implements the jsoner interface
	/// \param[in] config this is the resulting current configuration as a json object to be wrapped wil
	/// version and class_id.
    static json wrap_config_with_json_header(unsigned int version,
					     const std::string & class_id,
					     const json & config);


	/// from a given json global and common json structure split header parts and return the config part

	/// \param[in] json the global json configuration to apply to the current object
	/// \param[in] version the format and expected fields of the returned json configuration
	/// \param[in] class_id should match the name of the class this static method is used by
	/// \return inherited class specific data to apply to the current object following the format version 'version'
	/// \note may throw exception upon format error regarding expected json fields.
    static json unwrap_config_from_json_header(const json & source,
					       unsigned int & version,
					       std::string & class_id);

private:
    static constexpr const char* version_label = "version";
    static constexpr const char* id_label = "version";
    static constexpr const char* config_label = "config";


};

#endif
