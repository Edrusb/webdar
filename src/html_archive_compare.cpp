/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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
#include "environment.hpp"

    //
#include "html_archive_compare.hpp"

using namespace std;

const string html_archive_compare::diff_root_changed = "diff_fs_root_changed";

html_archive_compare::html_archive_compare():
    diff_fs_root_fs(""),
    diff_fs_root("Directory to compare the backup against",
		 global_envir.get_value_with_default("HOME", "/"),
		 "",
		 webdar_css_style::width_80vw,
		 "Select the directory to compare with..."),
    diff_fs_root_form("Update")
{
    opt_diff.reset(new (nothrow) html_options_compare());
    if(! opt_diff)
	throw exception_memory();

	// components configuration

    static const char* sect_diff_params = "dparams";
    static const char* sect_diff_options = "doptions";

    diff_params.add_section(sect_diff_params, "Comparison Parameters");
    diff_params.add_section(sect_diff_options, "Comparison Options");
    diff_params.set_active_section(sect_diff_params);

    diff_fs_root.set_select_mode(html_form_input_file::select_dir);
    diff_fs_root.set_can_create_dir(false);
    diff_fs_root.set_change_event_name(diff_root_changed);

    opt_diff->set_fs_root(diff_fs_root.get_value());

    in_place_msg.add_text(4, "Warning, \"in-place\" comparison is set!");
    in_place_msg.add_text(0, "The field above will only be used if the in-place information is not present in the archive");
    in_place_msg.add_nl();
    in_place_msg.add_text(0, "You can disable \"in-place\" comparison below in the section [Comparison Options | How to proceed]");

	// adoption tree

    diff_fs_root_fs.adopt(&diff_fs_root);
    diff_fs_root_fs.adopt(&in_place_msg);
    diff_fs_root_form.adopt(&diff_fs_root_fs);
    diff_params.adopt_in_section(sect_diff_params, &diff_fs_root_form);
    diff_params.adopt_in_section(sect_diff_options, &guichet_opt_diff);
    adopt(&diff_params);

	// events

    diff_fs_root.record_actor_on_event(this, diff_root_changed);
    opt_diff->record_actor_on_event(this, html_options_compare::changed);

	// visible

    in_place_msg.set_visible(false);

	// css

    webdar_css_style::normal_button(diff_params, true);
    in_place_msg.add_css_class(webdar_css_style::text_grey);
}

void html_archive_compare::set_biblio(const shared_ptr<bibliotheque> & ptr)
{
    if(!opt_diff)
	throw WEBDAR_BUG;
    opt_diff->set_biblio(ptr);

    guichet_opt_diff.set_child(ptr,
			       bibliotheque::confdiff,
			       opt_diff,
			       false);

    if(ptr->has_config(bibliotheque::confdiff, bibliotheque::default_config_name))
    {
	try
	{
	    guichet_opt_diff.load_from_bibliotheque(bibliotheque::default_config_name);
	}
	catch(...)
	{
		// ignore exception here, as this is called from constructor
		// of the session components and nothing is yet available to
		// display execeptions messages (due to old config in configuration file
		// or corrupted configuration file we read from)
		// The consequence is that we have a cleared configuration:
	    guichet_opt_diff.clear_json();
	}
    }
}

libdar::archive_options_diff html_archive_compare::get_options() const
{
    if(! opt_diff)
	throw WEBDAR_BUG;

    return opt_diff->get_options();
}

void html_archive_compare::on_event(const string & event_name)
{
    if(event_name == diff_root_changed)
    {
	opt_diff->set_fs_root(diff_fs_root.get_value());
    }
    else if(event_name == html_options_compare::changed)
    {
	in_place_msg.set_visible(opt_diff->get_in_place_mode());
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

