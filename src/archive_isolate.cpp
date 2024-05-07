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
#include "tokens.hpp"

    //
#include "archive_isolate.hpp"

using namespace std;

void archive_isolate::inherited_run()
{
    try
    {
	shared_ptr<libdar::archive> source;

	if(!ui && ! ui->get_user_interaction())
	    throw WEBDAR_BUG;

	if(param == nullptr)
	    throw WEBDAR_BUG;

	libdar::path archpath(param->get_archive_path(), true);
	string basename(param->get_archive_basename());
	libdar::archive_options_read read_opt(param->get_read_options(ui));
	libdar::path dest_path(param->get_isolating_path(), true);
	string dest_basename(param->get_isolating_basename());
	libdar::archive_options_isolate isol_opt(param->get_isolating_options(ui));

	    // first creating the archive object to isolate

	cancellation_checkpoint();
	source.reset(new (nothrow) libdar::archive(ui->get_user_interaction(),
						   archpath,
						   basename,
						   EXTENSION,
						   read_opt));

	if(!source)
	    throw exception_memory();


	    // resetting counters and logs
	ui->get_statistics().clear_counters();
	ui->get_statistics().clear_labels();


	    // now we can isolate the archive

	cancellation_checkpoint();
	source->op_isolate(dest_path,
			   dest_basename,
			   EXTENSION,
			   isol_opt);

	    // as the ref object being local to the local block
	    // it will be destroyed automatically (and the archive
	    // will be closed) once we will have exit this local block
    }
    catch(libdar::Egeneric & e)
    {
	throw exception_libcall(e);
    }
}


void archive_isolate::signaled_inherited_cancel()
{
    pthread_t libdar_tid;
    libdar::thread_cancellation th;

    if(is_running(libdar_tid))
	th.cancel(libdar_tid, true, 0);
}
