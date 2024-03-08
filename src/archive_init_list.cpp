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


    // webdar headers
#include "tokens.hpp"
#include "exceptions.hpp"

    //
#include "archive_init_list.hpp"

using namespace std;

const vector<libdar::list_entry> archive_init_list::get_children_in_table(const std::string & dir) const
{
    if(!ptr)
	throw WEBDAR_BUG;
    return ptr->get_children_in_table(dir);
}

bool archive_init_list::has_subdirectory(const std::string & dir) const
{
    if(!ptr)
	throw WEBDAR_BUG;
    return ptr->has_subdirectory(dir);
}

void archive_init_list::inherited_run()
{
    if(!ptr)
	throw WEBDAR_BUG;

    try
    {
	if(!ui)
	    throw WEBDAR_BUG;

	if(param == nullptr)
	    throw WEBDAR_BUG;

	ui->get_statistics().clear_counters();
	ui->get_statistics().clear_labels();

	libdar::path archpath(param->get_archive_path(), true);
	std::string basename(param->get_archive_basename());
	libdar::archive_options_read read_opt(param->get_read_options(ui));

	ui->auto_hide(true, true);
	ptr.reset(new (nothrow) libdar::archive(ui->get_user_interaction(),
						archpath,
						basename,
						EXTENSION,
						read_opt));
	if(!ptr)
	    throw exception_memory();

	try
	{
		// this is necessary before calling  get_children_in_table
	    ptr->init_catalogue();
	}
	catch(...)
	{
	    ptr.reset();
	    throw;
	}
    }
    catch(libdar::Egeneric & e)
    {
	if(ptr)
	    throw WEBDAR_BUG;
	throw exception_libcall(e);
    }
}
