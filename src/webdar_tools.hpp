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

#include "my_config.h"

    // C++ system header files
#include <sstream>
#include <vector>
#include <deque>
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
    /// \return true if either a < b or b < a, else false is returned and 'result' is
    ///  undefined
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
    return false;
}

template <class T> void webdar_tools_concat_vectors(std::vector<T> & op1,
						    const std::vector<T> & op2)
{
    for(unsigned int i = 0; i < op2.size(); ++i)
	op1.push_back(op2[i]);
}

template <class T> T webdar_tools_convert_from_infinint(const libdar::infinint & val, const std::string & conv_err_msg)
{
    T ret = 0;
    libdar::infinint tmp(val);

    tmp.unstack(ret);
    if(! tmp.is_zero())
    {
	if(conv_err_msg.empty())
	    throw WEBDAR_BUG;
	else
	    throw exception_range(conv_err_msg);
    }

    return ret;
}

    /// defines a substring portion of a existing string (to avoid data copy)

    /// \note as these are iterators on an existing string, these interator should
    /// not be used after this string they refer is changed or deleted
struct troncon
{
    std::string::const_iterator begin;
    std::string::const_iterator end;

    troncon() {}; // needed to use troncon in std::map
    troncon(std::string::const_iterator b, std::string::const_iterator e) { begin = b; end = e; };
    troncon(const std::string & ref) { begin = ref.begin(); end = ref.end(); };

    bool operator < (const troncon & ref) const;
};

extern int webdar_tools_convert_to_int(const std::string & ref);
extern unsigned int webdar_tools_convert_hexa_to_int(const std::string & ref);
extern void webdar_tools_split_by(char sep, const std::string & aggregate, std::vector<std::string> & splitted);
extern void webdar_tools_split_in_two(char sep, const std::string &aggregate, std::string & first, std::string & second);

    /// return the list of sequences found in aggregate that are separated by substring delimiter

    /// \param[in] substring the string that should be seen as delimiter and not included in the returned list
    /// \param[in] aggregate the string portion to analyse
    /// \note if the aggregate string portion starts with a substring, the first element of the deque is an empty portion
    /// (troncon.begin = troncon.end) same thing if the aggregate string ends by a substring the last element of the deque
    /// is also an empty portion.
extern std::deque<troncon> webdar_tools_split_by_substring(const std::string & substring,
							   const troncon & aggregate);

    /// provides the offset of the first byte of the first occurrence of substring found in the aggregate string portion

    /// \note if no substring could be found, this call returns aggregate.end
extern std::string::const_iterator webdar_tools_seek_to_substring(const std::string & substring, const troncon & aggregate);

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

extern std::string webdar_tools_html_display(const std::string & arg);

extern bool webdar_tools_exists_and_is_file(const std::string & path, bool follow_symlink);
extern bool webdar_tools_exists_and_is_dir(const std::string & path, bool follow_symlink);

#endif
