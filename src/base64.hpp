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
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "exceptions.hpp"

    /// implements the encoding to and decoding from base64

class base64
{
public:
    base64() {};
    base64(const base64 & ref) = default;
    base64(base64 && ref) noexcept = default;
    base64 & operator = (const base64 & ref) = default;
    base64 & operator = (base64 && ref) noexcept = default;
    ~base64() = default;

    std::string decode(const std::string & str) const;
    std::string encode(const std::string & str) const;

    typedef char decoded_block[3];
    typedef char encoded_block[4];

    void small_decode(const encoded_block & b64, decoded_block & out) const;
    void small_encode(unsigned int num_bytes, const decoded_block & bin, encoded_block & out) const;

private:
    static std::string convert(char a, char b, char c, char d);         ///< output 3 chars of the original binary data
    static std::string unconvert(int num_char,
				 unsigned char a,
				 unsigned char b,
				 unsigned char c); ///< outputs 4 chars in base64
    static unsigned int value64(char a);      ///< convert base64 char to its corresponding value
    static char to_value64(unsigned int val); ///< convert integer < 64 to base64 its char representation
    static void complement(std::string & ref); ///< add '=' bytes at the end to get a length multiple of 4 chars
};

#endif
