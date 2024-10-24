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

    // webdar headers
#include "webdar_css_style.hpp"
#include "html_form_mask_expression.hpp"

    //
#include "html_options_merge.hpp"


using namespace std;

const string html_options_merge::entrepot_changed = "html_options_merge_entrep_changed";

html_options_merge::html_options_merge():
    form_archgen("Update"),
    fs_archgen(""),
    allow_over("Allow slice overwriting", html_form_input::check, "", 1),
    warn_over("Warn before overwriting", html_form_input::check, "", 1),
    pause("Pause at each N slices (zero = no pause)", html_form_input::number, "", 3),
    slice_permission("Slice permission (octal)", html_form_input::text, "", 10),
    slice_user_ownership("Slice user ownership", html_form_input::text, "", 10),
    slice_group_ownership("slice group ownership", html_form_input::text, "", 10),
    sequential_marks("Add sequential marks", html_form_input::check, "", 1),
    sparse_file_min_size("Minimum size of holes to lookup in sparse files", html_form_input::number, "", 10),
    user_comment("User comment in slice header", html_form_input::text, "", 40),
    slice_min_digits("Minimum digits in slice filenames", html_form_input::number, "", 3),
    hash_algo("Hashing algorithm"),
    execute("Command to execute after each slice", html_form_input::text, "", 30),
    empty("Dry run execution", html_form_input::check, "", 1),
    has_aux_fs(""),
    has_aux("Use an auxiliary archive", html_form_input::check, "", 1),
    aux_form("Update"),
    aux_block(""),
    auxiliary("Auxiliary archive of reference"),
    form_shown("Update"),
    fs_shown(""),
    info_details("Detailed informations", html_form_input::check, "1", 1),
    display_skipped("Display skipped files", html_form_input::check, "1", 1),
    form_perimeter("Update"),
    fs_perimeter(""),
    empty_dir("Store ignored directories as empty directories", html_form_input::check, "", 1),
    decremental_mode("Decremental mode", html_form_input::check, "", 1),
    filename_mask("Filename expression"),
    path_mask(true),
    overwriting_policy(""),
    form_overwriting("Update"),
    form_compr("Update"),
    fs_compr(""),
    keep_compressed("Keep file compressed", html_form_input::check, "", 1),
    compression("Compression algorithm"),
    compression_level("Compression level", html_form_input::range, "", 3),
    min_compr_size("Minimum file sized compressed", html_form_input::number, "", 30),
    form_slicing("Update"),
    fs_slicing(""),
    slicing("Sliced archive", html_form_input::check, "", 1),
    slice_size("Slice size", html_form_input::number, "", 6),
    different_first_slice("Specific size for first slice", html_form_input::check, "", 1),
    first_slice_size("Slice size", html_form_input::number, "", 6),
    form_crypto("Update"),
    fs_crypto(""),
    crypto_algo("Cipher used"),
    crypto_pass1("Pass phrase", html_form_input::password, "", 30),
    crypto_pass2("Confirm pass phrase", html_form_input::password, "", 30),
    crypto_size("Cipher Block size", html_form_input::number, "", 30)
{
    libdar::archive_options_merge defaults;

	// removing Carriage Return for some components

    compression.set_no_CR();
    slice_size.set_no_CR();
    first_slice_size.set_no_CR();
    min_compr_size.set_no_CR();
    sparse_file_min_size.set_no_CR();


	// setting default values from libdar

    allow_over.set_value_as_bool(defaults.get_allow_over());
    warn_over.set_value_as_bool(defaults.get_warn_over());
    pause.set_value(libdar::deci(defaults.get_pause()).human());
    slice_permission.set_value(defaults.get_slice_permission());
    slice_user_ownership.set_value(defaults.get_slice_user_ownership());
    slice_group_ownership.set_value(defaults.get_slice_group_ownership());
    sequential_marks.set_value_as_bool(defaults.get_sequential_marks());
    sparse_file_min_size.set_value(libdar::deci(defaults.get_sparse_file_min_size()).human());
    user_comment.set_value(defaults.get_user_comment());
    slice_min_digits.set_value(libdar::deci(defaults.get_slice_min_digits()).human());
    hash_algo.set_value(defaults.get_hash_algo());
    execute.set_value(defaults.get_execute());
    empty.set_value_as_bool(defaults.get_empty());

    empty_dir.set_value_as_bool(defaults.get_empty_dir());
    decremental_mode.set_value_as_bool(defaults.get_decremental_mode());

    keep_compressed.set_value_as_bool(defaults.get_keep_compressed());
    compression.set_value(defaults.get_compression());
    compression_level.set_value(webdar_tools_convert_to_string(defaults.get_compression_level()));
    min_compr_size.set_value(libdar::deci(defaults.get_min_compr_size()).human());

    slicing.set_value_as_bool(defaults.get_slice_size() != 0);
    slice_size.set_value(libdar::deci(defaults.get_slice_size()).human());
    different_first_slice.set_value_as_bool(defaults.get_first_slice_size() != defaults.get_slice_size());
    first_slice_size.set_value(libdar::deci(defaults.get_first_slice_size()).human());

    crypto_algo.set_value(defaults.get_crypto_algo());
    crypto_pass1.set_value("");
    crypto_pass2.set_value("");
    crypto_size.set_value(webdar_tools_convert_to_string(defaults.get_crypto_size()));

	// building HTML structure

    static const char* sect_entrep = "entrepot";
    static const char* sect_general = "general";
    static const char* sect_aux = "auxiliary";
    static const char* sect_show = "show";
    static const char* sect_filter = "filter";
    static const char* sect_mask_file = "mask_file";
    static const char* sect_mask_path = "mask_path";
    static const char* sect_overwrite = "overwrite";
    static const char* sect_compr = "slicing";
    static const char* sect_slice = "compression";
    static const char* sect_cipher = "ciphering";

    deroule.add_section(sect_entrep, "Backup Repository");
    deroule.add_section(sect_general, "General Merging Options");
    deroule.add_section(sect_aux, "Auxiliary Backup of Reference");
    deroule.add_section(sect_show, "What to show during the operation");
    deroule.add_section(sect_filter, "What to take into consideration for Merging");
    deroule.add_section(sect_mask_file, "Filename based filtering");
    deroule.add_section(sect_mask_path, "Path based filterting");
    deroule.add_section(sect_overwrite, "Overwriting policy");
    deroule.add_section(sect_compr, "Compression Options");
    deroule.add_section(sect_slice, "Slicing Options");
    deroule.add_section(sect_cipher, "Encryption Options");

    deroule.adopt_in_section(sect_entrep, &entrep);

    fs_archgen.adopt(&allow_over);
    fs_archgen.adopt(&warn_over);
    fs_archgen.adopt(&pause);
    fs_archgen.adopt(&slice_permission);
    fs_archgen.adopt(&slice_user_ownership);
    fs_archgen.adopt(&slice_group_ownership);
    fs_archgen.adopt(&sequential_marks);
    fs_archgen.adopt(&sparse_file_min_size);
    fs_archgen.adopt(&sparse_file_min_size_unit);
    fs_archgen.adopt(&user_comment);
    fs_archgen.adopt(&slice_min_digits);
    fs_archgen.adopt(&hash_algo);
    fs_archgen.adopt(&execute);
    fs_archgen.adopt(&empty);
    form_archgen.adopt(&fs_archgen);
    deroule.adopt_in_section(sect_general, &form_archgen);

    has_aux_fs.adopt(&has_aux);
    aux_form.adopt(&has_aux_fs);
    aux_block.adopt(&aux_form);
    aux_block.adopt(&auxiliary);
    deroule.adopt_in_section(sect_aux, &aux_block);

    fs_shown.adopt(&info_details);
    fs_shown.adopt(&display_skipped);
    form_shown.adopt(&fs_shown);
    deroule.adopt_in_section(sect_show, &form_shown);

    fs_perimeter.adopt(&empty_dir);
    fs_perimeter.adopt(&decremental_mode);
    form_perimeter.adopt(&fs_perimeter);
    deroule.adopt_in_section(sect_filter, &form_perimeter);

    deroule.adopt_in_section(sect_mask_file, &filename_mask);
    deroule.adopt_in_section(sect_mask_path, &path_mask);

    form_overwriting.adopt(&overwriting_policy);
    deroule.adopt_in_section(sect_overwrite, &form_overwriting);

    fs_compr.adopt(&keep_compressed);
    fs_compr.adopt(&compression);
    fs_compr.adopt(&compression_level);
    fs_compr.adopt(&min_compr_size);
    fs_compr.adopt(&min_compr_size_unit);
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
    fs_crypto.adopt(&crypto_pass1);
    fs_crypto.adopt(&crypto_pass2);
    fs_crypto.adopt(&crypto_size);
    form_crypto.adopt(&fs_crypto);
    deroule.adopt_in_section(sect_cipher, &form_crypto);

    adopt(&deroule);

	// events and visibility
    register_name(entrepot_changed);

    keep_compressed.record_actor_on_event(this, html_form_input::changed);
    compression.record_actor_on_event(this, html_compression::changed);
    slicing.record_actor_on_event(this, html_form_input::changed);
    different_first_slice.record_actor_on_event(this, html_form_input::changed);
    has_aux.record_actor_on_event(this, html_form_input::changed);
    crypto_algo.record_actor_on_event(this, html_crypto_algo::changed);
    entrep.record_actor_on_event(this, html_entrepot::changed);

    on_event(html_form_input::changed);

	// CSS
    webdar_css_style::grey_button(deroule, true);
}


