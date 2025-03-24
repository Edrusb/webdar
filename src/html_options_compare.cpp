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
#include "webdar_tools.hpp"
#include "html_form_mask_expression.hpp"
#include "webdar_css_style.hpp"

    //
#include "html_options_compare.hpp"

using namespace std;

const string html_options_compare::changed = "hocompare_changed";

html_options_compare::html_options_compare():
    form("Update"),
    fs(""),
    in_place("Ignore the provided root directory above and use instead the \"in place\" directory information stored in the archive",
	     html_form_input::check,
	     "",
	     "1"),
    form_reading("Update"),
    fs_alter_atime("What to alter if furtive read mode is not used"),
    furtive_read_mode("Furtive read mode (if available)",
		      html_form_input::check,
		      "",
		      "1"),
    zeroing_neg_date("Automatically zeroing negative dates while reading",
		     html_form_input::check,
		     "",
		     "1"),
    hourshift("Ignore difference of exactly +/- N hour(s)",
	      html_form_input::number,
	      "0",
	      "5"),
    compare_symlink_date("Compare symlink mtime",
			 html_form_input::check,
			 "",
			 "1"),
    form_show("Update"),
    fs_show(""),
    info_details("Detailed information",
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
		    "1"),
    ignore_events(false)
{

    filename_mask.reset(new (nothrow) html_mask_form_filename("file name"));
    if(!filename_mask)
	throw exception_memory();

    ea_mask.reset(new (nothrow) html_mask_form_filename("extended attribute"));
    if(!ea_mask)
	throw exception_memory();

    path_mask.reset(new (nothrow) html_mask_form_path(false));
    if(!path_mask)
	throw exception_memory();

	/// component setup
    hourshift.set_range(0, 23);

    libdar::archive_options_diff defaults;

	/// default values
    in_place.set_value_as_bool(defaults.get_in_place());
    alter_atime.add_choice("atime", "Data last access time (atime)");
    alter_atime.add_choice("ctime", "Inode last change time (ctime)");
    if(defaults.get_alter_atime())
	alter_atime.set_selected_num(0);
    else
	alter_atime.set_selected_num(1);
    what_to_check.set_value(defaults.get_what_to_check());
    furtive_read_mode.set_value_as_bool(defaults.get_furtive_read_mode());
    zeroing_neg_date.set_value_as_bool(defaults.get_auto_zeroing_neg_dates());
    hourshift.set_value(webdar_tools_convert_to_string(defaults.get_hourshift()));
    compare_symlink_date.set_value_as_bool(defaults.get_compare_symlink_date());


	// building adoption tree

    static const char* sect_source = "source reading mode";
    static const char* sect_opt = "options";
    static const char* sect_show = "archive show opt";
    static const char* sect_mask_filename = "mask_file";
    static const char* sect_mask_path = "mask_path";
    static const char* sect_ea_mask = "EA masks";
    static const char* sect_fsa_scope = "FSA Scope";
    deroule.add_section(sect_source, "File system reading mode");
    deroule.add_section(sect_opt, "How to process the operation");
    deroule.add_section(sect_show, "What to show during the operation");
    deroule.add_section(sect_mask_filename, "File name based filtering");
    deroule.add_section(sect_mask_path, "Path based filtering");
    deroule.add_section(sect_ea_mask, "Extended Attributes filtering");
    deroule.add_section(sect_fsa_scope, "File system Specific Attributes filtering");

    fs_alter_atime.adopt(&alter_atime);
    form_reading.adopt(&furtive_read_mode);
    form_reading.adopt(&fs_alter_atime);
    form_reading.adopt(&zeroing_neg_date);
    form_reading.adopt(&hourshift);
    deroule.adopt_in_section(sect_source, &form_reading);

    fs.adopt(&in_place);
    fs.adopt(&what_to_check);
    fs.adopt(&compare_symlink_date);
    form.adopt(&fs);
    deroule.adopt_in_section(sect_opt, &form);

    fs_show.adopt(&info_details);
    fs_show.adopt(&display_treated);
    fs_show.adopt(&display_treated_only_dir);
    fs_show.adopt(&display_skipped);
    form_show.adopt(&fs_show);
    deroule.adopt_in_section(sect_show, &form_show);

    deroule.adopt_in_section(sect_mask_filename, &guichet_filename_mask);

    deroule.adopt_in_section(sect_mask_path, &guichet_path_mask);

    deroule.adopt_in_section(sect_ea_mask, &guichet_ea_mask);

    deroule.adopt_in_section(sect_fsa_scope, &fsa_scope);

    adopt(&deroule);

	// events
    alter_atime.record_actor_on_event(this, html_form_radio::changed);
    furtive_read_mode.record_actor_on_event(this, html_form_input::changed);
    zeroing_neg_date.record_actor_on_event(this, html_form_input::changed);
    hourshift.record_actor_on_event(this, html_form_input::changed);
    in_place.record_actor_on_event(this, html_form_input::changed);
    what_to_check.record_actor_on_event(this, html_form_select::changed);
    compare_symlink_date.record_actor_on_event(this, html_form_input::changed);
    info_details.record_actor_on_event(this, html_form_input::changed);
    display_treated.record_actor_on_event(this, html_form_input::changed);
    display_treated_only_dir.record_actor_on_event(this, html_form_input::changed);
    display_skipped.record_actor_on_event(this, html_form_input::changed);
    filename_mask->record_actor_on_event(this, html_mask_form_filename::changed);
    path_mask->record_actor_on_event(this, html_mask_form_path::changed);
    ea_mask->record_actor_on_event(this, html_mask_form_filename::changed);
    fsa_scope.record_actor_on_event(this, html_fsa_scope::changed);
    register_name(changed);
	// css

    webdar_css_style::grey_button(deroule, true);
    display_treated_only_dir.add_css_class(webdar_css_style::indent);

	// components visibility setup

    on_event(html_form_input::changed);
}

