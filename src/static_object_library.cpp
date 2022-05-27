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

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "tokens.hpp"


    //
#include "static_object_library.hpp"

using namespace std;

bool static_object_library::frozen = false;
map<string, static_object *> static_object_library::library;

void static_object_library::init()
{
    if(frozen)
	throw WEBDAR_BUG;
#include "static_object.sto"
    freeze_library();
}

const static_object * static_object_library::find_object(const string & name)
{
    if(!frozen)
	throw WEBDAR_BUG;

    map<string, static_object *>::iterator it = library.find(name);
    if(it == library.end())
	throw exception_range("unknown static object");

    return it->second;
}

void static_object_library::release()
{
    std::map<string, static_object *>::iterator it = library.begin();

    while(it != library.end())
    {
	if(it->second == nullptr)
	    throw WEBDAR_BUG;

	try
	{
	    delete it->second;
	    it->second = nullptr;
	}
	catch(...)
	{
	    it->second = nullptr;
	    throw;
	}
	++it;
    }

    library.clear();
}

void static_object_library::add_object_to_library(const string & name, static_object *ref)
{
    if(frozen)
	throw WEBDAR_BUG;

    map<string, static_object *>::iterator it = library.find(name);
    if(it != library.end())
	throw WEBDAR_BUG;

    library[name] = ref;
}
