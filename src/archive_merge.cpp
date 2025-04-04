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

    //
#include "archive_merge.hpp"

using namespace std;

void archive_merge::inherited_run()
{
    try
    {
	if(!ui && ! ui->get_user_interaction())
	    throw WEBDAR_BUG;

	ui->clear();
	ui->auto_hide(false, false);

	if(param == nullptr)
	    throw WEBDAR_BUG;

	libdar::path ref_path(param->get_archive_path(), true);
	string ref_basename(param->get_archive_basename());
	libdar::archive_options_read ref_opt(param->get_read_options(ui));
	libdar::path archpath(param->get_merge_path());
	string basename(param->get_merge_basename());
	libdar::archive_options_merge opt(param->get_merging_options(ui));
	libdar::statistics* progressive_report(ui->get_statistics().get_libdar_statistics());

	shared_ptr<libdar::archive> ref = nullptr;

	    // we must open the archive of reference
	    // and obtain an libdar::archive object to
	    // be added to the options passed to the merge
	    // constructor

	ui->get_user_interaction()->message("--- Opening the archive of reference...");
	cancellation_checkpoint();
	ref.reset(new (nothrow) libdar::archive(ui->get_user_interaction(),
						ref_path,
						ref_basename,
						EXTENSION,
						ref_opt));
	if(!ref)
	    throw exception_memory();

	ui->get_user_interaction()->message("--- The archive of reference is now opened");

	    // now we can merge the archive

		// resetting counters and logs
	ui->get_statistics().clear_labels();
	ui->get_statistics().set_treated_label("item(s) treated");
	ui->get_statistics().set_hard_links_label("hard link(s) treated");
	ui->get_statistics().set_ignored_label("item(s) ignored (excluded by filters)");
	ui->get_statistics().set_deleted_label("item(s) recorded as deleted");
	ui->get_statistics().set_ea_treated_label("item(s) with Extended Attributes");
	ui->get_statistics().set_total_label("item(s) considered");

	ui->get_user_interaction()->message("--- Proceeding to the merging operation...");
	cancellation_checkpoint();
	libdar::archive(ui->get_user_interaction(),
			archpath,
			ref,
			basename,
			EXTENSION,
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

void archive_merge::signaled_inherited_cancel()
{
    pthread_t libdar_tid;
    libdar::thread_cancellation th;

    if(is_running(libdar_tid))
	th.cancel(libdar_tid, true, 0);
}