void html_options_compare::set_biblio(const shared_ptr<bibliotheque> & ptr)
{
    filename_mask->set_child(ptr, bibliotheque::filefilter);
    guichet_filename_mask.set_child(ptr,
				    bibliotheque::filefilter,
				    filename_mask,
				    false);

    ea_mask->set_child(ptr, bibliotheque::filefilter);
    path_mask->set_child(ptr, bibliotheque::pathfilter);
    guichet_ea_mask.set_child(ptr,
			      bibliotheque::filefilter,
			      ea_mask,
			      false);

    guichet_path_mask.set_child(ptr,
				bibliotheque::pathfilter,
				path_mask,
				false);
}

libdar::archive_options_diff html_options_compare::get_options() const
{
    libdar::archive_options_diff ret;

    ret.set_in_place(in_place.get_value_as_bool());
    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_display_treated(display_treated.get_value_as_bool(),
			    display_treated_only_dir.get_value_as_bool());
    ret.set_display_skipped(display_skipped.get_value_as_bool());
    ret.set_what_to_check(what_to_check.get_value());
    ret.set_alter_atime(alter_atime.get_selected_id() == "atime");
    ret.set_furtive_read_mode(furtive_read_mode.get_value_as_bool());
    ret.set_auto_zeroing_neg_dates(zeroing_neg_date.get_value_as_bool());
    ret.set_hourshift(libdar::infinint(webdar_tools_convert_to_int(hourshift.get_value())));
    ret.set_compare_symlink_date(compare_symlink_date.get_value_as_bool());
    ret.set_selection(*(filename_mask->get_mask()));
    ret.set_subtree(*(path_mask->get_mask()));
    ret.set_ea_mask(*(ea_mask->get_mask()));
    ret.set_fsa_scope(fsa_scope.get_scope());

    return ret;
}

