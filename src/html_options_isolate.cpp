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
    delta_mask("Filename expression"),
    form_archgen("Update"),
    fs_archgen(""),
    allow_over("Allow slice overwriting", html_form_input::check, "", 1),
    warn_over("Warn before overwriting", html_form_input::check, "", 1),
    pause("Pause at each N slices (zero = no pause)", html_form_input::number, "", 3),
    slice_permission("Slice permission (octal)", html_form_input::text, "", 10),
    slice_user_ownership("Slice user ownership", html_form_input::text, "", 10),
    slice_group_ownership("slice group ownership", html_form_input::text, "", 10),
    sequential_marks("Add sequential marks", html_form_input::check, "", 1),
    user_comment("User comment in slice header", html_form_input::text, "", 40),
    slice_min_digits("Minimum digits in slice filenames", html_form_input::number, "", 3),
    hash_algo("Hashing algorithm"),
    execute("Command to execute after each slice", html_form_input::text, "", 30),
    empty("Dry run execution", html_form_input::check, "", 1),
    form_shown("Update"),
    fs_shown(""),
    info_details("Detailed informations", html_form_input::check, "1", 1),
    form_compr("Update"),
    fs_compr(""),
    compression("Compression algorithm"),
    compression_level("Compression level", html_form_input::number, "", 3),
    compression_block("Block compression for parallel compression (zero to zero to disable)", html_form_input::number, "0", 30),
    compr_threads("Number of threads for compression", html_form_input::number, "2", 5),
    form_slicing("Update"),
    fs_slicing(""),
    slicing("Sliced archive", html_form_input::check, "", 1),
    slice_size("Slice size", html_form_input::number, "", 6),
    different_first_slice("Specific size for first slice", html_form_input::check, "", 1),
    first_slice_size("Slice size", html_form_input::number, "", 6),
    form_crypto("Update"),
    fs_crypto(""),
    crypto_type("type of cryptography"),
    crypto_algo("Cipher used"),
    crypto_pass1("Pass phrase", html_form_input::password, "", 30),
    crypto_pass2("Confirm pass phrase", html_form_input::password, "", 30),
    crypto_size("Cipher Block size", html_form_input::number, "", 30),
    crypto_threads("Number of threads for ciphering", html_form_input::number, "2", 5),
    crypto_fs_kdf_hash("Key Derivation Function"),
    iteration_count("Iteration count", html_form_input::number, "1", 30)
{
    libdar::archive_options_isolate defaults;

	// removing Carriage Return for some components
    slice_size.set_no_CR();
    first_slice_size.set_no_CR();
    compression_block.set_no_CR();

	// configure html components
    crypto_type.add_choice("sym", "Symmetric encryption");
    crypto_type.add_choice("asym", "Asymmetric encryption");
    crypto_kdf_hash.add_choice("md5","md5");
    crypto_kdf_hash.add_choice("sha1","sha1");
    crypto_kdf_hash.add_choice("sha512","sha512");
    crypto_kdf_hash.add_choice("whirlpool","whirlpool");
    crypto_kdf_hash.add_choice("argon2","argon2");
    crypto_kdf_hash.set_selected("argon2");


	// setting default values from libdar
    allow_over.set_value_as_bool(defaults.get_allow_over());
    warn_over.set_value_as_bool(defaults.get_warn_over());
    pause.set_value(libdar::deci(defaults.get_pause()).human());
    pause.set_min_only(0);
    slice_permission.set_value(defaults.get_slice_permission());
    slice_user_ownership.set_value(defaults.get_slice_user_ownership());
    slice_group_ownership.set_value(defaults.get_slice_group_ownership());
    sequential_marks.set_value_as_bool(defaults.get_sequential_marks());
    user_comment.set_value(defaults.get_user_comment());
    slice_min_digits.set_value(libdar::deci(defaults.get_slice_min_digits()).human());
    hash_algo.set_value(defaults.get_hash_algo());
    execute.set_value(defaults.get_execute());
    empty.set_value_as_bool(defaults.get_empty());
    compression.set_value(defaults.get_compression());
    compression_level.set_value(webdar_tools_convert_to_string(defaults.get_compression_level()));
    compression_block.set_value(webdar_tools_convert_to_string(defaults.get_compression_block_size()));
    slicing.set_value_as_bool(defaults.get_slice_size() != 0);
    slice_size.set_value(libdar::deci(defaults.get_slice_size()).human());
    different_first_slice.set_value_as_bool(defaults.get_first_slice_size() != defaults.get_slice_size());
    first_slice_size.set_value(libdar::deci(defaults.get_first_slice_size()).human());
    crypto_algo.set_value(defaults.get_crypto_algo());
    crypto_pass1.set_value("");
    crypto_pass2.set_value("");
    crypto_size.set_value(webdar_tools_convert_to_string(defaults.get_crypto_size()));
    compr_threads.set_min_only(1);
    crypto_threads.set_min_only(1);

    int tmp = 0;
    libdar::infinint tmpi = defaults.get_iteration_count();
    tmpi.unstack(tmp);
    if(! tmpi.is_zero())
	throw exception_range("Value provided to iteration count exceeds the supported libdar integer flavor (infinint)");
    else
    {
	iteration_count.set_min_only(tmp);
	iteration_count.set_value(webdar_tools_convert_to_string(tmp));
    }


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
    delta_fs.adopt(&sig_block_size);
    form_delta_sig.adopt(&delta_fs);
    deroule.adopt_in_section(sect_delta, &form_delta_sig);
    deroule.adopt_in_section(sect_delta, &delta_mask);

    fs_archgen.adopt(&allow_over);
    fs_archgen.adopt(&warn_over);
    fs_archgen.adopt(&pause);
    fs_archgen.adopt(&slice_permission);
    fs_archgen.adopt(&slice_user_ownership);
    fs_archgen.adopt(&slice_group_ownership);
    fs_archgen.adopt(&sequential_marks);
    fs_archgen.adopt(&user_comment);
    fs_archgen.adopt(&slice_min_digits);
    fs_archgen.adopt(&hash_algo);
    fs_archgen.adopt(&execute);
    fs_archgen.adopt(&empty);
    form_archgen.adopt(&fs_archgen);
    deroule.adopt_in_section(sect_general, &form_archgen);

    fs_shown.adopt(&info_details);
    form_shown.adopt(&fs_shown);
    deroule.adopt_in_section(sect_show, &form_shown);

    fs_compr.adopt(&compression);
    fs_compr.adopt(&compression_level);
    fs_compr.adopt(&compression_block);
    fs_compr.adopt(&compr_block_unit);
    fs_compr.adopt(&compr_threads);
    form_compr.adopt(&fs_compr);
    deroule.adopt_in_section(sect_compr, &form_compr);

    fs_slicing.adopt(&slicing);
    fs_slicing.adopt(&slice_size);
    fs_slicing.adopt(&slice_size_unit);
    fs_slicing.adopt(&different_first_slice);
    fs_slicing.adopt(&first_slice_size);
    fs_slicing.adopt(&first_slice_size_unit);
    form_slicing.adopt(&fs_slicing);
    deroule.adopt_in_section(sect_slice, &form_slicing);

    fs_crypto.adopt(&crypto_algo);
    fs_crypto.adopt(&crypto_type);
    fs_crypto.adopt(&crypto_pass1);
    fs_crypto.adopt(&crypto_pass2);
    fs_crypto.adopt(&gnupg);
    fs_crypto.adopt(&crypto_size);
    fs_crypto.adopt(&crypto_threads);
    form_crypto.adopt(&fs_crypto);
    crypto_fs_kdf_hash.adopt(&crypto_kdf_hash);
    crypto_fs_kdf_hash.adopt(&iteration_count);
    form_crypto.adopt(&crypto_fs_kdf_hash);
    deroule.adopt_in_section(sect_cipher, &form_crypto);

    adopt(&deroule);

	// events and visibility
    register_name(entrepot_changed);

    delta_sig.record_actor_on_event(this, html_form_input::changed);
    delta_transfer_mode.record_actor_on_event(this, html_form_input::changed);
    compression.record_actor_on_event(this, html_compression::changed);
    slicing.record_actor_on_event(this, html_form_input::changed);
    different_first_slice.record_actor_on_event(this, html_form_input::changed);
    crypto_algo.record_actor_on_event(this, html_crypto_algo::changed);
    crypto_type.record_actor_on_event(this, html_form_select::changed);
    entrep.record_actor_on_event(this, html_entrepot::changed);
    crypto_kdf_hash.set_change_event_name(kdf_algo_changed);
    crypto_kdf_hash.record_actor_on_event(this, kdf_algo_changed);

    on_event(html_compression::changed);

	// css
    webdar_css_style::grey_button(deroule, true);
}

