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
#include <dar/tools.hpp>

    // webdar headers
#include "webdar_css_style.hpp"

    //
#include "html_options_test.hpp"

using namespace std;

const string html_options_test::changed = "hot_test";

html_options_test::html_options_test():
    form("Update options"),
    fs(""),
    empty("Dry-run execution",
	  html_form_input::check,
	  "",
	  "",
	  ""),
    form_show("Update"),
    fs_show(""),
    info_details("Detailed informations",
		 html_form_input::check,
		 "1",
		 "",
		 ""),
    display_treated("Display treated files",
		    html_form_input::check,
		    "1",
		    "",
		    ""),
    display_treated_only_dir("Display only treated directories",
			     html_form_input::check,
			     "",
			     "",
			     ""),
    display_skipped("Display skipped files",
		    html_form_input::check,
		    "1",
		    "",
		    ""),
    ignore_events(false)
{
    filename_mask.reset(new (nothrow) html_mask_form_filename("file name"));
    if(!filename_mask)
	throw exception_memory();

    path_mask.reset(new (nothrow) html_mask_form_path(false));
    if(!path_mask)
	throw exception_memory();

	// default values
    reset_non_pointer_fields();

	// building HTML structure

    static const char* sect_opt = "options";
    static const char* sect_show = "archive show opt";
    static const char* sect_mask = "mask";
    static const char* sect_path = "path";
    deroule.add_section(sect_opt, "How to process the operation");
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
    empty.record_actor_on_event(this, html_form_input::changed);
    info_details.record_actor_on_event(this, html_form_input::changed);
    display_treated.record_actor_on_event(this, html_form_input::changed);
    display_treated_only_dir.record_actor_on_event(this, html_form_input::changed);
    display_skipped.record_actor_on_event(this, html_form_input::changed);
    filename_mask->record_actor_on_event(this, html_mask_form_filename::changed);
    path_mask->record_actor_on_event(this, html_mask_form_path::changed);
    register_name(changed);

	// css

    webdar_css_style::grey_button(deroule, true);
    display_treated_only_dir.add_css_class(webdar_css_style::indent);

	// components visibility status
    on_event(html_form_input::changed);
}

void html_options_test::set_biblio(const shared_ptr<bibliotheque> & ptr)
{
    filename_mask->set_child(ptr, bibliotheque::filefilter);
    guichet_filename_mask.set_child(ptr,
				    bibliotheque::filefilter,
				    filename_mask,
				    false);
    path_mask->set_child(ptr, bibliotheque::pathfilter);
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

void html_options_test::load_json(const json & source)
{
    try
    {
	unsigned int version;
	string class_id;
	json config = unwrap_config_from_json_header(source,
						     version,
						     class_id);

	if(class_id != myclass_id)
	    throw exception_range(libdar::tools_printf("Unexpected class_id in json data, found %s while expecting %s",
						       class_id.c_str(),
						       myclass_id));

	if(version > format_version)
	    throw exception_range(libdar::tools_printf("Json format version too hight for %s, upgrade your webdar software",
						       myclass_id));

	ignore_events = true;

	try
	{
	    empty.set_value_as_bool(config.at(jlabel_dry_run));
	    info_details.set_value_as_bool(config.at(jlabel_info_details));
	    display_treated.set_value_as_bool(config.at(jlabel_disp_treated));
	    display_treated_only_dir.set_value_as_bool(config.at(jlabel_disp_only_dir));
	    display_skipped.set_value_as_bool(config.at(jlabel_disp_skipped));
	    guichet_filename_mask.load_json(config.at(jlabel_file_mask));
	    guichet_path_mask.load_json(config.at(jlabel_path_mask));
	}
	catch(...)
	{
	    ignore_events = false;
	    throw;
	}
	ignore_events = false;

	trigger_change();
    }
    catch(json::exception & e)
    {
	throw exception_json(libdar::tools_printf("Error loading %s config", myclass_id), e);
    }
}

json html_options_test::save_json() const
{
    json config;

    config[jlabel_dry_run] = empty.get_value_as_bool();
    config[jlabel_info_details] = info_details.get_value_as_bool();
    config[jlabel_disp_treated] = display_treated.get_value_as_bool();
    config[jlabel_disp_only_dir] = display_treated_only_dir.get_value_as_bool();
    config[jlabel_disp_skipped] = display_skipped.get_value_as_bool();
    config[jlabel_file_mask] = guichet_filename_mask.save_json();
    config[jlabel_path_mask] = guichet_path_mask.save_json();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					config);
}

void html_options_test::clear_json()
{
    ignore_events = true;
    try
    {
	reset_non_pointer_fields();
	guichet_filename_mask.clear_json();
	guichet_path_mask.clear_json();
    }
    catch(...)
    {
	ignore_events = false;
	throw;
    }
    ignore_events = false;

    trigger_change();
}

bibliotheque::using_set html_options_test::get_using_set() const
{
    bibliotheque::using_set ret;

    subconfig_add_to(ret, guichet_filename_mask.get_using_set());
    subconfig_add_to(ret, guichet_path_mask.get_using_set());

    return ret;
}

void html_options_test::on_event(const string & event_name)
{
    if(event_name == html_form_input::changed)
    {
	display_treated_only_dir.set_visible(display_treated.get_value_as_bool());
    }
    else if(event_name == html_mask_form_filename::changed
	    || event_name == html_mask_form_path::changed)
    {
    }
    else
	throw WEBDAR_BUG;

    trigger_change();
}

void html_options_test::set_to_webdar_defaults()
{
    clear_json();

    info_details.set_value_as_bool(true);
    display_treated.set_value_as_bool(true);
    display_treated_only_dir.set_value_as_bool(false);
    display_skipped.set_value_as_bool(true);
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

void html_options_test::reset_non_pointer_fields()
{
    empty.set_value_as_bool(false);
    info_details.set_value_as_bool(true);
    display_treated.set_value_as_bool(true);
    display_treated_only_dir.set_value_as_bool(false);
    display_skipped.set_value_as_bool(true);
}

void html_options_test::trigger_change()
{
    if(! ignore_events)
	act(changed);
}

