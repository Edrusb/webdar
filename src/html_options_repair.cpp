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
#include "webdar_css_style.hpp"
#include "tokens.hpp"
#include "html_form_mask_expression.hpp"

    //
#include "html_options_repair.hpp"

using namespace std;

const string html_options_repair::entrepot_changed = "html_options_repair_entrep_changed";
const string html_options_repair::changed = "html_options_repair_changed";

const string html_options_repair::css_indent = "html_options_repair_css_indent";


html_options_repair::html_options_repair():
    info_details("Detailed information",
		 html_form_input::check,
		 "1", // checked
		 "1"),
    display_treated("Display treated entries",
		    html_form_input::check,
		    "1", // checked
		    "1"),
    display_only_dir("Display only treated dirs",
		     html_form_input::check,
		     "",
		     "1"),
    display_skipped("Display skipped entries",
		    html_form_input::check,
		    "1", // checked
		    "1"),
    display_dirstats("Show summary after each directory",
		     html_form_input::check,
		     "", // unchecked
		     "1"),
    display_fs(""),
    display_form("Update"),
    allow_over("Allow slice overwriting",
	       html_form_input::check,
	       "1",
	       "1"),
    warn_over("Warn before overwriting",
	      html_form_input::check,
	      "1",
	      "1"),
    pause("Pause at each N slices (zero = no pause)",
	  html_form_input::number,
	  "0",
	  "5"),
    execute("Command to execute after each slice",
	    html_form_input::text,
	    "",
	    "80%"),
    dry_run("Dry-run execution",
	    html_form_input::check,
	    "",
	    "1"),
    multi_thread_compress("Number of thread for compression",
			  html_form_input::number,
			  "1",
			  "5"),
    hash_algo("Generate hashing files for each slice"),
    processing_fs(""),
    processing_form("Update"),
    user_comment("User comment in slice header",
		 html_form_input::text,
		 "",
		 "80%"),
    target_fs(""),
    target_form("Update"),
    ignore_events(false)
{
    entrep.reset(new (nothrow) html_entrepot());
    if(!entrep)
	throw exception_memory();

    slicing.reset(new (nothrow) html_slicing());
    if(! slicing)
	throw exception_memory();

    ciphering.reset(new (nothrow) html_ciphering());
    if(! ciphering)
	throw exception_memory();

	// component configuration
    static const char* sect_entrep = "entrepot";
    static const char* sect_display = "display";
    static const char* sect_process = "process";
    static const char* sect_target = "target";
    static const char* sect_slice = "slicing";
    static const char* sect_crypt = "crypto";

    deroule.add_section(sect_entrep, "Repaired Catalog Repository");
    deroule.add_section(sect_process, "How to process the operation");
    deroule.add_section(sect_display, "What to show during the operation");
    deroule.add_section(sect_target, "Target properties");
    deroule.add_section(sect_slice, "Slicing options");
    deroule.add_section(sect_crypt, "Encryption options");

    libdar::archive_options_repair defaults;
    ciphering->set_crypto_size_range(defaults.get_crypto_size(), libdar::infinint(4294967296)); // max is 2^32
    pause.set_min_only(0);
    multi_thread_compress.set_min_only(1);

	// default values
	// this is set in set_biblio()

	// repo adoption
    deroule.adopt_in_section(sect_entrep, &guichet_entrep);

	// adoption tree
    display_fs.adopt(&info_details);
    display_fs.adopt(&display_treated);
    display_fs.adopt(&display_only_dir);
    display_fs.adopt(&display_skipped);
    display_fs.adopt(&display_dirstats);
    display_form.adopt(&display_fs);
    deroule.adopt_in_section(sect_display, &display_form);

    processing_fs.adopt(&allow_over);
    processing_fs.adopt(&warn_over);
    processing_fs.adopt(&pause);
    processing_fs.adopt(&execute);
    processing_fs.adopt(&dry_run);
    processing_fs.adopt(&multi_thread_compress);
    processing_fs.adopt(&hash_algo);
    processing_form.adopt(&processing_fs);
    deroule.adopt_in_section(sect_process, &processing_form);

    target_fs.adopt(&user_comment);
    target_form.adopt(&target_fs);
    deroule.adopt_in_section(sect_target, &target_form);

    deroule.adopt_in_section(sect_slice, &guichet_slicing);

    deroule.adopt_in_section(sect_crypt, &guichet_ciphering);

    adopt(&deroule);

	// events

    register_name(entrepot_changed);
    register_name(changed);

    entrep->record_actor_on_event(this, html_entrepot::changed);
    display_treated.record_actor_on_event(this, html_form_input::changed);

    info_details.record_actor_on_event(this, html_form_input::changed);
    display_only_dir.record_actor_on_event(this, html_form_input::changed);
    display_skipped.record_actor_on_event(this, html_form_input::changed);
    display_dirstats.record_actor_on_event(this, html_form_input::changed);
    allow_over.record_actor_on_event(this, html_form_input::changed);
    warn_over.record_actor_on_event(this, html_form_input::changed);
    pause.record_actor_on_event(this, html_form_input::changed);
    execute.record_actor_on_event(this, html_form_input::changed);
    dry_run.record_actor_on_event(this, html_form_input::changed);
    multi_thread_compress.record_actor_on_event(this, html_form_input::changed);
    hash_algo.record_actor_on_event(this, html_hash_algo::changed);
    user_comment.record_actor_on_event(this, html_form_input::changed);
    slicing->record_actor_on_event(this, html_slicing::changed);
    ciphering->record_actor_on_event(this, html_ciphering::changed);

    on_event(html_form_input::changed); // first initialization

	// css

    display_only_dir.add_css_class(css_indent);
    webdar_css_style::grey_button(deroule, true);
}

