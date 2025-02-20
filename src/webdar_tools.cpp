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

#include "my_config.h"
extern "C"
{
#if HAVE_CTYPE_H
#include <ctype.h>
#endif

#if HAVE_TIME_H
#include <time.h>
#endif

#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#if HAVE_FCNTL_H
#include <fcntl.h>
#endif
}

    // libraries header files
#include <dar/libdar.hpp>

    // C++ system header files
#include <iostream>

    // webdar headers
#include "webdar_tools.hpp"
#include "exceptions.hpp"

using namespace std;

bool troncon::operator < (const troncon & ref) const
{
    string::const_iterator itme = begin;
    string::const_iterator itref = ref.begin;

    while(itme != end && itref != ref.end && *itme == *itref)
    {
	++itme;
	++itref;
    }

    if(itme != end)
    {
	if(itref != ref.end)
	    return *itme < *itref;
	else
	    return false; // ref shorter string than me
    }
    else
    {
	if(itref != ref.end)
	    return true;  // we are a shorter string than ref
	else
	    return false; // we are the exact same string as ref
    }
}

int webdar_tools_convert_to_int(const string & ref)
{
    int ret = -1;
    stringstream buf;

    buf.str(ref);
    buf >> ret;
    if(buf.rdstate() != ios_base::goodbit && buf.rdstate() != ios_base::eofbit)
	throw exception_range(string("Cannot convert ") + ref + string(" to integer"));

    return ret;
}

void webdar_tools_split_by(char sep, const string & aggregate, vector<string> & splitted)
{
    splitted.clear();

    string::const_iterator it = aggregate.begin();
    string::const_iterator bk = it;

    while(it != aggregate.end())
    {
	if(*it == sep)
	{
	    splitted.push_back(string(bk, it));
	    ++it;
	    bk = it;
	}
	else
	    ++it;
    }

    if(bk != aggregate.end())
	splitted.push_back(string(bk, it));
}


void webdar_tools_split_in_two(char sep, const string &aggregate, string & first, string & second)
{
    string::const_iterator it = aggregate.begin();

    while(it != aggregate.end() && *it != sep)
	++it;

    first = string(aggregate.begin(), it);
    if(it != aggregate.end())
	++it;
    second = string(it, aggregate.end());
}

deque<troncon> webdar_tools_split_by_substring(const string & substring,
					       const troncon & aggregate)
{
    deque<troncon> ret;
    troncon tronc = aggregate;
    string::const_iterator first_match;

    do
    {
	first_match = webdar_tools_seek_to_substring(substring, tronc);
	if(first_match != tronc.end)
	{

	       // fully matching string

	    ret.push_back(troncon(tronc.begin, first_match)); // may push a empty troncon / string
	    tronc.begin = first_match + substring.size();
	}
    }
    while(first_match != tronc.end);

    ret.push_back(tronc);

    return ret;
}

string::const_iterator webdar_tools_seek_to_substring(const string & substring, const troncon & aggregate)
{
    string::const_iterator ret = aggregate.begin;
    string::const_iterator offset;
    string::const_iterator suit = substring.begin();
    bool matching = false;
    bool found = false;

    while(ret != aggregate.end && ! found)
    {
	if(!matching)
	{
	    if(*ret == *suit)
	    {
		matching = true;
		offset = ret;
		++ret;
		++suit;
	    }
	    else
		++ret;
	}
	else // currently processing a matching substring
	{
	    if(suit != substring.end()) // still not complete match
	    {
		if(*ret != *suit) // failed fully matching substring
		{
		    matching = false;
		    suit = substring.begin();
		    ret = offset + 1; // yes, we may have portition of substring inside substring
		}
		else
		{
		    ++ret;
		    ++suit;
		}
	    }
	    else // fully matching string
		found = true;
	}
    }

    return found ? offset : ret;
}

string webdar_tools_remove_leading_spaces(const string & input)
{
    string::const_iterator it = input.begin();

    while(it != input.end() && *it == ' ')
	++it;

    return string(it, input.end());
}

