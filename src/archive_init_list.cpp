/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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
#include "my_config.h"
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

archive_init_list::archive_init_list(): param(nullptr)
{
#ifdef LIBTHREADAR_STACK_FEATURE
    set_stack_size(DEFAULT_STACK_SIZE);
#endif
}

const vector<libdar::list_entry> archive_init_list::get_children_in_table(const string & dir) const
{
    if(!ptr)
	throw WEBDAR_BUG;
    return ptr->get_children_in_table(dir);
}

bool archive_init_list::has_subdirectory(const string & dir) const
{
    if(!ptr)
	throw WEBDAR_BUG;
    return ptr->has_subdirectory(dir);
}

libdar::archive_summary archive_init_list::get_summary() const
{
    if(!ptr)
	throw WEBDAR_BUG;

    return ptr->summary_data();
}
string archive_init_list::get_archive_full_name() const
{
    if(!ptr)
	throw WEBDAR_BUG;

    if(param == nullptr)
	throw WEBDAR_BUG;

    return (libdar::path(param->get_archive_path()) + param->get_archive_basename()).display();
}

void archive_init_list::inherited_run()
{
    if(ptr)
	throw WEBDAR_BUG;

    try
    {
	if(!ui && ! ui->get_user_interaction())
	    throw WEBDAR_BUG;

	ui->clear();
	ui->auto_hide(false, false);

	if(param == nullptr)
	    throw WEBDAR_BUG;

	libdar::path archpath(param->get_archive_path(), true);
	string basename(param->get_archive_basename());
	libdar::archive_options_read read_opt(param->get_read_options(ui));

	ui->auto_hide(true, true);
	cancellation_checkpoint();
	ptr.reset(new (nothrow) libdar::archive(ui->get_user_interaction(),
						archpath,
						basename,
						EXTENSION,
						read_opt));
	if(!ptr)
	    throw exception_memory();

	try
	{
	    cancellation_checkpoint();

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

void archive_init_list::signaled_inherited_cancel()
{
    pthread_t libdar_tid;
    libdar::thread_cancellation th;

    if(is_running(libdar_tid))
	th.cancel(libdar_tid, true, 0);
}
