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

    //
#include "archive_isolate.hpp"

using namespace std;

void archive_isolate::set_archive_path(const string & val)
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

void archive_isolate::set_archive_reference(const string & refpath,
					    const string & basename,
					    const string & extension,
					    const libdar::archive_options_read & readopt)
{
    has_ref = true;
    ref_path = refpath;
    ref_basename = basename;
    ref_extension = extension;
    ref_opt = readopt;
}

void archive_isolate::inherited_run()
{
    try
    {

	    // we must open the archive of reference
	    // and obtain an libdar::archive object to
	    // be added to the options passed to the isolate
	    // constructor

	if(!has_ref)
	    throw WEBDAR_BUG;

	libdar::archive* ref = new (nothrow) libdar::archive(ui,
							     libdar::path(ref_path),
							     ref_basename,
							     ref_extension,
							     ref_opt);
	if(ref == nullptr)
	    throw exception_memory();

	try
	{

	    // now we can isolate the archive

	    ref->op_isolate(archpath,
			    basename,
			    extension,
			    opt);

		// as the ref object being local to the local block
		// it will be destroyed automatically (and the archive
		// will be closed) once we will have exit this local block
	}
	catch(...)
	{
	    delete ref;
	    throw;
	}
	delete ref;
    }
    catch(libdar::Egeneric & e)
    {
	throw exception_libcall(e);
    }
}