void webdar_tools_init_randomization()
{
    int pas = getpid() % getppid();
    srand(time(nullptr));

    if(pas == 0)
	pas = 10;
    for(int i = getpid() ; i > pas ; i -= pas)
	(void)rand();
}

string webdar_tools_generate_random_string(unsigned int size)
{
    string ret = "";
    unsigned int x;

    for(unsigned int i = 0; i < size; ++i)
    {
	if(i == 0)
	    x = (rand() % 52) + 10;
	else
	    x = rand() % 62;

	if(x < 10)
	    ret += char(x + 48); // digits 0 - 9
	else
	    if(x < 36) // 10 <= x < 36
		ret += char(55 + x); // uppercase letters A - Z
	    else  // 36 <= x < 61
		ret += char(61 + x); // lowercase letters a - z
    }

    return ret;
}

string webdar_tools_get_session_ID_from_URI(const uri & url)
{
    const chemin & path = url.get_path();

    if(!path.empty())
	return path[0];
    else
	return "";
}


string webdar_tools_to_canonical_case(const string & ch)
{
    string ret = "";
    bool previous_is_alpha = false;

    for(string::const_iterator it = ch.begin(); it != ch.end(); ++it)
    {
	if(isalpha((*it)))
	{
	    if(previous_is_alpha)
		ret += tolower(*it);
	    else
	    {
		ret+= toupper(*it);
		previous_is_alpha = true;
	    }
	}
	else
	{
	    ret += *it;
	    previous_is_alpha = false;
	}
    }

    return ret;
}

string webdar_tools_decode_urlencoded(const string & ch)
{
    string ret = "";
    string::const_iterator it = ch.begin();
    string::const_iterator bk = it;
    char x;

    while(it != ch.end())
    {
	switch(*it)
	{
	case '+':
	    ret += string(bk, it) + " ";
	    ++it;
	    bk = it;
	    break;
	case '%':
	    ret += string(bk, it);
	    ++it;
	    bk = it;
	    if(it != ch.end())
	    {
		++it;
		if(it != ch.end())
		{
		    ++it;
		    x = webdar_tools_convert_hexa_to_int(string(bk, it));
		    ret += x;
		}
		bk = it;
	    }
	    break;
	default:
	    ++it;
	}
    }

    ret += string(bk, it);

    return ret;
}


unsigned int webdar_tools_convert_hexa_to_int(const string & ref)
{
    unsigned int ret = 0;
    unsigned int val = 0;
    string::const_iterator it = ref.begin();

    while(it != ref.end())
    {
	if(*it >= '0' && *it <= '9')
	    val = *it - '0';
	else if(*it >= 'a' && *it <= 'f')
	    val = *it - 'a' + 10;
	else if(*it >= 'A' && *it <= 'F')
	    val = *it - 'A' + 10;
	else
	    throw exception_range("string does not represent an hexadecimal number");

	ret *= 16;
	ret += val;
	++it;
    }

    return ret;
}

string webdar_tools_get_title(const string & sessname, const string & status)
{
    string ret = "Webdar - ";
    if(sessname == "")
	ret += status;
    else
	ret += sessname + " - " + status;

    return ret;
}


string webdar_tools_html_display(const string & arg)
{
    string ret;
    string::const_iterator it = arg.begin();

    while(it != arg.end())
    {
	switch(*it)
	{
	case '<':
	    ret += "&lt;";
	    break;
	case '>':
	    ret += "&gt";
	    break;
	default:
	    ret += *it;
	}
	++it;
    }

    return ret;
}

bool webdar_tools_exists_and_is_file(const std::string & path, bool follow_symlink)
{
    struct stat info;
    int ret;

    if(follow_symlink)
	ret = stat(path.c_str(), & info);
    else
	ret = lstat(path.c_str(), & info);

    return ret == 0 && (info.st_mode & S_IFREG) != 0;
}

bool webdar_tools_exists_and_is_dir(const std::string & path, bool follow_symlink)
{
    struct stat info;
    int ret;

    if(follow_symlink)
	ret = stat(path.c_str(), & info);
    else
	ret = lstat(path.c_str(), & info);

    return ret == 0 && (info.st_mode & S_IFDIR) != 0;
}
