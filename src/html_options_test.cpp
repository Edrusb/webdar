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

    //
#include "html_options_test.hpp"

using namespace std;

html_options_test::html_options_test():
    form("Update options"),
    fs(""),
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
		    "1")
{
    libdar::archive_options_test defaults;

    filename_mask.reset(new (nothrow) html_mask_form_filename("file name"));
    if(!filename_mask)
	throw exception_memory();

    path_mask.reset(new (nothrow) html_mask_form_path(false));
    if(!path_mask)
	throw exception_memory();

	// default values
    empty.set_value_as_bool(defaults.get_empty());

	// building HTML structure

    static const char* sect_opt = "options";
    static const char* sect_show = "archive show opt";
    static const char* sect_mask = "mask";
    static const char* sect_path = "path";
    deroule.add_section(sect_opt, "Testing options");
    deroule.add_section(sect_show, "What to show during the operation");
    deroule.add_section(sect_mask, "File name based filtering");
    deroule.add_section(sect_path, "Path based filtering");

    fs.adopt(&empty);
    form.adopt(&fs);
    deroule.adopt_in_section(sect_opt, &form);

    fs_show.adopt(&info_details);
    fs_show.adopt(&display_treated);
    fs_show.adopt(&display_treated_only_dir);
    fs_show.adopt(&display_skipped);
    form_show.adopt(&fs_show);
    deroule.adopt_in_section(sect_show, &form_show);

    deroule.adopt_in_section(sect_mask, &guichet_filename_mask);

    deroule.adopt_in_section(sect_path, &guichet_path_mask);

    adopt(&deroule);

	// events
    display_treated.record_actor_on_event(this, html_form_input::changed);

	// css

    webdar_css_style::grey_button(deroule, true);
    display_treated_only_dir.add_css_class(webdar_css_style::wcs_indent);

	// components visibility status
    on_event(html_form_input::changed);

}

void html_options_test::set_biblio(const std::shared_ptr<bibliotheque> & ptr)
{
    filename_mask->set_child(ptr, bibliotheque::filefilter);
    guichet_filename_mask.set_child(ptr,
				    bibliotheque::filefilter,
				    filename_mask,
				    false);

    guichet_path_mask.set_child(ptr,
				bibliotheque::pathfilter,
				path_mask,
				false);
}


libdar::archive_options_test html_options_test::get_options() const
{
    libdar::archive_options_test ret;

    ret.set_empty(empty.get_value_as_bool());

    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_display_treated(display_treated.get_value_as_bool(),
			    display_treated_only_dir.get_value_as_bool());
    ret.set_display_skipped(display_skipped.get_value_as_bool());

    ret.set_selection(*(filename_mask->get_mask()));
    ret.set_subtree(*(path_mask->get_mask()));

    return ret;
}

void html_options_test::on_event(const string & event_name)
{
    if(event_name == html_form_input::changed)
    {
	display_treated_only_dir.set_visible(display_treated.get_value_as_bool());
    }
    else
	throw WEBDAR_BUG;
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
