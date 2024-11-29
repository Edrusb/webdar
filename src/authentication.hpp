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

#ifndef AUTHENTICATION_HPP
#define AUTHENTICATION_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers

    /// pure virtual class defining the interface for current and future authentication methods

class authentication
{
public:
    authentication() = default;
    authentication(const authentication & ref) = default;
    authentication(authentication && ref) noexcept = default;
    authentication & operator = (const authentication & ref) = default;
    authentication & operator = (authentication && ref) noexcept = default;
    virtual ~authentication() {};

    virtual bool valid_credentials(const std::string & username, const std::string & credential) const = 0;
};

    /// place holder class for Unix PAM authentication (not implemented for now)

class authentication_unix : public authentication
{
/// A IMPLEMENTER

};

    /// authentication_cli implements an authentication method based on a fixed login and password

class authentication_cli : public authentication
{
public:
    authentication_cli(const std::string & username, const std::string & password) { user = username; pass = password; };
    authentication_cli(const authentication_cli & ref) = default;
    authentication_cli(authentication_cli && ref) noexcept = default;
    authentication_cli & operator = (const authentication_cli & ref) = default;
    authentication_cli & operator = (authentication_cli && ref) noexcept = default;
    ~authentication_cli() = default;

    virtual bool valid_credentials(const std::string & username, const std::string & credential) const override { return username == user && credential == pass; }; // no need of mutex for this class

private:
    std::string user;
    std::string pass;

};


#endif
