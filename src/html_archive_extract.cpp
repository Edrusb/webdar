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
#include "html_archive_extract.hpp"

using namespace std;

const string html_archive_extract::extract_root_changed = "extract_fs_root_changed";

html_archive_extract::html_archive_extract():
    extract_fs_root_fs(""),
    extract_fs_root("Directory to take as root for restoration", "/", "80%", "Select a directory where to restore to..."),
    extract_fs_root_form("Update")
{

	// components configuration

    static const char* sect_extract_params = "eparams";
    extract_params.add_section(sect_extract_params, "Restoration parameters");
    extract_params.set_active_section(sect_extract_params);

    extract_fs_root.set_select_mode(html_form_input_file::select_dir);
    extract_fs_root.set_can_create_dir(false);
    extract_fs_root.set_change_event_name(extract_root_changed);

    opt_extract.set_fs_root(extract_fs_root.get_value());

	// adoption tree

    extract_fs_root_fs.adopt(&extract_fs_root);
    extract_fs_root_form.adopt(&extract_fs_root_fs);
    extract_params.adopt_in_section(sect_extract_params, &extract_fs_root_form);
    extract_params.adopt_in_section(sect_extract_params, &opt_extract);
    adopt(&extract_params);

	// events

    extract_fs_root.record_actor_on_event(this, extract_root_changed);

	//  css

    webdar_css_style::normal_button(extract_params, true);

}

void html_archive_extract::on_event(const std::string & event_name)
{
    if(event_name == extract_root_changed)
    {
	opt_extract.set_fs_root(extract_fs_root.get_value());
    }
    else
	throw WEBDAR_BUG;
}

string html_archive_extract::inherited_get_body_part(const chemin & path,
						     const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_archive_extract::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}
