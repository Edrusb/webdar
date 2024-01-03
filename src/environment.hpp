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

#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>
#include <map>

    // webdar headers

    /// class environment holds environment variables passed to webdar from its parent process (a shell for example)

class environment
{
public:
    environment() = default;
    environment(const environment & ref) = default;
    environment(environment && ref) noexcept = default;
    environment & operator = (const environment & ref) = default;
    environment & operator = (environment && ref) noexcept = default;
    ~environment() = default;

	/// feed the object with the system provided environment
    void feed(char** env);

	/// provide the value associated to a given environment variable

	/// \param[in] var is the name of the variable to fetch
	/// \param[out] value is the value associated with this value if it exists
	/// \return true if a value is available and the "value" argument is set, if the variable does
	/// not exist in the environment "value" is unchanged and this method returns false
    bool get_value_of(const std::string & var, std::string & value) const;

	/// provide the value associated to a given environment variable or the provided defaulted value

	/// \param[in] var is the name of the variable to fetch
	/// \param[in] defaulted it the default value to return if the variable is not found in the environment
	/// \return the value or default valuer associated to the "var" environment variable
    std::string get_value_with_default(const std::string & var, const std::string & defaulted) const;

private:
    std::map<std::string, std::string> envir;
};

#endif