void html_options_isolate::on_event(const string & event_name)
{
    if(event_name == html_compression::changed
       || event_name == html_form_input::changed
       || event_name == html_crypto_algo::changed
       || event_name == html_form_select::changed)
    {
	if(delta_sig.get_value_as_bool())
	{
	    delta_transfer_mode.set_visible(true);
	    sig_block_size.set_visible(delta_transfer_mode.get_value_as_bool());
	    delta_mask.set_visible(delta_transfer_mode.get_value_as_bool());
	}
	else
	{
	    delta_transfer_mode.set_visible(false);
	    sig_block_size.set_visible(false);
	    delta_mask.set_visible(false);
	}

	switch(compression.get_value())
	{
	case libdar::compression::none:
	    compression.set_no_CR(false);
	    compression_level.set_visible(false);
	    compression_block.set_visible(false);
	    compr_block_unit.set_visible(false);
	    compr_threads.set_visible(false);
	    break;
	case libdar::compression::lzo1x_1_15:
	case libdar::compression::lzo1x_1:
	    compression.set_no_CR(false);
	    compression_level.set_visible(false);
	    compression_block.set_visible(true);
	    compr_block_unit.set_visible(true);
	    compr_threads.set_visible(true);
	    break;
	case libdar::compression::gzip:
	case libdar::compression::bzip2:
	case libdar::compression::lzo:
	case libdar::compression::xz:
	case libdar::compression::zstd:
	case libdar::compression::lz4:
	    compression.set_no_CR(true);
	    compression_level.set_visible(true);
	    compression_block.set_visible(true);
	    compr_block_unit.set_visible(true);
	    compr_threads.set_visible(true);
	    break;
	default:
	    throw WEBDAR_BUG;
	}

	if(slicing.get_value_as_bool())
	{
	    slice_size.set_visible(true);
	    slice_size_unit.set_visible(true);
	    different_first_slice.set_visible(true);
	    if(different_first_slice.get_value_as_bool())
	    {
		first_slice_size.set_visible(true);
		first_slice_size_unit.set_visible(true);
	    }
	    else
	    {
		first_slice_size.set_visible(false);
		first_slice_size_unit.set_visible(false);
	    }
	}
	else // no slicing requested
	{
	    slice_size.set_visible(false);
	    slice_size_unit.set_visible(false);
	    different_first_slice.set_visible(false);
	    first_slice_size.set_visible(false);
	    first_slice_size_unit.set_visible(false);
	}

	switch(crypto_algo.get_value())
	{
	case libdar::crypto_algo::none:
	    crypto_type.set_visible(false);
	    crypto_pass1.set_visible(false);
	    crypto_pass2.set_visible(false);
	    crypto_size.set_visible(false);
	    gnupg.set_visible(false);
	    crypto_threads.set_visible(false);
	    crypto_fs_kdf_hash.set_visible(false);
	    break;
	case libdar::crypto_algo::scrambling:
	case libdar::crypto_algo::blowfish:
	case libdar::crypto_algo::aes256:
	case libdar::crypto_algo::twofish256:
	case libdar::crypto_algo::serpent256:
	case libdar::crypto_algo::camellia256:
	    crypto_type.set_visible(true);
	    switch(crypto_type.get_selected_num())
	    {
	    case 0: // symmetric
		crypto_pass1.set_visible(true);
		crypto_pass2.set_visible(true);
		gnupg.set_visible(false);
		crypto_fs_kdf_hash.set_visible(true);
		break;
	    case 1: // asymmetric
		crypto_pass1.set_visible(false);
		crypto_pass2.set_visible(false);
		gnupg.set_visible(true);
		crypto_fs_kdf_hash.set_visible(false);
		break;
	    default:
		throw WEBDAR_BUG;
	    }
	    crypto_size.set_visible(true);
	    crypto_threads.set_visible(true);
	    break;
	default:
	    throw WEBDAR_BUG;
	}

	    // no need to call my_body_part_has_changed()
	    // because changed done in on_event concern
	    // body_builder objects we have adopted
    }
    else if(event_name == html_entrepot::changed)
	act(entrepot_changed);
    else if(event_name == kdf_algo_changed)
    {
	libdar::infinint cur_it = libdar::deci(iteration_count.get_value()).computer();

	if(crypto_kdf_hash.get_selected_id() != "argon2")
	{
	    if(cur_it < libdar::default_iteration_count)
		iteration_count.set_value(libdar::deci(libdar::default_iteration_count).human());
	}
	else
	{
	    if(cur_it < libdar::default_iteration_count_argon2)
		throw WEBDAR_BUG;
		// this should be avoided thanks to the min value
		// set to iteration_count component

	    if(cur_it == libdar::default_iteration_count)
		iteration_count.set_value(libdar::deci(libdar::default_iteration_count_argon2).human());
		// we will not change back to lower value when swapping back to argon2
		// unless the value is the default one (thus user can change the hash
		// algorithm and stick to the default value for the selected hash algo).
	}
    }
    else
	throw WEBDAR_BUG;
}


