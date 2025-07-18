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
#include <dar/tools.hpp>

    // webdar headers
#include "webdar_css_style.hpp"
#include "html_form_mask_expression.hpp"
#include "tooltip_messages.hpp"

    //
#include "html_options_extract.hpp"

using namespace std;

const string html_options_extract::changed = "html_options_extract_changed";


html_options_extract::html_options_extract():
    form_archgen("Update"),
    fs(""),
    in_place("Ignore the provided root directory above and use instead the \"in place\" directory information stored in the archive",
	     html_form_input::check,
	     "",
	     "",
	     ""),
    warn_over("Warn before overwriting",
	      html_form_input::check,
	      "1",
	      "",
	      ""),
    flat("Do not restore directory structure",
	 html_form_input::check,
	 "",
	 "",
	 ""),
    what_to_check("Fields to restore"),
    warn_remove_no_match("Warn if a file to remove does not match expected type",
			 html_form_input::check,
			 "",
			 "",
			 ""),
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
    empty_dir("Restore even empty directories",
	      html_form_input::check,
	      "1",
	      "",
	      ""),
    dirty_behavior("Dirty file behavior"),
    ignore_sockets("Do not restore Unix sockets",
		   html_form_input::check,
		   "",
		   "",
		   ""),
    only_deleted("Restore only deleted files",
		 html_form_input::check,
		 "",
		 "",
		 ""),
    ignore_deleted("Avoid removing files flagged as to be removed",
		   html_form_input::check,
		   "",
		   "",
		   ""),
    ignore_events(false)
{
	// set html fields to default value used by libdar

    filename_mask.reset(new (nothrow) html_mask_form_filename("file name"));
    if(!filename_mask)
	throw exception_memory();

    ea_mask.reset(new (nothrow) html_mask_form_filename("extended attribute"));
    if(!ea_mask)
	throw exception_memory();

    path_mask.reset(new (nothrow) html_mask_form_path(true));
    if(!path_mask)
	throw exception_memory();

    overwriting_policy.reset(new (nothrow) html_form_overwrite_action(""));
    if(! overwriting_policy)
	throw exception_memory();

    what_to_check.add_choice("all", "All");
    what_to_check.add_choice("ignore_owner", "All but ownership");
    what_to_check.add_choice("mtime", "All but ownership and permission");
    what_to_check.add_choice("inode_type", "Nothing inode related, just data");

    dirty_behavior.add_choice("ignore", "Exclude dirty files");
    dirty_behavior.add_choice("warn", "Warn before restoring");
    dirty_behavior.add_choice("ok", "Restored as normal files");

	// default values
	// these are set from set_biblio

	// build the adoption tree

    static const char* sect_opt = "options";
    static const char* sect_show = "archive show opt";
    static const char* sect_mask_file = "mask_file";
    static const char* sect_mask_path = "mask_path";
    static const char* sect_ea_mask = "EA masks";
    static const char* sect_fsa_scope = "FSA Scope";
    static const char* sect_overwriting = "overwriting";
    deroule.add_section(sect_opt, "How to process the operation");
    deroule.add_section(sect_show, "What to show during the operation");
    deroule.add_section(sect_mask_file, "File name based filtering");
    deroule.add_section(sect_mask_path, "Path based filtering");
    deroule.add_section(sect_ea_mask, "Extended Attributes filtering");
    deroule.add_section(sect_fsa_scope, "File system Specific Attributes filtering");
    deroule.add_section(sect_overwriting, "Overwriting policy");

    fs.adopt(&in_place);
    fs.adopt(&warn_over);
    fs.adopt(&warn_remove_no_match);
    fs.adopt(&flat);
    fs.adopt(&what_to_check);
    fs.adopt(&empty_dir);
    fs.adopt(&dirty_behavior);
    fs.adopt(&only_deleted);
    fs.adopt(&ignore_deleted);
    fs.adopt(&ignore_sockets);
    fs.adopt(&empty);
    form_archgen.adopt(&fs);
    deroule.adopt_in_section(sect_opt, &form_archgen);

    fs_show.adopt(&info_details);
    fs_show.adopt(&display_treated);
    fs_show.adopt(&display_treated_only_dir);
    fs_show.adopt(&display_skipped);
    form_show.adopt(&fs_show);
    deroule.adopt_in_section(sect_show, &form_show);

    deroule.adopt_in_section(sect_mask_file, &guichet_filename_mask);

    deroule.adopt_in_section(sect_mask_path, &guichet_path_mask);

    deroule.adopt_in_section(sect_ea_mask, &guichet_ea_mask);

    deroule.adopt_in_section(sect_fsa_scope, &fsa_scope);

    deroule.adopt_in_section(sect_overwriting, &guichet_overwriting_policy);

    adopt(&deroule);

	// events
    display_treated.record_actor_on_event(this, html_form_input::changed);
    in_place.record_actor_on_event(this, html_form_input::changed);
    warn_over.record_actor_on_event(this, html_form_input::changed);
    flat.record_actor_on_event(this, html_form_input::changed);
    what_to_check.record_actor_on_event(this, html_form_select::changed);
    warn_remove_no_match.record_actor_on_event(this, html_form_input::changed);
    empty.record_actor_on_event(this, html_form_input::changed);
    empty_dir.record_actor_on_event(this, html_form_input::changed);
    dirty_behavior.record_actor_on_event(this, html_form_select::changed);
    ignore_sockets.record_actor_on_event(this, html_form_input::changed);
    info_details.record_actor_on_event(this, html_form_input::changed);
    display_treated.record_actor_on_event(this, html_form_input::changed);
    display_treated_only_dir.record_actor_on_event(this, html_form_input::changed);
    display_skipped.record_actor_on_event(this, html_form_input::changed);
    overwriting_policy->record_actor_on_event(this, html_form_overwrite_action::changed);
    only_deleted.record_actor_on_event(this, html_form_input::changed);
    ignore_deleted.record_actor_on_event(this, html_form_input::changed);
    filename_mask->record_actor_on_event(this, html_mask_form_filename::changed);
    path_mask->record_actor_on_event(this, html_mask_form_path::changed);
    ea_mask->record_actor_on_event(this, html_mask_form_filename::changed);
    fsa_scope.record_actor_on_event(this, html_fsa_scope::changed);

    register_name(changed);
	// css

    webdar_css_style::grey_button(deroule, true);
    display_treated_only_dir.add_css_class(webdar_css_style::indent);

	// components visibility status
    on_event(html_form_input::changed);

	// tooltips
    in_place.set_tooltip(TOOLTIP_HOE_IN_PLACE);
    warn_over.set_tooltip(TOOLTIP_HOE_WARN_OVER);
    warn_remove_no_match.set_tooltip(TOOLTIP_HOE_WARN_NO_MATCH);
    flat.set_tooltip(TOOLTIP_HOE_FLAT);
    what_to_check.set_tooltip(TOOLTIP_HOE_WTC);
    empty_dir.set_tooltip(TOOLTIP_HOE_EMPTY_DIR);
    dirty_behavior.set_tooltip(TOOLTIP_HOE_DIRTY);
    only_deleted.set_tooltip(TOOLTIP_HOE_ONLY_DELETED);
    ignore_deleted.set_tooltip(TOOLTIP_HOE_IGNORE_DELETED);
    ignore_sockets.set_tooltip(TOOLTIP_HOE_IGNORE_SOCKETS);
    empty.set_tooltip(TOOLTIP_HOE_EMPTY);
    info_details.set_tooltip(TOOLTIP_HOE_INFO_DETAILS);
    display_treated.set_tooltip(TOOLTIP_HOE_DISPLAY_TREATED);
    display_treated_only_dir.set_tooltip(TOOLTIP_HOE_DISPLAY_ONLY_DIR);
    display_skipped.set_tooltip(TOOLTIP_HOE_DISPLAY_SKIPPED);
}

