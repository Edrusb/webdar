/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2023 Denis Corbin
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
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "webdar_tools.hpp"
#include "webdar_css_style.hpp"
#include "tokens.hpp"

    //
#include "html_options_create.hpp"

using namespace std;

const string html_options_create::entrepot_changed = "html_options_create_entrep_changed";

html_options_create::html_options_create():
    form_archtype("Update"),
    form_archgen("Update"),
    form_shown("Update"),
    form_perimeter("Update"),
    form_reading("Update"),
    form_compr("Update"),
    form_slicing("Update"),
    form_crypto("Update"),
    wrap_ref("Archive of reference related parameters"),
    reference("Archive of reference"),
    allow_over("Allow slice overwriting", html_form_input::check, "", 1),
    warn_over("Warn before overwriting", html_form_input::check, "", 1),
    info_details("Detailed informations", html_form_input::check, "", 1),
    pause("Pause at each N slices (zero = no pause)", html_form_input::number, "", 3),
    empty_dir("Store ignored directories as empty directories", html_form_input::check, "", 1),
    compression("Compression algorithm"),
    compression_level("Compression level", html_form_input::range, "", 3),
    slicing("Sliced archive", html_form_input::check, "", 1),
    slice_size("Slice size", html_form_input::number, "", 6),
    different_first_slice("Specific size for first slice", html_form_input::check, "", 1),
    first_slice_size("Slice size", html_form_input::number, "", 6),
    execute("Command to execute after each slice", html_form_input::text, "", 30),
    crypto_algo("Cipher used"),
    crypto_pass1("Pass phrase", html_form_input::password, "", 30),
    crypto_pass2("Confirm pass phrase", html_form_input::password, "", 30),
    crypto_size("Cipher Block size", html_form_input::number, "", 30),
    min_compr_size("Minimum file sized compressed", html_form_input::number, "", 30),
    nodump("Avoid saving files having the 'Nodump' flag set", html_form_input::check, "", 1),
    hourshift("Hour shift", html_form_input::number, "", 5),
    empty("Dry run execution", html_form_input::check, "", 1),
    fs_alter_atime("What to alter if furtive read mode is not used"),
    furtive_read_mode("Furtive read mode (if available)", html_form_input::check, "", 1),
    same_fs("Only consider files located on the same filesystem as the rooted directory", html_form_input::check, "", 1),
    cache_directory_tagging("Ignore directories that use a cache directory tag", html_form_input::check, "", 1),
    display_skipped("Display skipped files", html_form_input::check, "", 1),
    fixed_date("Only record files changed since:"),
    slice_permission("Slice permission (octal)", html_form_input::text, "", 10),
    slice_user_ownership("Slice user ownership", html_form_input::text, "", 10),
    slice_group_ownership("slice group ownership", html_form_input::text, "", 10),
    retry_on_change_times("Max retries saving files that changed", html_form_input::number, "", 3),
    retry_on_change_overhead("Max wasted bytes retrying saving changed files", html_form_input::number, "", 10),
    sequential_marks("Add sequential marks", html_form_input::check, "", 1),
    sparse_file_min_size("Minimum size of holes to lookup in sparse files", html_form_input::number, "", 10),
    security_check("Security warning", html_form_input::check, "", 1),
    user_comment("User comment in slice header", html_form_input::text, "", 40),
    hash_algo("Hashing algorithm"),
    slice_min_digits("Minimum digits in slice filenames", html_form_input::number, "", 3),
    ignore_unknown_inode_type("Ignore unknown inode type instead of warning", html_form_input::check, "", 1)
{
    libdar::archive_options_create defaults;

	// set field parameters

    archtype.add_choice("full", "Full backup");
    archtype.add_choice("diff", "Differential/Incremental backup");
    archtype.add_choice("snap", "Snapshot backup");
    archtype.add_choice("date", "Date based differential backup");
    alter_atime.add_choice("atime", "Data last access time (atime)");
    alter_atime.add_choice("ctime", "Inode last change time (ctime)");
    compression_level.set_range(1, 9);
    if(defaults.get_reference() != nullptr)
	throw WEBDAR_BUG; // not expected default value!!!
    else
	if(defaults.get_snapshot())
	    archtype.set_selected(2);
	else
	    if(defaults.get_fixed_date() != 0)
		archtype.set_selected(3);
	    else
		archtype.set_selected(0);

    compression.set_no_CR();
    slice_size.set_no_CR();
    first_slice_size.set_no_CR();
    min_compr_size.set_no_CR();
    retry_on_change_overhead.set_no_CR();
    sparse_file_min_size.set_no_CR();

	// set default values
    allow_over.set_value_as_bool(defaults.get_allow_over());
    warn_over.set_value_as_bool(defaults.get_warn_over());
    info_details.set_value_as_bool(defaults.get_info_details());
    pause.set_value(libdar::deci(defaults.get_pause()).human());
    empty_dir.set_value_as_bool(defaults.get_empty_dir());
    compression.set_value(defaults.get_compression());
    compression_level.set_value(webdar_tools_convert_to_string(defaults.get_compression_level()));
    slicing.set_value_as_bool(defaults.get_slice_size() != 0);
    slice_size.set_value(libdar::deci(defaults.get_slice_size()).human());
    different_first_slice.set_value_as_bool(defaults.get_first_slice_size() != defaults.get_slice_size());
    first_slice_size.set_value(libdar::deci(defaults.get_first_slice_size()).human());
    execute.set_value(defaults.get_execute());
    crypto_algo.set_value(defaults.get_crypto_algo());
    crypto_pass1.set_value("");
    crypto_pass2.set_value("");
    crypto_size.set_value(webdar_tools_convert_to_string(defaults.get_crypto_size()));
    min_compr_size.set_value(libdar::deci(defaults.get_min_compr_size()).human());
    nodump.set_value_as_bool(defaults.get_nodump());
    what_to_check.set_value(defaults.get_comparison_fields());
    hourshift.set_value(libdar::deci(defaults.get_hourshift()).human());
    empty.set_value_as_bool(defaults.get_empty());
    if(defaults.get_alter_atime())
	alter_atime.set_selected(0);
    else
	alter_atime.set_selected(1);
    furtive_read_mode.set_value_as_bool(defaults.get_furtive_read_mode());
    same_fs.set_value_as_bool(defaults.get_same_fs());
    cache_directory_tagging.set_value_as_bool(defaults.get_cache_directory_tagging());
    display_skipped.set_value_as_bool(defaults.get_display_skipped());
    fixed_date.set_value(defaults.get_fixed_date());
    slice_permission.set_value(defaults.get_slice_permission());
    slice_user_ownership.set_value(defaults.get_slice_user_ownership());
    slice_group_ownership.set_value(defaults.get_slice_group_ownership());
    retry_on_change_times.set_value(libdar::deci(defaults.get_repeat_count()).human());
    retry_on_change_overhead.set_value(libdar::deci(defaults.get_repeat_byte()).human());
    sequential_marks.set_value_as_bool(defaults.get_sequential_marks());
    sparse_file_min_size.set_value(libdar::deci(defaults.get_sparse_file_min_size()).human());
    security_check.set_value_as_bool(defaults.get_security_check());
    user_comment.set_value(defaults.get_user_comment());
    hash_algo.set_value(defaults.get_hash_algo());
    slice_min_digits.set_value(libdar::deci(defaults.get_slice_min_digits()).human());
    ignore_unknown_inode_type.set_value_as_bool(defaults.get_ignore_unknown_inode_type());

	// build tree dependancy

    static const char* sect_repo = "repo";
    static const char* sect_type = "type";
    static const char* sect_ref = "archive_ref";
    static const char* sect_gen = "archive_gen_opt";
    static const char* sect_show = "archive show opt";
    static const char* sect_perimeter = "backup perimeter";
    static const char* sect_source = "source reading mode";
    static const char* sect_compr = "compression";
    static const char* sect_slice = "slicing";
    static const char* sect_cipher = "ciphering";

    deroule.add_section(sect_repo, "Archive repository");
    deroule.add_section(sect_type, "Archive type");
    deroule.add_section(sect_ref, "Archive of Reference");
    deroule.add_section(sect_gen, "Archive generation options");
    deroule.add_section(sect_show, "What to show during the operation");
    deroule.add_section(sect_perimeter, "What to take into consideration for backup");
    deroule.add_section(sect_source, "Source file reading mode");
    deroule.add_section(sect_compr, "Compression options");
    deroule.add_section(sect_slice, "Slicing options");
    deroule.add_section(sect_cipher, "Encryption options");
    adopt(&deroule);

	// repo
    deroule.adopt_in_section(sect_repo, &entrep);

	// archive type and associated optional fields
    form_archtype.adopt(&archtype);
    form_archtype.adopt(&what_to_check);
    form_archtype.adopt(&hourshift);
    form_archtype.adopt(&fixed_date);
    deroule.adopt_in_section(sect_type, &form_archtype);

	// archive ref
    ref_placeholder.add_text(2, "Reference archive is only available for Differential/Incremental archive type");
    wrap_ref.adopt(&reference);
    wrap_ref.adopt(&ref_placeholder);
    deroule.adopt_in_section(sect_ref, &wrap_ref);

	// archive generation
    form_archgen.adopt(&allow_over);
    form_archgen.adopt(&warn_over);
    form_archgen.adopt(&pause);
    form_archgen.adopt(&slice_permission);
    form_archgen.adopt(&slice_user_ownership);
    form_archgen.adopt(&slice_group_ownership);
    form_archgen.adopt(&retry_on_change_times);
    form_archgen.adopt(&retry_on_change_overhead);
    form_archgen.adopt(&retry_on_change_overhead_unit);
    form_archgen.adopt(&sequential_marks);
    form_archgen.adopt(&sparse_file_min_size);
    form_archgen.adopt(&sparse_file_min_size_unit);
    form_archgen.adopt(&user_comment);
    form_archgen.adopt(&slice_min_digits);
    form_archgen.adopt(&hash_algo);
    form_archgen.adopt(&execute);
    form_archgen.adopt(&empty);
    deroule.adopt_in_section(sect_gen, &form_archgen);


	// operation visibility
    form_shown.adopt(&info_details);
    form_shown.adopt(&display_skipped);
    form_shown.adopt(&security_check);
    form_shown.adopt(&ignore_unknown_inode_type);
    deroule.adopt_in_section(sect_show, &form_shown);

	// perimeter
    form_perimeter.adopt(&empty_dir);
    form_perimeter.adopt(&nodump);
    form_perimeter.adopt(&same_fs);
    form_perimeter.adopt(&cache_directory_tagging);
    deroule.adopt_in_section(sect_perimeter, &form_perimeter);

	// source data
    fs_alter_atime.adopt(&alter_atime);
    form_reading.adopt(&furtive_read_mode);
    form_reading.adopt(&fs_alter_atime);
    deroule.adopt_in_section(sect_source, &form_reading);

	// compression
    form_compr.adopt(&compression);
    form_compr.adopt(&compression_level);
    form_compr.adopt(&min_compr_size);
    form_compr.adopt(&min_compr_size_unit);
    deroule.adopt_in_section(sect_compr, &form_compr);

	// slicing
    form_slicing.adopt(&slicing);
    form_slicing.adopt(&slice_size);
    form_slicing.adopt(&slice_size_unit);
    form_slicing.adopt(&different_first_slice);
    form_slicing.adopt(&first_slice_size);
    form_slicing.adopt(&first_slice_size_unit);
    deroule.adopt_in_section(sect_slice, &form_slicing);

	// ciphering
    form_crypto.adopt(&crypto_algo);
    form_crypto.adopt(&crypto_pass1);
    form_crypto.adopt(&crypto_pass2);
    form_crypto.adopt(&crypto_size);
    deroule.adopt_in_section(sect_cipher, &form_crypto);

	// events and visibility
    register_name(entrepot_changed);

    archtype.record_actor_on_event(this, html_form_radio::changed);
    compression.record_actor_on_event(this, html_compression::changed);
    slicing.record_actor_on_event(this, html_form_input::changed);
    different_first_slice.record_actor_on_event(this, html_form_input::changed);
    crypto_algo.record_actor_on_event(this, html_crypto_algo::changed);
    entrep.record_actor_on_event(this, html_entrepot::changed);

    on_event(html_form_radio::changed); // used to initialize the html components visibility

	// css
    webdar_css_style::grey_button(deroule, true);
}

