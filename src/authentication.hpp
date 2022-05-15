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

#ifndef AUTHENTICATION_HPP
#define AUTHENTICATION_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers

class authentication
{
public:
    virtual ~authentication() {};

    virtual bool valid_credentials(const std::string & username, const std::string & credential) const = 0;
};



class authentication_unix : public authentication
{
/// A IMPLEMENTER

};

class authentication_cli : public authentication
{
public:
    authentication_cli(const std::string & username, const std::string & password) { user = username; pass = password; };

    virtual bool valid_credentials(const std::string & username, const std::string & credential) const override { return username == user && credential == pass; }; // no need of mutex for this class

private:
    std::string user;
    std::string pass;

};


#endif