void html_options_extract::set_biblio(const shared_ptr<bibliotheque> & ptr)
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
    guichet_overwriting_policy.set_child(ptr,
					 bibliotheque::over_policy,
					 overwriting_policy,
					 true);
    init();
}

libdar::archive_options_extract html_options_extract::get_options() const
{
    libdar::archive_options_extract ret;

    ret.set_in_place(in_place.get_value_as_bool());
    ret.set_warn_over(warn_over.get_value_as_bool());
    ret.set_flat(flat.get_value_as_bool());
    if(what_to_check.get_selected_id() == "all")
	ret.set_what_to_check(libdar::comparison_fields::all);
    else if(what_to_check.get_selected_id() == "ignore_owner")
	ret.set_what_to_check(libdar::comparison_fields::ignore_owner);
    else if(what_to_check.get_selected_id() == "mtime")
	ret.set_what_to_check(libdar::comparison_fields::mtime);
    else if(what_to_check.get_selected_id() == "inode_type")
	ret.set_what_to_check(libdar::comparison_fields::inode_type);
    else
	throw WEBDAR_BUG;
    ret.set_warn_remove_no_match(warn_remove_no_match.get_value_as_bool());
    ret.set_empty(empty.get_value_as_bool());

    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_display_treated(display_treated.get_value_as_bool(),
			    display_treated_only_dir.get_value_as_bool());
    ret.set_display_skipped(display_skipped.get_value_as_bool());

    if(dirty_behavior.get_selected_id() == "ignore")
	ret.set_dirty_behavior(libdar::archive_options_extract::dirty_ignore);
    else if(dirty_behavior.get_selected_id() == "warn")
	ret.set_dirty_behavior(libdar::archive_options_extract::dirty_warn);
    else if(dirty_behavior.get_selected_id() == "ok")
	ret.set_dirty_behavior(libdar::archive_options_extract::dirty_ok);
    else
	throw WEBDAR_BUG;
    ret.set_only_deleted(only_deleted.get_value_as_bool());
    ret.set_ignore_deleted(ignore_deleted.get_value_as_bool());
    ret.set_ignore_unix_sockets(ignore_sockets.get_value_as_bool());

    ret.set_selection(*(filename_mask->get_mask()));
    ret.set_subtree(*(path_mask->get_mask()));
    ret.set_ea_mask(*(ea_mask->get_mask()));
    ret.set_fsa_scope(fsa_scope.get_scope());
    ret.set_overwriting_rules(*(overwriting_policy->get_overwriting_action()));

    return ret;
}