void html_options_merge::on_event(const string & event_name)
{
    if(event_name == html_form_input::changed
       || event_name == html_compression::changed
       || event_name == html_crypto_algo::changed)
    {
	auxiliary.set_visible(has_aux.get_value_as_bool());

	if(keep_compressed.get_value_as_bool())
	{
	    compression.set_visible(false);
	    compression_level.set_visible(false);
	    min_compr_size.set_visible(false);
	    min_compr_size_unit.set_visible(false);
	}
	else
	{
	    compression.set_visible(true);
	    switch(compression.get_value())
	    {
	    case libdar::compression::none:
		compression_level.set_visible(false);
		min_compr_size.set_visible(false);
		min_compr_size_unit.set_visible(false);
		break;
	    case libdar::compression::gzip:
	    case libdar::compression::bzip2:
	    case libdar::compression::lzo:
		compression_level.set_visible(true);
		min_compr_size.set_visible(true);
		min_compr_size_unit.set_visible(true);
		break;
	    default:
		throw WEBDAR_BUG;
	    }
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

	    // no need to call my_body_part_has_changed()
	    // because changed done in on_event concern
	    // body_builder objects we have adopted
    }
    else if(event_name == html_entrepot::changed)
    {
	act(entrepot_changed);
    }
    else
	throw WEBDAR_BUG;
}


libdar::archive_options_merge html_options_merge::get_options(shared_ptr<html_web_user_interaction> & webui) const
{
    libdar::archive_options_merge ret;
    shared_ptr<libdar::archive> aux;

    ret.set_entrepot(entrep.get_entrepot(webui));
    ret.set_allow_over(allow_over.get_value_as_bool());
    ret.set_warn_over(warn_over.get_value_as_bool());
    ret.set_pause(libdar::deci(pause.get_value()).computer());
    ret.set_slice_permission(slice_permission.get_value());
    ret.set_slice_user_ownership(slice_user_ownership.get_value());
    ret.set_slice_group_ownership(slice_group_ownership.get_value());
    ret.set_sequential_marks(sequential_marks.get_value_as_bool());
    ret.set_sparse_file_min_size(libdar::deci(sparse_file_min_size.get_value()).computer() * sparse_file_min_size_unit.get_value());
    ret.set_user_comment(user_comment.get_value());
    ret.set_slice_min_digits(libdar::deci(slice_min_digits.get_value()).computer());
    ret.set_hash_algo(hash_algo.get_value());
    ret.set_empty(empty.get_value_as_bool());
    ret.set_execute(execute.get_value());
    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_display_skipped(display_skipped.get_value_as_bool());
    ret.set_empty_dir(empty_dir.get_value_as_bool());
    ret.set_decremental_mode(decremental_mode.get_value_as_bool());
    ret.set_selection(*(filename_mask.get_mask()));
    ret.set_subtree(*(path_mask.get_mask()));
    ret.set_overwriting_rules(*(overwriting_policy.get_overwriting_action()));
    ret.set_keep_compressed(keep_compressed.get_value_as_bool());
    ret.set_compression(compression.get_value());
    ret.set_compression_level(webdar_tools_convert_to_int(compression_level.get_value()));
    ret.set_min_compr_size(libdar::deci(min_compr_size.get_value()).computer() * min_compr_size_unit.get_value());

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
	if(crypto_pass1.get_value() != crypto_pass2.get_value())
	    throw exception_range("crypto password and its confirmation do not match");
	ret.set_crypto_pass(libdar::secu_string(crypto_pass1.get_value().c_str(), crypto_pass1.get_value().size()));
	ret.set_crypto_size(webdar_tools_convert_to_int(crypto_size.get_value()));
    }

    if(has_aux.get_value_as_bool())
    {
	aux.reset(new (nothrow) libdar::archive(webui->get_user_interaction(),
						libdar::path(auxiliary.get_archive_path()),
						auxiliary.get_archive_basename(),
						EXTENSION,
						auxiliary.get_read_options(webui)));
	if(!aux)
	    throw exception_memory();
	else
	    ret.set_auxiliary_ref(aux);
    }

    return ret;
}


string html_options_merge::inherited_get_body_part(const chemin & path,
						   const request & req)
{
    return get_body_part_from_all_children(path, req);
}


void html_options_merge::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}