libdar::archive_options_isolate html_options_isolate::get_options(shared_ptr<html_web_user_interaction> & webui) const
{
    libdar::archive_options_isolate ret;
    libdar::infinint compr_bs = libdar::deci(compression_block.get_value()).computer() * compr_block_unit.get_value();
    libdar::U_I val = 0;

    ret.set_entrepot(entrep.get_entrepot(webui));
    ret.set_allow_over(allow_over.get_value_as_bool());
    ret.set_warn_over(warn_over.get_value_as_bool());
    ret.set_pause(libdar::deci(pause.get_value()).computer());
    ret.set_slice_permission(slice_permission.get_value());
    ret.set_slice_user_ownership(slice_user_ownership.get_value());
    ret.set_slice_group_ownership(slice_group_ownership.get_value());
    ret.set_sequential_marks(sequential_marks.get_value_as_bool());
    ret.set_user_comment(user_comment.get_value());
    ret.set_slice_min_digits(libdar::deci(slice_min_digits.get_value()).computer());
    ret.set_hash_algo(hash_algo.get_value());
    ret.set_empty(empty.get_value_as_bool());
    ret.set_execute(execute.get_value());
    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_compression(compression.get_value());
    ret.set_compression_level(webdar_tools_convert_to_int(compression_level.get_value()));
    ret.set_multi_threaded_compress(webdar_tools_convert_to_int(compr_threads.get_value()));
    ret.set_multi_threaded_crypto(webdar_tools_convert_to_int(crypto_threads.get_value()));

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
    }

    val = 0;
    compr_bs.unstack(val);
    if(!compr_bs.is_zero())
	throw exception_range("compression block size is too large for the underlying operating system, please reduce");

    if(val < tokens_min_compr_bs && val != 0)
	throw exception_range(libdar::tools_printf("compression block size is too small, select either zero to disable compression per block or a block size greater or equal to %d", tokens_min_compr_bs));
    ret.set_compression_block_size(val);

    if(slicing.get_value_as_bool())
    {
	libdar::infinint s_size = libdar::deci(slice_size.get_value()).computer() * slice_size_unit.get_value();
	libdar::infinint f_s_size = 0;

	if(different_first_slice.get_value_as_bool())
	    f_s_size = libdar::deci(first_slice_size.get_value()).computer() * first_slice_size_unit.get_value();

	ret.set_slicing(s_size, f_s_size);
    }

    ret.set_crypto_algo(crypto_algo.get_value());
    if(crypto_algo.get_value() != libdar::crypto_algo::none)
    {
	switch(crypto_type.get_selected_num())
	{
	case 0: // sym
	    if(crypto_pass1.get_value() != crypto_pass2.get_value())
		throw exception_range("crypto password and its confirmation do not match");
	    ret.set_crypto_pass(libdar::secu_string(crypto_pass1.get_value().c_str(), crypto_pass1.get_value().size()));
	    ret.set_iteration_count(libdar::deci(iteration_count.get_value()).computer());
	    switch(crypto_kdf_hash.get_selected_num())
	    {
	    case 0:
		ret.set_kdf_hash(libdar::hash_algo::md5);
		break;
	    case 1:
		ret.set_kdf_hash(libdar::hash_algo::sha1);
		break;
	    case 2:
		ret.set_kdf_hash(libdar::hash_algo::sha512);
		break;
	    case 3:
		ret.set_kdf_hash(libdar::hash_algo::whirlpool);
		break;
	    case 4:
		ret.set_kdf_hash(libdar::hash_algo::argon2);
		break;
	    default:
		throw WEBDAR_BUG;
	    }
	    break;
	case 1: // asym
	    ret.set_gnupg_recipients(gnupg.get_gnupg_recipients());
	    ret.set_gnupg_signatories(gnupg.get_gnupg_signatories());
	    break;
	default:
	    throw WEBDAR_BUG;
	}
	ret.set_crypto_size(webdar_tools_convert_to_int(crypto_size.get_value()));
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
