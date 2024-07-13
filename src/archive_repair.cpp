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
#include "exceptions.hpp"

    //
#include "archive_repair.hpp"

using namespace std;

void archive_repair::inherited_run()
{
    try
    {
	shared_ptr<libdar::archive> ref = nullptr;

	    // in case of differential / incremental backup
	    // we must open the archive of reference
	    // and obtain an libdar::archive object to
	    // be added to the options passed to the repair
	    // constructor

	if(!ui && ! ui->get_user_interaction())
	    throw WEBDAR_BUG;

	ui->clear();
	ui->auto_hide(false, false);

	if(param == nullptr)
	    throw WEBDAR_BUG;

	libdar::path ref_path(param->get_archive_path(), true);
	string ref_basename(param->get_archive_basename());
	libdar::archive_options_read read_opt(param->get_read_options(ui));
	libdar::path archpath(param->get_repairing_path());
	string basename(param->get_repairing_basename());
	libdar::archive_options_repair repair_opt(param->get_repairing_options(ui));
	libdar::statistics* progressive_report = ui->get_statistics().get_libdar_statistics();

	    // resetting counters and logs
	ui->get_statistics().clear_counters();
	ui->get_statistics().clear_labels();
	ui->get_statistics().set_treated_label("item(s) treated");
	ui->get_statistics().set_hard_links_label("hard link(s) treated");
	ui->get_statistics().set_byte_amount_label("byte(s) wasted due to changing files at the time they were read");
	ui->get_statistics().set_errored_label("items(s) with error (filesystem error)");
	ui->get_statistics().set_deleted_label("item(s) recorded as deleted");
	ui->get_statistics().set_ea_treated_label("item(s) with Extended Attributes");

	    // let's now repair the archive

	cancellation_checkpoint();
	libdar::archive target(ui->get_user_interaction(),
			       ref_path,
			       ref_basename,
			       EXTENSION,
			       read_opt,
			       archpath,
			       basename,
			       EXTENSION,
			       repair_opt,
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

void archive_repair::signaled_inherited_cancel()
{
    pthread_t libdar_tid;
    libdar::thread_cancellation th;

    if(is_running(libdar_tid))
	th.cancel(libdar_tid, true, 0);
}