void html_options_repair::set_biblio(const std::shared_ptr<bibliotheque> & ptr)
{
    guichet_entrep.set_child(ptr,
			     bibliotheque::repo,
			     entrep,
			     false);
    guichet_slicing.set_child(ptr,
			      bibliotheque::slicing,
			      slicing,
			      false);
    guichet_ciphering.set_child(ptr,
				bibliotheque::ciphering,
				ciphering,
				false);

    init();
}

void html_options_repair::load_json(const json & source)
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
	    guichet_entrep.load_json(config.at(jlabel_entrep));
	    info_details.set_value_as_bool(config.at(jlabel_info_details));
	    display_treated.set_value_as_bool(config.at(jlabel_display_treated));
	    display_only_dir.set_value_as_bool(config.at(jlabel_display_only_dir));
	    display_skipped.set_value_as_bool(config.at(jlabel_display_skipped));
	    display_dirstats.set_value_as_bool(config.at(jlabel_display_dirstats));
	    allow_over.set_value_as_bool(config.at(jlabel_allow_over));
	    warn_over.set_value_as_bool(config.at(jlabel_warn_over));
	    pause.set_value_as_bool(config.at(jlabel_pause));
	    execute.set_value(config.at(jlabel_execute));
	    dry_run.set_value_as_bool(config.at(jlabel_dry_run));
	    multi_thread_compress.set_value_as_int(config.at(jlabel_multi_thread_compress));
	    hash_algo.set_selected_id(config.at(jlabel_hash_algo));
	    user_comment.set_value(config.at(jlabel_user_comment));
	    guichet_slicing.load_json(config.at(jlabel_slicing));
	    guichet_ciphering.load_json(config.at(jlabel_ciphering));
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

json html_options_repair::save_json() const
{
    json config;

    config[jlabel_entrep] = guichet_entrep.save_json();
    config[jlabel_info_details] = info_details.get_value_as_bool();
    config[jlabel_display_treated] = display_treated.get_value_as_bool();
    config[jlabel_display_only_dir] = display_only_dir.get_value_as_bool();
    config[jlabel_display_skipped] = display_skipped.get_value_as_bool();
    config[jlabel_display_dirstats] = display_dirstats.get_value_as_bool();
    config[jlabel_allow_over] = allow_over.get_value_as_bool();
    config[jlabel_warn_over] = warn_over.get_value_as_bool();
    config[jlabel_pause] = pause.get_value_as_bool();
    config[jlabel_execute] = execute.get_value();
    config[jlabel_dry_run] = dry_run.get_value_as_bool();
    config[jlabel_multi_thread_compress] = multi_thread_compress.get_value_as_int();
    config[jlabel_hash_algo] = hash_algo.get_selected_id();
    config[jlabel_user_comment] = user_comment.get_value();
    config[jlabel_slicing] = guichet_slicing.save_json();
    config[jlabel_ciphering] = guichet_ciphering.save_json();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					config);
}

void html_options_repair::clear_json()
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

bibliotheque::using_set html_options_repair::get_using_set() const
{
    bibliotheque::using_set ret;

    subconfig_add_to(ret, guichet_entrep.get_using_set());
    subconfig_add_to(ret, guichet_slicing.get_using_set());
    subconfig_add_to(ret, guichet_ciphering.get_using_set());

    return ret;
}

