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

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "tokens.hpp"


    //
#include "archive_init_list.hpp"

using namespace std;

void archive_init_list::set_archive_path(const std::string & val)
{
    try
    {
	archpath = libdar::path(val, true);
    }
    catch(libdar::Egeneric & e)
    {
	throw exception_libcall(e);
    }
}

const vector<libdar::list_entry> archive_init_list::get_children_in_table(const std::string & dir) const
{
    if(ptr == NULL)
	throw WEBDAR_BUG;
    return ptr->get_children_in_table(dir);
}

bool archive_init_list::has_subdirectory(const std::string & dir) const
{
    if(ptr == NULL)
	throw WEBDAR_BUG;
    return ptr->has_subdirectory(dir);
}

void archive_init_list::inherited_run()
{
    if(ptr != NULL)
	throw WEBDAR_BUG;

    try
    {
	ptr = new (nothrow) libdar::archive(ui,
					    archpath,
					    basename,
					    EXTENSION,
					    read_opt);
	if(ptr == NULL)
	    throw exception_memory();

	try
	{
		// this is necessary before calling  get_children_in_table
	    ptr->init_catalogue(ui);
	}
	catch(...)
	{
	    delete ptr;
	    ptr = NULL;
	    throw;
	}
    }
    catch(libdar::Egeneric & e)
    {
	if(ptr != NULL)
	    throw WEBDAR_BUG;
	throw exception_libcall(e);
    }
}