void html_options_extract::load_json(const json & source)
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
	    in_place.set_value_as_bool(config.at(jlabel_in_place));
	    warn_over.set_value_as_bool(config.at(jlabel_warn_over));
	    flat.set_value_as_bool(config.at(jlabel_flat));
	    what_to_check.set_selected_id(config.at(jlabel_what_to_check));
	    warn_remove_no_match.set_value_as_bool(config.at(jlabel_warn_remove_no_match));
	    empty.set_value_as_bool(config.at(jlabel_empty));
	    empty_dir.set_value_as_bool(config.at(jlabel_empty_dir));
	    dirty_behavior.set_selected_id(config.at(jlabel_dirty_behavior));
	    ignore_sockets.set_value_as_bool(config.at(jlabel_ignore_sockets));
	    info_details.set_value_as_bool(config.at(jlabel_info_details));
	    display_treated.set_value_as_bool(config.at(jlabel_display_treated));
	    display_treated_only_dir.set_value_as_bool(config.at(jlabel_display_only_dir));
	    display_skipped.set_value_as_bool(config.at(jlabel_display_skipped));
	    guichet_overwriting_policy.load_json(config.at(jlabel_overwriting_policy));
	    only_deleted.set_value_as_bool(config.at(jlabel_only_deleted));
	    ignore_deleted.set_value_as_bool(config.at(jlabel_ignore_deleted));
	    guichet_filename_mask.load_json(config.at(jlabel_filename_mask));
	    guichet_path_mask.load_json(config.at(jlabel_path_mask));
	    guichet_ea_mask.load_json(config.at(jlabel_ea_mask));
	    fsa_scope.load_json(config.at(jlabel_fsa_scope));
	}
	catch(...)
	{
	    ignore_events = false;
	    throw;
	}
	ignore_events = false;
	trigger_changed();
    }
    catch(json::exception & e)
    {
	throw exception_json(libdar::tools_printf("Error loading %s config", myclass_id), e);
    }
}

json html_options_extract::save_json() const
{
    json config;

    config[jlabel_in_place] = in_place.get_value_as_bool();
    config[jlabel_warn_over] = warn_over.get_value_as_bool();
    config[jlabel_flat] = flat.get_value_as_bool();
    config[jlabel_what_to_check] = what_to_check.get_selected_id();
    config[jlabel_warn_remove_no_match] = warn_remove_no_match.get_value_as_bool();
    config[jlabel_empty] = empty.get_value_as_bool();
    config[jlabel_empty_dir] = empty_dir.get_value_as_bool();
    config[jlabel_dirty_behavior] = dirty_behavior.get_selected_id();
    config[jlabel_ignore_sockets] = ignore_sockets.get_value_as_bool();
    config[jlabel_info_details] = info_details.get_value_as_bool();
    config[jlabel_display_treated] = display_treated.get_value_as_bool();
    config[jlabel_display_only_dir] = display_treated_only_dir.get_value_as_bool();
    config[jlabel_display_skipped] = display_skipped.get_value_as_bool();
    config[jlabel_overwriting_policy] = guichet_overwriting_policy.save_json();
    config[jlabel_only_deleted] = only_deleted.get_value_as_bool();
    config[jlabel_ignore_deleted] = ignore_deleted.get_value_as_bool();
    config[jlabel_filename_mask] = guichet_filename_mask.save_json();
    config[jlabel_path_mask] = guichet_path_mask.save_json();
    config[jlabel_ea_mask] = guichet_ea_mask.save_json();
    config[jlabel_fsa_scope] = fsa_scope.save_json();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					config);
}

