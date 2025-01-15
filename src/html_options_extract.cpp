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
#include "webdar_css_style.hpp"
#include "html_form_mask_expression.hpp"

    //
#include "html_options_extract.hpp"

using namespace std;

html_options_extract::html_options_extract():
    form_archgen("Update"),
    fs(""),
    in_place("Ignore the provided root directory above and use instead the \"in place\" directory information stored in the archive",
	     html_form_input::check,
	     "",
	     "1"),
    warn_over("Warn before overwriting",
	      html_form_input::check,
	      "1",
	      "1"),
    flat("Do not restore directory structure",
	 html_form_input::check,
	 "",
	 "1"),
    what_to_check("Fields to restore"),
    warn_remove_no_match("Warn if a file to remove does not match expected type",
			 html_form_input::check,
			 "",
			 "1"),
    empty("Dry-run execution",
	  html_form_input::check,
	  "",
	  "1"),
    form_show("Update"),
    fs_show(""),
    info_details("Detailed informations",
		 html_form_input::check,
		 "1",
		 "1"),
    display_treated("Display treated files",
		    html_form_input::check,
		    "1",
		    "1"),
    display_treated_only_dir("Display only treated directories",
			     html_form_input::check,
			     "",
			     "1"),
    display_skipped("Display skipped files",
		    html_form_input::check,
		    "1",
		    "1"),
    empty_dir("Restore even empty directories",
	      html_form_input::check,
	      "1",
	      "1"),
    dirty_behavior("Dirty file behavior"),
    ignore_sockets("Do not restore Unix sockets",
		   html_form_input::check,
		   "",
		   "1"),
    overwriting_policy(""),
    form_overwriting("Update"),
    only_deleted("Restore only deleted files",
		 html_form_input::check,
		 "",
		 "1"),
    ignore_deleted("Avoid removing files flagged as to be removed",
		   html_form_input::check,
		   "",
		   "1")
{
	// set html fields to default value used by libdar

    libdar::archive_options_extract defaults;

    filename_mask.reset(new (nothrow) html_mask_form_filename("file name"));
    if(!filename_mask)
	throw exception_memory();

    ea_mask.reset(new (nothrow) html_mask_form_filename("extended attribute"));
    if(!ea_mask)
	throw exception_memory();

    path_mask.reset(new (nothrow) html_mask_form_path(true));
    if(!path_mask)
	throw exception_memory();

    in_place.set_value_as_bool(defaults.get_in_place());
    flat.set_value_as_bool(defaults.get_flat());
    what_to_check.add_choice("all", "All");
    what_to_check.add_choice("ignore_owner", "All but ownership");
    what_to_check.add_choice("mtime", "All but ownership and permission");
    what_to_check.add_choice("inode_type", "Nothing inode related, just data");
    warn_remove_no_match.set_value_as_bool(defaults.get_warn_remove_no_match());
    empty.set_value_as_bool(defaults.get_empty());
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
    ignore_sockets.set_value_as_bool(defaults.get_ignore_unix_sockets());

	// build the adoption tree

    static const char* sect_opt = "options";
    static const char* sect_show = "archive show opt";
    static const char* sect_mask_file = "mask_file";
    static const char* sect_mask_path = "mask_path";
    static const char* sect_ea_mask = "EA masks";
    static const char* sect_fsa_scope = "FSA Scope";
    static const char* sect_overwriting = "overwriting";
    deroule.add_section(sect_opt, "Restoration options");
    deroule.add_section(sect_show, "What to show during the operation");
    deroule.add_section(sect_mask_file, "File name based filtering");
    deroule.add_section(sect_mask_path, "Path based filtering");
    deroule.add_section(sect_ea_mask, "Extended Attributes filtering");
    deroule.add_section(sect_fsa_scope, "File system Specific Attributes filtering");
    deroule.add_section(sect_overwriting, "Overwriting policy");

    fs.adopt(&in_place);
    fs.adopt(&warn_over);
    fs.adopt(&warn_remove_no_match);
    fs.adopt(&flat);
    fs.adopt(&what_to_check);
    fs.adopt(&empty_dir);
    fs.adopt(&dirty_behavior);
    fs.adopt(&only_deleted);
    fs.adopt(&ignore_deleted);
    fs.adopt(&ignore_sockets);
    fs.adopt(&empty);
    form_archgen.adopt(&fs);
    deroule.adopt_in_section(sect_opt, &form_archgen);

    fs_show.adopt(&info_details);
    fs_show.adopt(&display_treated);
    fs_show.adopt(&display_treated_only_dir);
    fs_show.adopt(&display_skipped);
    form_show.adopt(&fs_show);
    deroule.adopt_in_section(sect_show, &form_show);

    deroule.adopt_in_section(sect_mask_file, &guichet_filename_mask);

    deroule.adopt_in_section(sect_mask_path, &guichet_path_mask);

    deroule.adopt_in_section(sect_ea_mask, &guichet_ea_mask);

    deroule.adopt_in_section(sect_fsa_scope, &fsa_scope);

    form_overwriting.adopt(&overwriting_policy);
    deroule.adopt_in_section(sect_overwriting, &form_overwriting);

    adopt(&deroule);

	// events
    display_treated.record_actor_on_event(this, html_form_input::changed);

	// css

    webdar_css_style::grey_button(deroule, true);
    display_treated_only_dir.add_css_class(webdar_css_style::wcs_indent);

	// components visibility status
    on_event(html_form_input::changed);
}

void html_options_extract::set_biblio(const shared_ptr<bibliotheque> & ptr)
{
    filename_mask->set_child(ptr, bibliotheque::filefilter);
    guichet_filename_mask.set_child(ptr,
				    bibliotheque::filefilter,
				    filename_mask,
				    false);

    ea_mask->set_child(ptr, bibliotheque::filefilter);
    path_mask->set_child(ptr, bibliotheque::pathfilter);
    guichet_ea_mask.set_child(ptr,
			      bibliotheque::filefilter,
			      ea_mask,
			      false);

    guichet_path_mask.set_child(ptr,
				bibliotheque::pathfilter,
				path_mask,
				false);
}

libdar::archive_options_extract html_options_extract::get_options() const
{
    libdar::archive_options_extract ret;

    ret.set_in_place(in_place.get_value_as_bool());
    ret.set_warn_over(warn_over.get_value_as_bool());
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

    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_display_treated(display_treated.get_value_as_bool(),
			    display_treated_only_dir.get_value_as_bool());
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
    ret.set_ignore_unix_sockets(ignore_sockets.get_value_as_bool());

    ret.set_selection(*(filename_mask->get_mask()));
    ret.set_subtree(*(path_mask->get_mask()));
    ret.set_ea_mask(*(ea_mask->get_mask()));
    ret.set_fsa_scope(fsa_scope.get_scope());
    ret.set_overwriting_rules(*(overwriting_policy.get_overwriting_action()));

    return ret;
}

void html_options_extract::on_event(const string & event_name)
{
    if(event_name == html_form_input::changed)
    {
	display_treated_only_dir.set_visible(display_treated.get_value_as_bool());
    }
    else
	throw WEBDAR_BUG;
}

string html_options_extract::inherited_get_body_part(const chemin & path,
						     const request & req)
{
    return get_body_part_from_all_children(path, req);
}


void html_options_extract::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}
