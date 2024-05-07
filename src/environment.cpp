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

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "webdar_tools.hpp"
#include "exceptions.hpp"

    //
#include "environment.hpp"


using namespace std;


void environment::feed(char** env)
{
    char** ptr = nullptr;
    string tmp;
    string var, val;

    if(env == nullptr)
	throw exception_range("empty environment variable set provided");

    envir.clear();
    ptr = env;

    while(*ptr != nullptr)
    {
	webdar_tools_split_in_two('=', *ptr, var, val);
	envir[var] = val;
	++ptr;
    }
}

bool environment::get_value_of(const string & var, string & value) const
{
    map<string, string>::const_iterator it = envir.find(var);

    if(it == envir.end())
	return false;
    else
    {
	value = it->second;
	return true;
    }
}

string environment::get_value_with_default(const string & var, const string & defaulted) const
{
    string ret;

    if(! get_value_of(var, ret))
	ret = defaulted;

    return ret;
}



environment global_envir;
