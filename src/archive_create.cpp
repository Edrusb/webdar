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
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "tokens.hpp"
#include "exceptions.hpp"

    //
#include "archive_create.hpp"

using namespace std;

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

	if(!ui && ! ui->get_user_interaction())
	    throw WEBDAR_BUG;

	ui->clear();
	ui->auto_hide(false, false);

	if(param == nullptr)
	    throw WEBDAR_BUG;

	libdar::path archpath(param->get_archive_path(), true);
	string basename(param->get_archive_basename());
	libdar::path fs_root(param->get_fs_root(), true);
	libdar::archive_options_create opt(param->get_creating_options(ui));
	libdar::statistics* progressive_report = ui->get_statistics().get_libdar_statistics();

	    // resetting counters and logs
	ui->get_statistics().clear_counters();
	ui->get_statistics().clear_labels();
	ui->get_statistics().set_treated_label("item(s) treated");
	ui->get_statistics().set_hard_links_label("hard link(s) treated");
	ui->get_statistics().set_tooold_label("item(s) modified while read for backup (dirty files)");
	ui->get_statistics().set_byte_amount_label("byte(s) wasted due to changing files at the time they were read");
	ui->get_statistics().set_skipped_label("item(s) not saved (no inode/file change)");
	ui->get_statistics().set_errored_label("items(s) with error (file system error)");
	ui->get_statistics().set_ignored_label("item(s) ignored (excluded by filters)");
	ui->get_statistics().set_deleted_label("item(s) recorded as deleted");
	ui->get_statistics().set_ea_treated_label("item(s) with Extended Attributes");

	    // let's now create the archive

	cancellation_checkpoint();
	libdar::archive target(ui->get_user_interaction(),
			       fs_root,
			       archpath,
			       basename,
			       EXTENSION,
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

void archive_create::signaled_inherited_cancel()
{
    pthread_t libdar_tid;
    libdar::thread_cancellation th;

    if(is_running(libdar_tid))
	th.cancel(libdar_tid, true, 0);
}
