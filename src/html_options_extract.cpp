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
#include "html_options_extract.hpp"

using namespace std;

html_options_extract::html_options_extract():
    form("Update Options"),
    fs("Restoration options"),
    warn_over("Warn before overwriting",
	      html_form_input::check,
	      "",
	      0),
    info_details("Detailed informations",
		 html_form_input::check,
		 "",
		 0),
    flat("Do not restore directory structure",
	 html_form_input::check,
	 "",
	 0),
    what_to_check("Fields to restore"),
    warn_remove_no_match("Warn if a file to remove does not match expected type",
			 html_form_input::check,
			 "",
			 0),
    empty("Dry-run execution",
	  html_form_input::check,
	  "",
	  0),
    display_skipped("Display skipped files",
		    html_form_input::check,
		    "",
		    0),
    empty_dir("Restore even empty directories",
	      html_form_input::check,
	      "",
	      0),
    dirty_behavior("Dirty file behavior"),
    only_deleted("Restore only deleted files",
		 html_form_input::check,
		 "",
		 0),
    ignore_deleted("Avoid removing files flagged as to be removed",
		   html_form_input::check,
		   "",
		   0)
{
    libdar::archive_options_extract defaults;

    warn_over.set_value_as_bool(defaults.get_warn_over());
    info_details.set_value_as_bool(defaults.get_info_details());
    flat.set_value_as_bool(defaults.get_flat());
    what_to_check.add_choice("all", "All");
    what_to_check.add_choice("ignore_owner", "All but ownership");
    what_to_check.add_choice("mtime", "All but ownership and permission");
    what_to_check.add_choice("inode_type", "Nothing inode related, just data");
    warn_remove_no_match.set_value_as_bool(defaults.get_warn_remove_no_match());
    empty.set_value_as_bool(defaults.get_empty());
    display_skipped.set_value_as_bool(defaults.get_display_skipped());
    dirty_behavior.add_choice("ignore", "Exclude dirty files");
    dirty_behavior.add_choice("warn", "Warn before restoring");
    dirty_behavior.add_choice("ok", "Restored as normal files");
    switch(defaults.get_dirty_behavior())
    {
    case libdar::archive_options_extract::dirty_ignore:
	dirty_behavior.set_selected(0);
	break;
    case libdar::archive_options_extract::dirty_warn:
	dirty_behavior.set_selected(1);
	break;
    case libdar::archive_options_extract::dirty_ok:
	dirty_behavior.set_selected(2);
	break;
    default:
	throw WEBDAR_BUG;
    }

    only_deleted.set_value_as_bool(defaults.get_only_deleted());
    ignore_deleted.set_value_as_bool(defaults.get_ignore_deleted());

    fs.adopt(&warn_over);
    fs.adopt(&info_details);
    fs.adopt(&flat);
    fs.adopt(&what_to_check);
    fs.adopt(&warn_remove_no_match);
    fs.adopt(&empty);
    fs.adopt(&display_skipped);
    fs.adopt(&empty_dir);
    fs.adopt(&dirty_behavior);
    fs.adopt(&only_deleted);
    fs.adopt(&ignore_deleted);
    form.adopt(&fs);
    adopt(&form);
}

libdar::archive_options_extract html_options_extract::get_options() const
{
    libdar::archive_options_extract ret;

    ret.set_warn_over(warn_over.get_value_as_bool());
    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_flat(flat.get_value_as_bool());
    if(what_to_check.get_selected_id() == "all")
	ret.set_what_to_check(libdar::comparison_fields::all);
    else if(what_to_check.get_selected_id() == "ignore_owner")
	ret.set_what_to_check(libdar::comparison_fields::ignore_owner);
    else if(what_to_check.get_selected_id() == "mtime")
	ret.set_what_to_check(libdar::comparison_fields::mtime);
    else if(what_to_check.get_selected_id() == "inode_type")
	ret.set_what_to_check(libdar::comparison_fields::inode_type);
    else
	throw WEBDAR_BUG;
    ret.set_warn_remove_no_match(warn_remove_no_match.get_value_as_bool());
    ret.set_empty(empty.get_value_as_bool());
    ret.set_display_skipped(display_skipped.get_value_as_bool());
    if(dirty_behavior.get_selected_id() == "ignore")
	ret.set_dirty_behavior(libdar::archive_options_extract::dirty_ignore);
    else if(dirty_behavior.get_selected_id() == "warn")
	ret.set_dirty_behavior(libdar::archive_options_extract::dirty_warn);
    else if(dirty_behavior.get_selected_id() == "ok")
	ret.set_dirty_behavior(libdar::archive_options_extract::dirty_ok);
    else
	throw WEBDAR_BUG;
    ret.set_only_deleted(only_deleted.get_value_as_bool());
    ret.set_ignore_deleted(ignore_deleted.get_value_as_bool());

    return ret;
}


std::string html_options_extract::inherited_get_body_part(const chemin & path,
							  const request & req)
{
    return get_body_part_from_all_children(path, req);
}