void html_options_compare::load_json(const json & source)
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
	    alter_atime.set_selected_id(config.at(jlabel_alter_atime));
	    furtive_read_mode.set_value_as_bool(config.at(jlabel_furtive_read));
	    zeroing_neg_date.set_value_as_bool(config.at(jlabel_zeroing_neg_dates));
	    hourshift.set_value_as_int(config.at(jlabel_hourshift));
	    in_place.set_value_as_bool(config.at(jlabel_in_place));
	    what_to_check.set_selected_id_with_warning(config.at(jlabel_what_to_check), jlabel_what_to_check);
	    compare_symlink_date.set_value_as_bool(config.at(jlabel_symlink_date));
	    info_details.set_value_as_bool(config.at(jlabel_info_details));
	    display_treated.set_value_as_bool(config.at(jlabel_disp_treated));
	    display_treated_only_dir.set_value_as_bool(config.at(jlabel_disp_only_dir));
	    display_skipped.set_value_as_bool(config.at(jlabel_disp_skipped));
	    guichet_filename_mask.load_json(config.at(jlabel_file_mask));
	    guichet_path_mask.load_json(config.at(jlabel_path_mask));
	    guichet_ea_mask.load_json(config.at(jlabel_fsa_scope));
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

json html_options_compare::save_json() const
{
    json config;

    config[jlabel_alter_atime] = alter_atime.get_selected_id();
    config[jlabel_furtive_read] = furtive_read_mode.get_value_as_bool();
    config[jlabel_zeroing_neg_dates] = zeroing_neg_date.get_value_as_bool();
    config[jlabel_hourshift] = hourshift.get_value_as_int();
    config[jlabel_in_place] = in_place.get_value_as_bool();
    config[jlabel_what_to_check] = what_to_check.get_selected_id();
    config[jlabel_symlink_date] = compare_symlink_date.get_value_as_bool();
    config[jlabel_info_details] = info_details.get_value_as_bool();
    config[jlabel_disp_treated] = display_treated.get_value_as_bool();
    config[jlabel_disp_only_dir] = display_treated_only_dir.get_value_as_bool();
    config[jlabel_disp_skipped] = display_skipped.get_value_as_bool();
    config[jlabel_file_mask] = guichet_filename_mask.save_json();
    config[jlabel_path_mask] = guichet_path_mask.save_json();
    config[jlabel_fsa_scope] = guichet_ea_mask.save_json();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					config);
}

void html_options_compare::clear_json()
{
    try
    {
	alter_atime.set_selected_num(0);
	furtive_read_mode.set_value_as_bool(true);
	zeroing_neg_date.set_value_as_bool(true);
	hourshift.set_value_as_int(0);
	in_place.set_value_as_bool(false);
	what_to_check.set_selected_num(0);
	compare_symlink_date.set_value_as_bool(true);
	info_details.set_value_as_bool(true);
	display_treated.set_value_as_bool(true);
	display_treated_only_dir.set_value_as_bool(false);
	display_skipped.set_value_as_bool(true);
	guichet_filename_mask.clear_json();
	guichet_path_mask.clear_json();
	guichet_ea_mask.clear_json();
    }
    catch(...)
    {
	ignore_events = false;
	throw;
    }
    ignore_events = false;

    trigger_change();
}

bibliotheque::using_set html_options_compare::get_using_set() const
{
    bibliotheque::using_set ret;

    subconfig_add_to(ret, guichet_filename_mask.get_using_set());
    subconfig_add_to(ret, guichet_path_mask.get_using_set());
    subconfig_add_to(ret, guichet_ea_mask.get_using_set());

    return ret;
}

void html_options_compare::on_event(const std::string & event_name)
{
    if(event_name == html_form_input::changed)
    {
	display_treated_only_dir.set_visible(display_treated.get_value_as_bool());
    }
    else if(event_name == html_form_radio::changed
	    || event_name == html_form_select::changed
	    || event_name == html_mask_form_filename::changed
	    || event_name == html_mask_form_path::changed
	    || event_name == html_fsa_scope::changed)
    {
	    // nothing to do, because of trigger_change() below
    }
    else
	throw WEBDAR_BUG;

    trigger_change();
}

void html_options_compare::set_to_webdar_defaults()
{
    clear_json();

    alter_atime.set_selected_num(0);
    furtive_read_mode.set_value_as_bool(true);
    zeroing_neg_date.set_value_as_bool(true);
    in_place.set_value_as_bool(false);
    info_details.set_value_as_bool(true);
    display_treated.set_value_as_bool(true);
    display_treated_only_dir.set_value_as_bool(false);
    display_skipped.set_value_as_bool(true);
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

void html_options_compare::trigger_change()
{
    if(!ignore_events)
	act(changed);
}
