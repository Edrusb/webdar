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
#include "archive_merge.hpp"

using namespace std;

void archive_merge::set_archive_path(const string & val)
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

void archive_merge::set_archive_reference(const string & refpath,
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

void archive_merge::inherited_run()
{
    try
    {
	shared_ptr<libdar::archive> ref = nullptr;

	    // we must open the archive of reference
	    // and obtain an libdar::archive object to
	    // be added to the options passed to the merge
	    // constructor

	if(!has_ref)
	    throw exception_range("archive of reference not provided for merging");

	if(!ui && !ui->get_user_interaction())
	    throw WEBDAR_BUG;
	ui->get_user_interaction()->message("--- Opening the archive of reference...");
	ref.reset(new (nothrow) libdar::archive(ui->get_user_interaction(),
						libdar::path(ref_path),
						ref_basename,
						ref_extension,
						ref_opt));
	if(!ref)
	    throw exception_memory();

	ui->get_user_interaction()->message("--- The archive of reference is now opened");

	    // now we can merge the archive

	ui->get_user_interaction()->message("--- Proceeding to the merging operation...");
	libdar::archive(ui->get_user_interaction(),
			archpath,
			ref,
			basename,
			extension,
			opt,
			progressive_report);

	    // as the object being local to the local block
	    // it will be destroyed automatically (and the archive
	    // will be closed) once we will have exit this local block

	ui->get_user_interaction()->message("--- Merging operation completed");
    }
    catch(libdar::Egeneric & e)
    {
	throw exception_libcall(e);
    }

}
