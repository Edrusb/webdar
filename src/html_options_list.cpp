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
#include <string>

    // webdar headers
#include "webdar_css_style.hpp"

    //
#include "html_options_list.hpp"

using namespace std;

html_options_list::html_options_list():
    fs(""),
    form("Update")
{

	// component setup
    static const char* listing_opt = "listing_opt";
    deroule.add_section(listing_opt, "Listing Parameters");
    deroule.set_active_section(listing_opt);

    action.add_choice("list", "display backup contents");
    action.add_choice("summ", "display backup properties");
    action.set_selected_num(0);

	// adoption tree
    fs.adopt(&action);
    form.adopt(&fs);
    deroule.adopt_in_section(listing_opt, &form);
    adopt(&deroule);

	// events


	// css
    webdar_css_style::normal_button(deroule, true);
}

string html_options_list::inherited_get_body_part(const chemin & path,
						  const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_options_list::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}
