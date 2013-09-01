/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013 Denis Corbin
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

#ifndef DATE_HPP
#define DATE_HPP

    // C system header files
extern "C"
{
#include <time.h>
#include <ctype.h>
}

    // C++ system header files
#include <string>

    // webdar headers


class date
{
public:
	/// constructor, date'value is "now"
    date();

	/// constructor, date is given in one of the following format
	///
	/// \note RFC 1123, RFC 1036 or ANSI C's asctime() formats
    date(const std::string & when);

	/// returns date in RFC 1123 format
    std::string get_canonical_format() const;

    bool operator < (const date & ref) const;
    bool operator <= (const date & ref) const { return *this < ref || *this == ref; };
    bool operator == (const date & ref) const;
private:
    struct tm val;  // date is stored as UTC

};


#endif
