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



    //
#include "html_form_mask_subdir.hpp"

using namespace std;


html_form_mask_subdir::html_form_mask_subdir():
    fs(""),
    mask_type("Mask Type",
	      "unused_event"),
    casesensitivity("Case Sensitive",
		    html_form_input::check,
		    "1", // checked
		    1),
    mask_subdir("Concerned path",
		    html_form_input::text,
		    "",
		    30)
{

	// component configuration

    mask_type.add_choice("0", "Include path and subdir");
    mask_type.add_choice("1", "Include path only (no wildcard no subdir)");
    mask_type.add_choice("2", "Exclude path and subdir (no wildcard!)");
    mask_type.set_selected(0);

    init();
}

html_form_mask_subdir::html_form_mask_subdir(const html_form_mask_subdir & ref):
    html_mask(ref), // parent class
    fs(ref.fs),
    mask_type(ref.mask_type),
    casesensitivity(ref.casesensitivity),
    mask_subdir(ref.mask_subdir)
{
    init();
}

unique_ptr<libdar::mask> html_form_mask_subdir::get_mask() const
{
    bool casesensit = casesensitivity.get_value_as_bool();
    unique_ptr<libdar::mask> ret;

    switch(mask_type.get_selected_num())
    {
    case 0: // Include path and subdir
	ret.reset(new (nothrow) libdar::simple_path_mask(mask_subdir.get_value(),
							 casesensit));
	break;
    case 1: // Include fixed path only
	ret.reset(new (nothrow) libdar::same_path_mask(mask_subdir.get_value(),
						       casesensit));
	break;
    case 2: // Exclude path and subdir
	ret.reset(new (nothrow) libdar::exclude_dir_mask(mask_subdir.get_value(),
							 casesensit));
	break;
    default:
	throw WEBDAR_BUG;
    }

    if(!ret)
	throw exception_memory();

    return ret;
}


string html_form_mask_subdir::inherited_get_body_part(const chemin & path,
						   const request & req)
{
    return get_body_part_from_all_children(path, req);
}


void html_form_mask_subdir::init()
{
	// adoption tree

    fs.adopt(&mask_type);
    fs.adopt(&casesensitivity);
    fs.adopt(&mask_subdir);
    adopt(&fs);

	// events

	// visibity

	// css stuff
}
