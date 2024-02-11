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
#include "archive_create.hpp"

using namespace std;

void archive_create::set_archive_path(const string & val)
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

void archive_create::set_fs_root(const string & val)
{
    try
    {
	fs_root = libdar::path(val, true);
    }
    catch(libdar::Egeneric & e)
    {
	throw exception_libcall(e);
    }
}

void archive_create::inherited_run()
{
    try
    {
	shared_ptr<libdar::archive> ref = nullptr;

	    // in case of differential / incremental backup
	    // we must open the archive of reference
	    // and obtain an libdar::archive object to
	    // be added to the options passed to the create
	    // constructor

	if(!ui)
	    throw WEBDAR_BUG;

	    // let's now create the archive

	libdar::archive target(ui->get_user_interaction(),
			       fs_root,
			       archpath,
			       basename,
			       extension,
			       opt,
			       progressive_report);

	    // as the object being local to the local block
	    // it will be destroyed automatically (and the archive
	    // will be closed) once we will have exit this local block
    }
    catch(libdar::Egeneric & e)
    {
	throw exception_libcall(e);
    }
}
