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

#ifndef WEBDAR_TOOLS_HPP
#define WEBDAR_TOOLS_HPP

    // C++ system header files
#include <sstream>
#include <vector>
#include <string>

    // webdar headers
#include "uri.hpp"
#include "webdar_tools.hpp"


template <class T> std::string webdar_tools_convert_to_string(T val)
{
    std::stringstream tmp;
    tmp << val;
    return tmp.str();
}

    /// semi comparison
    ///
    /// \param[in] a first member of the comparison
    /// \param[in] b second member of the comparision
    /// \param[out] result result of the comparision (true if a < b, false if b < a)
    /// \return true if a < b or b < a else false
template <class T> bool webdar_tools_semi_less_than(T a, T b, bool & result)
{
    if(a < b)
    {
	result = true;
	return true;
    }
    if(b < a)
    {
	result = false;
	return true;
    }
    return false; // assu
}

template <class T> void webdar_tools_concat_vectors(std::vector<T> & op1,
						    const std::vector<T> & op2)
{
    for(unsigned int i = 0; i < op2.size(); ++i)
	op1.push_back(op2[i]);
}

extern int webdar_tools_convert_to_int(const std::string & ref);
extern unsigned int webdar_tools_convert_hexa_to_int(const std::string & ref);
extern void webdar_tools_split_by(char sep, const std::string & aggregate, std::vector<std::string> & splitted);
extern void webdar_tools_split_in_two(char sep, const std::string &aggregate, std::string & first, std::string & second);
extern std::string webdar_tools_remove_leading_spaces(const std::string & input);
extern void webdar_tools_init_randomization();

    /// generate a random string composted of letters (lowercase and uppercases) and digits having the first character a letter
    ///
    /// \param[in] size is the size of the string to generate
    /// \return a the quite random string
extern std::string webdar_tools_generate_random_string(unsigned int size);
extern std::string webdar_tools_get_session_ID_from_URI(const uri & url);
extern std::string webdar_tools_to_canonical_case(const std::string & ch);
extern std::string webdar_tools_decode_urlencoded(const std::string & ch);
extern std::string webdar_tools_get_title(const std::string & sessname, const std::string & status);

#endif
