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
#include "webdar_css_style.hpp"


    //
#include "html_archive_merge.hpp"

using namespace std;

html_archive_merge::html_archive_merge():
    form("Update"),
    fs("Resulting archive"),
    sauv_path("Where to create the archive", "/", 20, "Select directory where to create the archive..."),
    basename("Archive basename", html_form_input::text, "", 10),
    reference("Archive of reference")
{
    static const char* sect_archive = "archive";
    static const char* sect_ref = "reference";
    static const char* sect_opt = "options";

    deroule.add_section(sect_archive, "Archive Merging");
    deroule.add_section(sect_ref, "Reference Archive");
    deroule.add_section(sect_opt, "Merging Options");
    deroule.set_active_section(0);

    sauv_path.set_select_mode(html_form_input_file::select_dir);
    sauv_path.set_can_create_dir(true);

	// adoption tree

    fs.adopt(&sauv_path);
    fs.adopt(&basename);
    form.adopt(&fs);
    deroule.adopt_in_section(sect_archive, &form);
    deroule.adopt_in_section(sect_ref, &reference);
    deroule.adopt_in_section(sect_opt, &options);
    adopt(&deroule);

	// CSS

    webdar_css_style::normal_button(deroule, true);
}

string html_archive_merge::inherited_get_body_part(const chemin & path,
						   const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_archive_merge::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}
