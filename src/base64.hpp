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

#ifndef BASE64_HPP
#define BASE64_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "exceptions.hpp"

class base64
{
public:
    base64(const std::string & str);
    base64(const base64 & ref) = default;
    base64(base64 && ref) noexcept = default;
    base64 & operator = (const base64 & ref) = default;
    base64 & operator = (base64 && ref) noexcept = default;
    ~base64() = default;

    std::string decode() const;
    std::string encode() const { throw exception_feature("base64::encode"); }

private:
    std::string ch;

    static std::string convert(char a, char b, char c, char d);
    static unsigned int value64(char a);
};

#endif