void html_options_extract::clear_json()
{
    ignore_events = true;
    try
    {
	init();
    }
    catch(...)
    {
	ignore_events = false;
	throw;
    }
    ignore_events = false;
    trigger_changed();
}

bibliotheque::using_set html_options_extract::get_using_set() const
{
    bibliotheque::using_set ret;

    subconfig_add_to(ret, guichet_filename_mask.get_using_set());
    subconfig_add_to(ret, guichet_path_mask.get_using_set());
    subconfig_add_to(ret, guichet_ea_mask.get_using_set());
    subconfig_add_to(ret, guichet_overwriting_policy.get_using_set());

    return ret;
}

void html_options_extract::on_event(const string & event_name)
{
    if(event_name == html_form_input::changed)
    {
	display_treated_only_dir.set_visible(display_treated.get_value_as_bool());
	trigger_changed();
    }
    else if(event_name == html_form_select::changed
	    || event_name == html_form_overwrite_action::changed
	    || event_name == html_mask_form_filename::changed
	    || event_name == html_mask_form_path::changed
	    || event_name == html_fsa_scope::changed)
    {
	trigger_changed();
    }
    else
	throw WEBDAR_BUG;
}

void html_options_extract::set_to_webdar_defaults()
{
    clear_json();

    in_place.set_value_as_bool(false);
    warn_over.set_value_as_bool(true);
    empty_dir.set_value_as_bool(true);
    dirty_behavior.set_selected_id("warn");
    ignore_sockets.set_value_as_bool(true);
    info_details.set_value_as_bool(true);
    display_treated.set_value_as_bool(true);
    display_treated_only_dir.set_value_as_bool(false);
    display_skipped.set_value_as_bool(true);
    guichet_overwriting_policy.load_from_bibliotheque(bibliotheque::default_config_name);
}

string html_options_extract::inherited_get_body_part(const chemin & path,
						     const request & req)
{
    return get_body_part_from_all_children(path, req);
}


void html_options_extract::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}

void html_options_extract::init()
{
    libdar::archive_options_extract defaults;

    in_place.set_value_as_bool(defaults.get_in_place());
    warn_over.set_value_as_bool(defaults.get_warn_over());
    flat.set_value_as_bool(defaults.get_flat());

    switch(defaults.get_what_to_check())
    {
    case libdar::comparison_fields::all:
	what_to_check.set_selected_id("all");
	break;
    case libdar::comparison_fields::ignore_owner:
	what_to_check.set_selected_id("ignore_owner");
	break;
    case libdar::comparison_fields::mtime:
	what_to_check.set_selected_id("mtime");
	break;
    case libdar::comparison_fields::inode_type:
	what_to_check.set_selected_id("inode_type");
	break;
    default:
	throw WEBDAR_BUG;
    }

    warn_remove_no_match.set_value_as_bool(defaults.get_warn_remove_no_match());
    empty.set_value_as_bool(defaults.get_empty());
    empty_dir.set_value_as_bool(defaults.get_empty_dir());

    switch(defaults.get_dirty_behavior())
    {
    case libdar::archive_options_extract::dirty_ignore:
	dirty_behavior.set_selected_num(0);
	break;
    case libdar::archive_options_extract::dirty_warn:
	dirty_behavior.set_selected_num(1);
	break;
    case libdar::archive_options_extract::dirty_ok:
	dirty_behavior.set_selected_num(2);
	break;
    default:
	throw WEBDAR_BUG;
    }

    ignore_sockets.set_value_as_bool(defaults.get_ignore_unix_sockets());
    info_details.set_value_as_bool(defaults.get_info_details());
    display_treated.set_value_as_bool(defaults.get_display_treated());
    display_treated_only_dir.set_value_as_bool(defaults.get_display_treated_only_dir());
    display_skipped.set_value_as_bool(defaults.get_display_skipped());

    guichet_overwriting_policy.clear_json();
    only_deleted.set_value_as_bool(defaults.get_only_deleted());
    ignore_deleted.set_value_as_bool(defaults.get_ignore_deleted());

    guichet_filename_mask.clear_json();
    guichet_path_mask.clear_json();
    guichet_ea_mask.clear_json();
    fsa_scope.clear_json();
}

void html_options_extract::trigger_changed()
{
    if(! ignore_events)
	act(changed);
}
