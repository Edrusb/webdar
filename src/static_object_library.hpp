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

#ifndef STATIC_OBJECT_LIBRARY_HPP
#define STATIC_OBJECT_LIBRARY_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <map>
#include <string>

    // webdar headers
#include "static_object.hpp"


class static_object_library
{
public:
	/// initialize the library once and for all
    static void init();


	/// operation should held an bug if the library is not frozen
	/// \note throw exception_range if no object can be found under that name
    static const static_object * find_object(const std::string & name);

	/// release all objects added to the library
    static void release();

private:
	/// throw exception if the name already exists
	/// this should held a bug if the library is frozen
	/// \note the given object becomes under the responsibility of the library (will be deleted by it)
    static void add_object_to_library(const std::string & name, static_object * ref);

	/// freeze the library once and for all
    static void freeze_library() { frozen = true; };

	// no locking is required as the library is filled once before
	// multi-thread is started at startup and stays read-only the
	// rest of the time
    static std::map<std::string, static_object *> library;
    static bool frozen;
};

#endif
