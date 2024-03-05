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
#include "archive_restore.hpp"

using namespace std;


archive_restore::archive_restore():
    progressive_report(nullptr),
    param(nullptr),
    archpath("/"),
    fs_root("/")
{
}

void archive_restore::inherited_run()
{
    try
    {
	if(!ui)
	    throw WEBDAR_BUG;
	if(param == nullptr)
	    throw WEBDAR_BUG;

	try
	{
	    archpath = libdar::path(param->get_archive_path(), true);
	}
	catch(libdar::Egeneric & e)
	{
	    throw exception_libcall(e);
	}

	try
	{
	    fs_root = libdar::path(param->get_fs_root(), true);
	}
	catch(libdar::Egeneric & e)
	{
	    throw exception_libcall(e);
	}

	basename = param->get_archive_basename();
	extract_opt = param->get_extraction_options();
	read_opt = param->get_read_options(ui);    ///< this lead to subthread creation to get entrepot object and clears the web_ui interface

	libdar::archive arch(ui->get_user_interaction(),
			     archpath,
			     basename,
			     EXTENSION,
			     read_opt);

	    // restting counters and logs
	ui->clear();
	ui->get_statistics().clear_counters();
	ui->get_statistics().clear_labels();
	ui->get_statistics().set_treated_label("item(s) restored");
	ui->get_statistics().set_skipped_label("item(s) not restored (not saved in archive)");
	ui->get_statistics().set_tooold_label("item(s) not restored (overwriting policy decision)");
	ui->get_statistics().set_errored_label("item(s) failed to restore (filesystem error)");
	ui->get_statistics().set_ignored_label("item(s) ignored (excluded by filters)");
	ui->get_statistics().set_hard_links_label("hard link(s) restored");
	ui->get_statistics().set_ea_treated_label("item(s) having their EA restored");
	ui->get_statistics().set_total_label("item(s) considered");
	progressive_report = ui->get_statistics().get_libdar_statistics();

	libdar::statistics final = arch.op_extract(fs_root,
						   extract_opt,
						   progressive_report);
    }
    catch(libdar::Egeneric & e)
    {
	throw exception_libcall(e);
    }
}
