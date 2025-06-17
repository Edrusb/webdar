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
#include "archive_compare.hpp"

using namespace std;

archive_compare::archive_compare(): param(nullptr)
{
#ifdef LIBTHREADAR_STACK_FEATURE
    set_stack_size(DEFAULT_STACK_SIZE);
#endif
}

void archive_compare::inherited_run()
{
    try
    {
	    // we create a local libdar::archive
	    // it will be destroyed exiting this try/catch block

	if(!ui && ! ui->get_user_interaction())
	    throw WEBDAR_BUG;

	ui->clear();
	ui->auto_hide(false, false);

	if(param == nullptr)
	    throw WEBDAR_BUG;

	libdar::path archpath(param->get_archive_path(), true);
	string basename(param->get_archive_basename());
	libdar::path fs_root(param->get_fs_root(), true);
	libdar::archive_options_read read_opt(param->get_read_options(ui));
	libdar::archive_options_diff diff_opt(param->get_comparison_options());
	libdar::statistics* progressive_report = ui->get_statistics().get_libdar_statistics();

	cancellation_checkpoint();
	libdar::archive arch(ui->get_user_interaction(),
			     archpath,
			     basename,
			     EXTENSION,
			     read_opt);

	    // if no exception has been raised, the object
	    // has been created so we can compare

		// restting counters and logs
	ui->clear();
	ui->get_statistics().clear_labels();
	ui->get_statistics().set_treated_label("item(s) identical");
	ui->get_statistics().set_errored_label("item(s) do not match those on file system");
	ui->get_statistics().set_ignored_label("item(s) ignored (excluded by filters)");
	ui->get_statistics().set_total_label("inode(s) considered");

	cancellation_checkpoint();
	libdar::statistics final = arch.op_diff(fs_root,
						diff_opt,
						progressive_report);
    }
    catch(libdar::Egeneric & e)
    {
	    // converting libdar exceptions to exception_libcall
	    // used within webdar
	throw exception_libcall(e);
    }
}

void archive_compare::signaled_inherited_cancel()
{
    pthread_t libdar_tid;
    libdar::thread_cancellation th;

    if(is_running(libdar_tid))
	th.cancel(libdar_tid, true, 0);
}