void html_options_repair::on_event(const string & event_name)
{
    if(event_name == html_form_input::changed)
    {
	display_only_dir.set_visible(display_treated.get_value_as_bool());
	trigger_changed();
    }
    else if(event_name == html_entrepot::changed)
    {
	act(entrepot_changed);
	trigger_changed();
    }
    else if(event_name == html_hash_algo::changed
	    || event_name == html_slicing::changed
	    || event_name == html_ciphering::changed)
    {
	trigger_changed();
    }
    else
	throw WEBDAR_BUG;
}


libdar::archive_options_repair html_options_repair::get_options(shared_ptr<html_web_user_interaction> & webui) const
{
    libdar::archive_options_repair ret;

    ret.set_allow_over(allow_over.get_value_as_bool());
    ret.set_warn_over(warn_over.get_value_as_bool());
    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_display_treated(display_treated.get_value_as_bool(), display_only_dir.get_value_as_bool());
    ret.set_display_skipped(display_skipped.get_value_as_bool());
    ret.set_display_finished(display_dirstats.get_value_as_bool());
    ret.set_pause(libdar::deci(pause.get_value()).computer());

    libdar::infinint s_size;
    libdar::infinint f_s_size;
    slicing->get_slicing(s_size, f_s_size);
    ret.set_slicing(s_size, f_s_size);

    ret.set_execute(execute.get_value());

    ret.set_crypto_algo(ciphering->get_crypto_algo());
    if(ciphering->get_crypto_algo() != libdar::crypto_algo::none)
    {
	switch(ciphering->get_crypto_type())
	{
	case html_ciphering::sym:
	    ret.set_crypto_pass(ciphering->get_crypto_pass());
	    ret.set_iteration_count(ciphering->get_iteration_count());
	    ret.set_kdf_hash(ciphering->get_kdf_hash());
	    break;
	case html_ciphering::asym:
	    ret.set_gnupg_recipients(ciphering->get_gnupg_recipients());
	    ret.set_gnupg_signatories(ciphering->get_gnupg_signatories());
	    break;
	default:
	    throw WEBDAR_BUG;
	}
	ret.set_crypto_size(ciphering->get_crypto_size());
	ret.set_multi_threaded_crypto(ciphering->get_multi_threaded_crypto());
    }

    ret.set_empty(dry_run.get_value_as_bool());
    ret.set_slice_permission(slicing->get_permission());
    ret.set_slice_user_ownership(slicing->get_user_ownership());
    ret.set_slice_group_ownership(slicing->get_group_ownership());
    ret.set_user_comment(user_comment.get_value());
    ret.set_hash_algo(hash_algo.get_value());
    ret.set_slice_min_digits(slicing->get_min_digits());
    ret.set_entrepot(entrep->get_entrepot(webui));
    ret.set_multi_threaded_compress(webdar_tools_convert_to_int(multi_thread_compress.get_value()));

    return ret;
}


string html_options_repair::inherited_get_body_part(const chemin & path,
						    const request & req)
{
    return get_body_part_from_all_children(path, req);
}


void html_options_repair::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);

    if(! csslib->class_exists(css_indent))
    {
	css tmp;

	tmp.css_margin_left("3em");
	csslib->add(css_indent, tmp);
    }
}

void html_options_repair::init()
{
    libdar::archive_options_repair defaults;

    guichet_entrep.clear_json();
    info_details.set_value_as_bool(defaults.get_info_details());
    display_treated.set_value_as_bool(defaults.get_display_treated());
    display_only_dir.set_value_as_bool(defaults.get_display_treated_only_dir());
    display_skipped.set_value_as_bool(defaults.get_display_skipped());
    display_dirstats.set_value_as_bool(defaults.get_display_finished());
    allow_over.set_value_as_bool(defaults.get_allow_over());
    warn_over.set_value_as_bool(defaults.get_warn_over());
    pause.set_value(libdar::deci(defaults.get_pause()).human());
    execute.set_value(defaults.get_execute());
    dry_run.set_value_as_bool(defaults.get_empty());
    multi_thread_compress.set_value_as_int(defaults.get_multi_threaded_compress());
    hash_algo.set_value(defaults.get_hash_algo());
    user_comment.set_value(defaults.get_user_comment());
    guichet_slicing.clear_json();
    guichet_ciphering.clear_json();
}

void html_options_repair::trigger_changed()
{
    if(! ignore_events)
	act(changed);
}
