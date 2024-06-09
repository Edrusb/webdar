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
#include "webdar_css_style.hpp"

    // webdar headers
#include "webdar_tools.hpp"
#include "html_form_mask_expression.hpp"

    //
#include "html_options_compare.hpp"

using namespace std;

html_options_compare::html_options_compare():
    form("Update options"),
    fs(""),
    info_details("Detailed information",
		 html_form_input::check,
		 "1",
		 1),
    alter_atime("Modify atime",
		html_form_input::check,
		"",
		1),
    furtive_read_mode("Furtive read mode",
		      html_form_input::check,
		      "",
		      1),
    display_skipped("Display skipped files",
		    html_form_input::check,
		    "1",
		    1),
    hourshift("Ignore difference of exactly +/- N hour(s)",
	      html_form_input::number,
	      "0",
	      2),
    compare_symlink_date("Compare symlink mtime",
			 html_form_input::check,
			 "",
			 1)
{
    libdar::archive_options_diff defaults;

	/// default values
    what_to_check.set_value(defaults.get_what_to_check());
    alter_atime.set_value_as_bool(defaults.get_alter_atime());
    furtive_read_mode.set_value_as_bool(defaults.get_furtive_read_mode());
    hourshift.set_value(webdar_tools_convert_to_string(defaults.get_hourshift()));
    compare_symlink_date.set_value_as_bool(defaults.get_compare_symlink_date());

	// building adoption tree

    static const char* sect_opt = "options";
    static const char* sect_mask = "mask";
    deroule.add_section(sect_opt, "Comparison options");
    deroule.add_section(sect_mask, "Filename based filtering");

    fs.adopt(&info_details);
    fs.adopt(&what_to_check);
    fs.adopt(&alter_atime);
    fs.adopt(&furtive_read_mode);
    fs.adopt(&display_skipped);
    fs.adopt(&hourshift);
    fs.adopt(&compare_symlink_date);
    form.adopt(&fs);
    deroule.adopt_in_section(sect_opt, &form);
    deroule.adopt_in_section(sect_mask, &filename_mask);
    adopt(&deroule);

	// css

    webdar_css_style::grey_button(deroule, true);
}

libdar::archive_options_diff html_options_compare::get_options() const
{
    libdar::archive_options_diff ret;

    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_what_to_check(what_to_check.get_value());
    ret.set_alter_atime(alter_atime.get_value_as_bool());
    ret.set_furtive_read_mode(furtive_read_mode.get_value_as_bool());
    ret.set_display_skipped(display_skipped.get_value_as_bool());
    ret.set_hourshift(libdar::infinint(webdar_tools_convert_to_int(hourshift.get_value())));
    ret.set_compare_symlink_date(compare_symlink_date.get_value_as_bool());
    ret.set_selection(*(filename_mask.get_mask()));

    return ret;
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
