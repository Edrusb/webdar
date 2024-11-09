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
#include <dar/libdar.hpp>
#include <dar/tools.hpp>

    // webdar headers
#include "webdar_css_style.hpp"
#include "webdar_tools.hpp"
#include "tokens.hpp"

    //
#include "html_options_isolate.hpp"

using namespace std;

const string html_options_isolate::entrepot_changed = "html_options_isolate_entrep_changed";

html_options_isolate::html_options_isolate():
    form_delta_sig("Update"),
    delta_fs(""),
    delta_sig("Transfer binary delta signature", html_form_input::check, "", 1),
    delta_transfer_mode("Compute delta signature when they are missing", html_form_input::check, "", 1),
    delta_sig_min_size("Avoid calculating delta signature for file smaller than", 0, 30),
    delta_mask("file name"),
    form_archgen("Update"),
    fs_archgen(""),
    allow_over("Allow slice overwriting", html_form_input::check, "", 1),
    warn_over("Warn before overwriting", html_form_input::check, "", 1),
    pause("Pause at each N slices (zero = no pause)", html_form_input::number, "", 3),
    sequential_marks("Add sequential marks", html_form_input::check, "", 1),
    user_comment("User comment in slice header", html_form_input::text, "", 40),
    hash_algo("Hashing algorithm"),
    execute("Command to execute after each slice", html_form_input::text, "", 30),
    empty("Dry run execution", html_form_input::check, "", 1),
    form_shown("Update"),
    fs_shown(""),
    info_details("Detailed informations", html_form_input::check, "1", 1),
    compr_params(false, false, false)
{
    libdar::archive_options_isolate defaults;

	// configure html components

	// setting default values from libdar
    allow_over.set_value_as_bool(defaults.get_allow_over());
    warn_over.set_value_as_bool(defaults.get_warn_over());
    pause.set_value(libdar::deci(defaults.get_pause()).human());
    pause.set_min_only(0);
    slicing.set_permission(defaults.get_slice_permission());
    slicing.set_user_ownership(defaults.get_slice_user_ownership());
    slicing.set_group_ownership(defaults.get_slice_group_ownership());
    sequential_marks.set_value_as_bool(defaults.get_sequential_marks());
    user_comment.set_value(defaults.get_user_comment());
    slicing.set_min_digits(defaults.get_slice_min_digits());
    hash_algo.set_value(defaults.get_hash_algo());
    execute.set_value(defaults.get_execute());
    empty.set_value_as_bool(defaults.get_empty());
    compr_params.set_compression_algo(defaults.get_compression());
    compr_params.set_compression_level(defaults.get_compression_level());
    compr_params.set_compression_block(defaults.get_compression_block_size());
    slicing.set_slicing(defaults.get_slice_size(),
			defaults.get_first_slice_size());
    ciphering.set_crypto_size_range(defaults.get_crypto_size(), libdar::infinint(4294967296)); // max is 2^32
    delta_sig_min_size.set_value_as_infinint(defaults.get_delta_sig_min_size());

	// building HTML structure

    static const char* sect_entrep = "entrepot";
    static const char* sect_delta = "delta_sig";
    static const char* sect_general = "general";
    static const char* sect_show = "show";
    static const char* sect_compr = "compress";
    static const char* sect_slice = "slicing";
    static const char* sect_cipher = "ciphering";

    deroule.add_section(sect_entrep, "Isolated Catalog Repository");
    deroule.add_section(sect_delta, "Delta signatures");
    deroule.add_section(sect_general, "General Isolation Options");
    deroule.add_section(sect_show, "What to show during the operation");
    deroule.add_section(sect_compr, "Compression options");
    deroule.add_section(sect_slice, "Slicing options");
    deroule.add_section(sect_cipher, "Encryption options");

    deroule.adopt_in_section(sect_entrep, &entrep);

    delta_fs.adopt(&delta_sig);
    delta_fs.adopt(&delta_transfer_mode);
    delta_fs.adopt(&delta_sig_min_size);
    delta_fs.adopt(&sig_block_size);
    form_delta_sig.adopt(&delta_fs);
    deroule.adopt_in_section(sect_delta, &form_delta_sig);
    deroule.adopt_in_section(sect_delta, &delta_mask);

    fs_archgen.adopt(&allow_over);
    fs_archgen.adopt(&warn_over);
    fs_archgen.adopt(&pause);
    fs_archgen.adopt(&sequential_marks);
    fs_archgen.adopt(&user_comment);
    fs_archgen.adopt(&hash_algo);
    fs_archgen.adopt(&execute);
    fs_archgen.adopt(&empty);
    form_archgen.adopt(&fs_archgen);
    deroule.adopt_in_section(sect_general, &form_archgen);

    fs_shown.adopt(&info_details);
    form_shown.adopt(&fs_shown);
    deroule.adopt_in_section(sect_show, &form_shown);

    deroule.adopt_in_section(sect_compr, &compr_params);

    deroule.adopt_in_section(sect_slice, &slicing);

    deroule.adopt_in_section(sect_cipher, &ciphering);

    adopt(&deroule);

	// events and visibility
    register_name(entrepot_changed);

    delta_sig.record_actor_on_event(this, html_form_input::changed);
    delta_transfer_mode.record_actor_on_event(this, html_form_input::changed);
    entrep.record_actor_on_event(this, html_entrepot::changed);

    on_event(html_form_input::changed);

	// css
    webdar_css_style::grey_button(deroule, true);
}

