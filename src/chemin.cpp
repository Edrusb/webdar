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

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <algorithm>

    // webdar headers
#include "exceptions.hpp"
#include "webdar_tools.hpp"

    //
#include "chemin.hpp"

using namespace std;

chemin::chemin(const string & path)
{
    vector<string> tmp;

    webdar_tools_split_by('/', path, tmp);
    members.clear();
    while(!tmp.empty())
    {
	if(!tmp.back().empty())
	    members.push_front(tmp.back());
	tmp.pop_back();
    }
    index = 0;
}

void chemin::push_back(const std::string & x)
{
	// must avoid adding a string that contains a /
    if(find(x.begin(), x.end(), '/') != x.end())
	throw exception_range("Cannot add a path member if it contains a slash");

    members.push_back(x);
}

void chemin::operator += (const chemin & ref)
{
    deque<string>::const_iterator it = ref.members.begin();

    while(it != ref.members.end())
    {
	members.push_back(*it);
	++it;
    }
}

void chemin::pop_front()
{
    if(empty())
	throw exception_range("cannot pop a member from an empty chemin");

    members.pop_front();
    if(index > 1)
	--(const_cast<chemin *>(this)->index);
}

void chemin::pop_back()
{
    if(empty())
	throw exception_range("cannot pop a member from an empty chemin");

    members.pop_back();
    if(index >= size())
	--(const_cast<chemin *>(this)->index);
}

void chemin::set_index(unsigned int val) const
{
    if(val >= size())
	throw exception_range("invalid index value given to chemin");
    const_cast<chemin *>(this)->index = val;
}

string chemin::display(bool relative) const
{
    string ret = relative ? "" : "/";
    deque<string>::const_iterator it = members.begin();

    if(it != members.end())
    {
	ret += *it;
	++it;
    }

    while(it != members.end())
    {
	ret += "/";
	ret += *it;
	++it;
    }

    return ret;
}

std::string chemin::namify() const
{
    string ret = "";
    deque<string>::const_iterator it = members.begin();

    if(it != members.end())
    {
	ret = *it;
	++it;
    }

    while(it != members.end())
    {
	ret += "_";
	ret += *it;
	++it;
    }

    return ret;
}

const string & chemin::operator[] (unsigned int x) const
{
    if(x >= members.size())
	throw exception_range("out of bound member requested in chemin");

    return members[x];
}


