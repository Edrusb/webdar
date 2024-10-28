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
#include "webdar_tools.hpp"
#include "webdar_css_style.hpp"
#include "tokens.hpp"
#include "html_form_mask_expression.hpp"

    //
#include "html_options_repair.hpp"

using namespace std;

const string html_options_repair::entrepot_changed = "html_options_repair_entrep_changed";

const string html_options_repair::css_indent = "html_options_repair_css_indent";


html_options_repair::html_options_repair():
    info_details("Detailed information",
		 html_form_input::check,
		 "1", // checked
		 1),
    display_treated("Display treated entries",
		    html_form_input::check,
		    "1", // checked
		    1),
    display_only_dir("Display only treated dirs",
		     html_form_input::check,
		     "",
		     1),
    display_skipped("Display skipped entries",
		    html_form_input::check,
		    "1", // checked
		    1),
    display_dirstats("Show summary after each directory",
		     html_form_input::check,
		     "", // unchecked
		     1),
    display_fs(""),
    display_form("Update"),
    allow_over("Allow slice overwriting",
	       html_form_input::check,
	       "1",
	       1),
    warn_over("Warn before overwriting",
	      html_form_input::check,
	      "1",
	      1),
    pause("Pause at each N slices (zero = no pause)",
	  html_form_input::number,
	  "0",
	  3),
    execute("Command to execute after each slice",
	    html_form_input::text,
	    "",
	    30),
    dry_run("Dry-run execution",
	    html_form_input::check,
	    "",
	    1),
    multi_thread_crypto("Number of thread for ciphering",
			html_form_input::number,
			"1",
			3),
    multi_thread_compress("Number of thread for compression",
			  html_form_input::number,
			  "1",
			  3),
    hash_algo("Generate hashing files for each slice"),
    processing_fs(""),
    processing_form("Update"),
    user_comment("User comment in slice header",
		 html_form_input::text,
		 "",
		 40),
    target_fs(""),
    target_form("Update"),
    crypto_algo("Cipher used"),
    crypto_pass1("Pass phrase",
		 html_form_input::password,
		 "",
		 30),
    crypto_pass2("Confirm pass phrase",
		 html_form_input::password,
		 "",
		 30),
    crypto_size("Cipher Block size",
		html_form_input::number,
		"",
		30),
    crypto_fs(""),
    crypto_form("Update")
{
    libdar::archive_options_repair defaults;

	// component configuration
    static const char* sect_display = "display";
    static const char* sect_process = "process";
    static const char* sect_target = "target";
    static const char* sect_slice = "slicing";
    static const char* sect_crypt = "crypto";

    deroule.add_section(sect_display, "What to show during the operation");
    deroule.add_section(sect_process, "How to process the operation");
    deroule.add_section(sect_target, "Target properties");
    deroule.add_section(sect_slice, "Slicing options");
    deroule.add_section(sect_crypt, "Encryption options");

    allow_over.set_value_as_bool(defaults.get_allow_over());
    warn_over.set_value_as_bool(defaults.get_warn_over());
    pause.set_value(libdar::deci(defaults.get_pause()).human());
    crypto_algo.set_value(defaults.get_crypto_algo());
    crypto_pass1.set_value("");
    crypto_pass2.set_value("");
    crypto_size.set_value(webdar_tools_convert_to_string(defaults.get_crypto_size()));

    pause.set_min_only(0);
    multi_thread_crypto.set_min_only(1);
    multi_thread_compress.set_min_only(1);

    slicing.set_permission(defaults.get_slice_permission());
    slicing.set_user_ownership(defaults.get_slice_user_ownership());
    slicing.set_group_ownership(defaults.get_slice_group_ownership());
    slicing.set_min_digits(defaults.get_slice_min_digits());
    slicing.set_slicing(defaults.get_slice_size(), defaults.get_first_slice_size());

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
    processing_fs.adopt(&multi_thread_crypto);
    processing_fs.adopt(&multi_thread_compress);
    processing_fs.adopt(&hash_algo);
    processing_form.adopt(&processing_fs);
    deroule.adopt_in_section(sect_process, &processing_form);

    target_fs.adopt(&user_comment);
    target_form.adopt(&target_fs);
    deroule.adopt_in_section(sect_target, &target_form);

    deroule.adopt_in_section(sect_slice, &slicing);

    crypto_fs.adopt(&crypto_algo);
    crypto_fs.adopt(&crypto_pass1);
    crypto_fs.adopt(&crypto_pass2);
    crypto_fs.adopt(&crypto_size);
    crypto_form.adopt(&crypto_fs);
    deroule.adopt_in_section(sect_crypt, &crypto_form);

    adopt(&deroule);

	// events

    register_name(entrepot_changed);

    display_treated.record_actor_on_event(this, html_form_input::changed);
    crypto_algo.record_actor_on_event(this, html_crypto_algo::changed);
    entrep.record_actor_on_event(this, html_entrepot::changed);

    on_event(html_form_input::changed); // first initialization

	// css

    display_only_dir.add_css_class(css_indent);
    webdar_css_style::grey_button(deroule, true);
}


void html_options_repair::on_event(const string & event_name)
{
    if(event_name == html_form_input::changed ||
       event_name == html_crypto_algo::changed)
    {
	display_only_dir.set_visible(display_treated.get_value_as_bool());

	switch(crypto_algo.get_value())
	{
	case libdar::crypto_algo::none:
	    crypto_pass1.set_visible(false);
	    crypto_pass2.set_visible(false);
	    crypto_size.set_visible(false);
	    break;
	case libdar::crypto_algo::scrambling:
	case libdar::crypto_algo::blowfish:
	case libdar::crypto_algo::aes256:
	case libdar::crypto_algo::twofish256:
	case libdar::crypto_algo::serpent256:
	case libdar::crypto_algo::camellia256:
	    crypto_pass1.set_visible(true);
	    crypto_pass2.set_visible(true);
	    crypto_size.set_visible(true);
	    break;
	default:
	    throw WEBDAR_BUG;
	}

    }
    else if(event_name == html_entrepot::changed)
    {
	throw WEBDAR_BUG; // a implementer
	act(entrepot_changed);
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
    slicing.get_slicing(s_size, f_s_size);
    ret.set_slicing(s_size, f_s_size);

    ret.set_execute(execute.get_value());
    ret.set_crypto_algo(crypto_algo.get_value());
    if(crypto_algo.get_value() != libdar::crypto_algo::none)
    {
	if(crypto_pass1.get_value() != crypto_pass2.get_value())
	    throw exception_range("crypto password and its confirmation do not match");
	ret.set_crypto_pass(libdar::secu_string(crypto_pass1.get_value().c_str(), crypto_pass1.get_value().size()));
	ret.set_crypto_size(webdar_tools_convert_to_int(crypto_size.get_value()));
    }
	// gnupg recipient not surfacing to GUI for now
	// gnupg signatories not surfacing to GUI for now
    ret.set_empty(dry_run.get_value_as_bool());
    ret.set_slice_permission(slicing.get_permission());
    ret.set_slice_user_ownership(slicing.get_user_ownership());
    ret.set_slice_group_ownership(slicing.get_group_ownership());
    ret.set_user_comment(user_comment.get_value());
    ret.set_hash_algo(hash_algo.get_value());
    ret.set_slice_min_digits(slicing.get_min_digits());
    ret.set_entrepot(entrep.get_entrepot(webui));
    ret.set_multi_threaded_crypto(webdar_tools_convert_to_int(multi_thread_crypto.get_value()));
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