libdar::archive_options_create html_options_create::get_options(std::shared_ptr<html_web_user_interaction> & webui) const
{
    libdar::archive_options_create ret;
    std::shared_ptr<libdar::archive> ref_arch; // used locally to pass the archive of reference we may build for diff/incr backup

    switch(archtype.get_selected_num())
    {
    case 0: // full backup
	break;
    case 1: // diff/incremental backup
	ref_arch.reset(new (nothrow) libdar::archive(webui->get_user_interaction(),
						     libdar::path(reference.get_archive_path()),
						     reference.get_archive_basename(),
						     EXTENSION,
						     reference.get_read_options(webui)));
	if(!ref_arch)
	    throw exception_memory();
	else
	    ret.set_reference(ref_arch);
	break;
    case 2: // snapshot
	ret.set_snapshot(true);
	break;
    case 3: // fixed date
	ret.set_fixed_date(fixed_date.get_value());
	break;
    default:
	throw WEBDAR_BUG;
    }

    ret.set_hourshift(libdar::deci(hourshift.get_value()).computer());
    ret.set_allow_over(allow_over.get_value_as_bool());
    ret.set_warn_over(warn_over.get_value_as_bool());
    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_pause(libdar::deci(pause.get_value()).computer());
    ret.set_empty_dir(empty_dir.get_value_as_bool());
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
    ret.set_nodump(nodump.get_value_as_bool());
    ret.set_what_to_check(what_to_check.get_value());
    ret.set_empty(empty.get_value_as_bool());
    ret.set_alter_atime(alter_atime.get_selected_id() == "atime");
    ret.set_furtive_read_mode(furtive_read_mode.get_value_as_bool());
    ret.set_same_fs(same_fs.get_value_as_bool());
    ret.set_cache_directory_tagging(cache_directory_tagging.get_value_as_bool());
    ret.set_display_skipped(display_skipped.get_value_as_bool());
    ret.set_slice_permission(slice_permission.get_value());
    ret.set_slice_user_ownership(slice_user_ownership.get_value());
    ret.set_slice_group_ownership(slice_group_ownership.get_value());
    ret.set_retry_on_change(libdar::deci(retry_on_change_times.get_value()).computer(),
			    libdar::deci(retry_on_change_overhead.get_value()).computer() * retry_on_change_overhead_unit.get_value());
    ret.set_sequential_marks(sequential_marks.get_value_as_bool());
    ret.set_sparse_file_min_size(libdar::deci(sparse_file_min_size.get_value()).computer() * sparse_file_min_size_unit.get_value());
    ret.set_security_check(security_check.get_value_as_bool());
    ret.set_user_comment(user_comment.get_value());
    ret.set_hash_algo(hash_algo.get_value());
    ret.set_slice_min_digits(libdar::deci(slice_min_digits.get_value()).computer());
    ret.set_ignore_unknown_inode_type(ignore_unknown_inode_type.get_value_as_bool());
    ret.set_execute(execute.get_value());
    ret.set_entrepot(entrep.get_entrepot(webui));

    return ret;
}


