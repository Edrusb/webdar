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
#include "webdar_css_style.hpp"
#include "html_form_mask_expression.hpp"

    //
#include "html_options_test.hpp"

using namespace std;

html_options_test::html_options_test():
    form("Update options"),
    fs(""),
    info_details("Detailed informations",
		 html_form_input::check,
		 "1",
		 1),
    empty("Dry-run execution",
	  html_form_input::check,
	  "",
	  1),
    display_skipped("Display skipped files",
		    html_form_input::check,
		    "1",
		    1)
{
    libdar::archive_options_test defaults;

	// default values
    empty.set_value_as_bool(defaults.get_empty());

	// building HTML structure

    static const char* sect_opt = "options";
    static const char* sect_mask = "mask";
    deroule.add_section(sect_opt, "Testing options");
    deroule.add_section(sect_mask, "Filename based filtering");

    fs.adopt(&info_details);
    fs.adopt(&empty);
    fs.adopt(&display_skipped);
    form.adopt(&fs);
    deroule.adopt_in_section(sect_opt, &form);
    deroule.adopt_in_section(sect_mask, &filename_mask);
    adopt(&deroule);

	// css

    webdar_css_style::grey_button(deroule, true);
}

libdar::archive_options_test html_options_test::get_options() const
{
    libdar::archive_options_test ret;

    ret.set_display_treated(info_details.get_value_as_bool(), false);
    ret.set_empty(empty.get_value_as_bool());
    ret.set_display_skipped(display_skipped.get_value_as_bool());
    ret.set_selection(*(filename_mask.get_mask()));

    return ret;
}

string html_options_test::inherited_get_body_part(const chemin & path,
						  const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_options_test::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}