void html_options_isolate::on_event(const string & event_name)
{
    if(event_name == html_form_input::changed
       || event_name == html_form_select::changed)
    {
	if(delta_sig.get_value_as_bool())
	{
	    delta_transfer_mode.set_visible(true);
	    delta_sig_min_size.set_visible(delta_transfer_mode.get_value_as_bool());
	    sig_block_size.set_visible(delta_transfer_mode.get_value_as_bool());
	    delta_mask.set_visible(delta_transfer_mode.get_value_as_bool());
	}
	else
	{
	    delta_transfer_mode.set_visible(false);
	    delta_sig_min_size.set_visible(false);
	    sig_block_size.set_visible(false);
	    delta_mask.set_visible(false);
	}

	    // no need to call my_body_part_has_changed()
	    // because changed done in on_event concern
	    // body_builder objects we have adopted
    }
    else if(event_name == html_entrepot::changed)
	act(entrepot_changed);
    else
	throw WEBDAR_BUG;
}


libdar::archive_options_isolate html_options_isolate::get_options(shared_ptr<html_web_user_interaction> & webui) const
{
    libdar::archive_options_isolate ret;
    libdar::infinint compr_bs = compr_params.get_compression_block();
    libdar::U_I val = 0;

    ret.set_entrepot(entrep.get_entrepot(webui));
    ret.set_allow_over(allow_over.get_value_as_bool());
    ret.set_warn_over(warn_over.get_value_as_bool());
    ret.set_pause(libdar::deci(pause.get_value()).computer());
    ret.set_slice_permission(slicing.get_permission());
    ret.set_slice_user_ownership(slicing.get_user_ownership());
    ret.set_slice_group_ownership(slicing.get_group_ownership());
    ret.set_sequential_marks(sequential_marks.get_value_as_bool());
    ret.set_user_comment(user_comment.get_value());
    ret.set_slice_min_digits(slicing.get_min_digits());
    ret.set_hash_algo(hash_algo.get_value());
    ret.set_empty(empty.get_value_as_bool());
    ret.set_execute(execute.get_value());
    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_compression(compr_params.get_compression_algo());
    ret.set_compression_level(compr_params.get_compression_level());
    ret.set_multi_threaded_compress(compr_params.get_num_threads());
    ret.set_multi_threaded_crypto(ciphering.get_multi_threaded_crypto());

    ret.set_delta_signature(delta_sig.get_value_as_bool());
    if(delta_sig.get_value_as_bool()
       && delta_transfer_mode.get_value_as_bool())
    {
	unique_ptr<libdar::mask> dmask = delta_mask.get_mask();

	if(dmask)
	    ret.set_delta_mask(*dmask);
	else
	    throw WEBDAR_BUG;

	ret.set_sig_block_len(sig_block_size.get_value());
	ret.set_delta_sig_min_size(delta_sig_min_size.get_value_as_infinint());
    }

    val = webdar_tools_convert_from_infinint<libdar::U_I>(compr_bs,
							  string("compression block size is too large for the underlying operating system, please reduce"));

    if(val < tokens_min_compr_bs && val != 0)
	throw exception_range(libdar::tools_printf("compression block size is too small, select either zero to disable compression per block or a block size greater or equal to %d", tokens_min_compr_bs));
    ret.set_compression_block_size(val);


    libdar::infinint s_size;
    libdar::infinint f_s_size;

    slicing.get_slicing(s_size, f_s_size);
    ret.set_slicing(s_size, f_s_size);

    ret.set_crypto_algo(ciphering.get_crypto_algo());
    if(ciphering.get_crypto_algo() != libdar::crypto_algo::none)
    {
	switch(ciphering.get_crypto_type())
	{
	case html_ciphering::sym:
	    ret.set_crypto_pass(ciphering.get_crypto_pass());
	    ret.set_iteration_count(ciphering.get_iteration_count());
	    ret.set_kdf_hash(ciphering.get_kdf_hash());
	    break;
	case html_ciphering::asym:
	    ret.set_gnupg_recipients(ciphering.get_gnupg_recipients());
	    ret.set_gnupg_signatories(ciphering.get_gnupg_signatories());
	    break;
	default:
	    throw WEBDAR_BUG;
	}
	ret.set_crypto_size(ciphering.get_crypto_size());
	ret.set_multi_threaded_crypto(ciphering.get_multi_threaded_crypto());
    }

    return ret;
}

string html_options_isolate::inherited_get_body_part(const chemin & path,
						     const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_options_isolate::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}
