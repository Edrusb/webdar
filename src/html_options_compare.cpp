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
#include "webdar_css_style.hpp"

    // webdar headers
#include "webdar_tools.hpp"
#include "html_form_mask_expression.hpp"

    //
#include "html_options_compare.hpp"

using namespace std;

html_options_compare::html_options_compare():
    form("Update"),
    fs(""),
    form_reading("Update"),
    fs_alter_atime("What to alter if furtive read mode is not used"),
    furtive_read_mode("Furtive read mode (if available)",
		      html_form_input::check,
		      "",
		      1),
    hourshift("Ignore difference of exactly +/- N hour(s)",
	      html_form_input::number,
	      "0",
	      2),
    compare_symlink_date("Compare symlink mtime",
			 html_form_input::check,
			 "",
			 1),
    form_show("Update"),
    fs_show(""),
    info_details("Detailed information",
		 html_form_input::check,
		 "1",
		 1),
    display_treated("Display treated files",
		    html_form_input::check,
		    "1",
		    1),
    display_treated_only_dir("Display only treated directories",
			     html_form_input::check,
			     "",
			     1),
    display_skipped("Display skipped files",
		    html_form_input::check,
		    "1",
		    1),
    filename_mask("Filename expression"),
    path_mask(false),
    ea_mask("Extended Attribute expression")
{
    libdar::archive_options_diff defaults;

	/// default values
    alter_atime.add_choice("atime", "Data last access time (atime)");
    alter_atime.add_choice("ctime", "Inode last change time (ctime)");
    if(defaults.get_alter_atime())
	alter_atime.set_selected(0);
    else
	alter_atime.set_selected(1);
    what_to_check.set_value(defaults.get_what_to_check());
    furtive_read_mode.set_value_as_bool(defaults.get_furtive_read_mode());
    hourshift.set_value(webdar_tools_convert_to_string(defaults.get_hourshift()));
    compare_symlink_date.set_value_as_bool(defaults.get_compare_symlink_date());


	// building adoption tree

    static const char* sect_source = "source reading mode";
    static const char* sect_opt = "options";
    static const char* sect_show = "archive show opt";
    static const char* sect_mask_filename = "mask_file";
    static const char* sect_mask_path = "mask_path";
    static const char* sect_ea_mask = "EA masks";
    static const char* sect_fsa_scope = "FSA Scope";
    deroule.add_section(sect_source, "Filesystem reading mode");
    deroule.add_section(sect_opt, "Comparison options");
    deroule.add_section(sect_show, "What to show during the operation");
    deroule.add_section(sect_mask_filename, "Filename based filtering");
    deroule.add_section(sect_mask_path, "Path based filtering");
    deroule.add_section(sect_ea_mask, "Extended Attributes filtering");
    deroule.add_section(sect_fsa_scope, "Filesystem Specific Attributes filtering");

    fs_alter_atime.adopt(&alter_atime);
    form_reading.adopt(&furtive_read_mode);
    form_reading.adopt(&fs_alter_atime);
    deroule.adopt_in_section(sect_source, &form_reading);

    fs.adopt(&what_to_check);
    fs.adopt(&hourshift);
    fs.adopt(&compare_symlink_date);
    form.adopt(&fs);
    deroule.adopt_in_section(sect_opt, &form);

    fs_show.adopt(&info_details);
    fs_show.adopt(&display_treated);
    fs_show.adopt(&display_treated_only_dir);
    fs_show.adopt(&display_skipped);
    form_show.adopt(&fs_show);
    deroule.adopt_in_section(sect_show, &form_show);

    deroule.adopt_in_section(sect_mask_filename, &filename_mask);

    deroule.adopt_in_section(sect_mask_path, &path_mask);

    deroule.adopt_in_section(sect_ea_mask, &ea_mask);

    deroule.adopt_in_section(sect_fsa_scope, &fsa_scope);

    adopt(&deroule);

	// events

    display_treated.record_actor_on_event(this, html_form_input::changed);

	// css

    webdar_css_style::grey_button(deroule, true);
    display_treated_only_dir.add_css_class(webdar_css_style::wcs_indent);

	// components visibility setup

    on_event(html_form_input::changed);
}

libdar::archive_options_diff html_options_compare::get_options() const
{
    libdar::archive_options_diff ret;

    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_display_treated(display_treated.get_value_as_bool(),
			    display_treated_only_dir.get_value_as_bool());
    ret.set_display_skipped(display_skipped.get_value_as_bool());
    ret.set_what_to_check(what_to_check.get_value());
    ret.set_alter_atime(alter_atime.get_selected_id() == "atime");
    ret.set_furtive_read_mode(furtive_read_mode.get_value_as_bool());
    ret.set_hourshift(libdar::infinint(webdar_tools_convert_to_int(hourshift.get_value())));
    ret.set_compare_symlink_date(compare_symlink_date.get_value_as_bool());
    ret.set_selection(*(filename_mask.get_mask()));
    ret.set_subtree(*(path_mask.get_mask()));
    ret.set_ea_mask(*(ea_mask.get_mask()));
    ret.set_fsa_scope(fsa_scope.get_scope());

    return ret;
}

void html_options_compare::on_event(const std::string & event_name)
{
    if(event_name == html_form_input::changed)
    {
	display_treated_only_dir.set_visible(display_treated.get_value_as_bool());
    }
    else
	throw WEBDAR_BUG;
}



string html_options_compare::inherited_get_body_part(const chemin & path,
						     const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_options_compare::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}
