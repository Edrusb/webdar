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
#include "html_archive_compare.hpp"

using namespace std;

const string html_archive_compare::diff_root_changed = "diff_fs_root_changed";

html_archive_compare::html_archive_compare():
    diff_fs_root_fs(""),
    diff_fs_root("Directory to compare the backup against", "/", "80%", "Select the directory to compare with..."),
    diff_fs_root_form("Update")
{

	// components configuration

    static const char* sect_diff_params = "dparams";

    diff_params.add_section(sect_diff_params, "Comparison parameters");
    diff_params.set_active_section(sect_diff_params);

    diff_fs_root.set_select_mode(html_form_input_file::select_dir);
    diff_fs_root.set_can_create_dir(false);
    diff_fs_root.set_change_event_name(diff_root_changed);

    opt_diff.set_fs_root(diff_fs_root.get_value());


	// adoption tree

    diff_fs_root_fs.adopt(&diff_fs_root);
    diff_fs_root_form.adopt(&diff_fs_root_fs);
    diff_params.adopt_in_section(sect_diff_params, &diff_fs_root_form);
    diff_params.adopt_in_section(sect_diff_params, &opt_diff);
    adopt(&diff_params);

	// events

    diff_fs_root.record_actor_on_event(this, diff_root_changed);


	// css
    webdar_css_style::normal_button(diff_params, true);

}

void html_archive_compare::on_event(const string & event_name)
{
    if(event_name == diff_root_changed)
    {
	opt_diff.set_fs_root(diff_fs_root.get_value());
    }
    else
	throw WEBDAR_BUG;
}

string html_archive_compare::inherited_get_body_part(const chemin & path,
						     const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_archive_compare::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}

