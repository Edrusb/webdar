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
#include "exceptions.hpp"

    //
#include "archive_compare.hpp"

using namespace std;


archive_compare::archive_compare():
    archpath("/"),
    fs_root("/")
{
    progressive_report = nullptr;
}

void archive_compare::set_archive_path(const std::string & val)
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

void archive_compare::set_fs_root(const std::string & val)
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

void archive_compare::inherited_run()
{
    try
    {
	libdar::archive arch(ui,
			     archpath,
			     basename,
			     EXTENSION,
			     read_opt);

	libdar::statistics final = arch.op_diff(fs_root,
						diff_opt,
						progressive_report);
    }
    catch(libdar::Egeneric & e)
    {
	throw exception_libcall(e);
    }
}