string html_options_create::inherited_get_body_part(const chemin & path,
						    const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_options_create::on_event(const std::string & event_name)
{
    if(event_name == html_form_radio::changed
       || event_name == html_compression::changed
       || event_name == html_form_input::changed
       || event_name == html_crypto_algo::changed)
    {
	switch(archtype.get_selected_num())
	{
	case 0: // full
	    reference.set_visible(false);
	    ref_placeholder.set_visible(true);
	    hourshift.set_visible(false);
	    fixed_date.set_visible(false);
	    what_to_check.set_visible(false);
	    break;
	case 1: // diff
	    reference.set_visible(true);
	    ref_placeholder.set_visible(false);
	    hourshift.set_visible(true);
	    fixed_date.set_visible(false);
	    what_to_check.set_visible(true);
	    break;
	case 2: // snapshot
	    reference.set_visible(false);
	    ref_placeholder.set_visible(true);
	    hourshift.set_visible(false);
	    fixed_date.set_visible(false);
	    what_to_check.set_visible(false);
	    break;
	case 3: // date
	    reference.set_visible(false);
	    ref_placeholder.set_visible(true);
	    hourshift.set_visible(true);
	    fixed_date.set_visible(true);
	    what_to_check.set_visible(false);
	    break;
	default:
	    throw WEBDAR_BUG;
	}

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

void html_options_create::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}
