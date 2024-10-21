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
#include "webdar_tools.hpp"
#include "webdar_css_style.hpp"
#include "tokens.hpp"
#include "html_form_mask_expression.hpp"
#include "webdar_tools.hpp"

    //
#include "html_options_create.hpp"

using namespace std;

const string html_options_create::entrepot_changed = "html_options_create_entrep_changed";

html_options_create::html_options_create():
    form_archtype("Update"),
    form_archgen("Update"),
    form_reading("Update"),
    form_delta_sig("Update"),
    form_shown("Update"),
    form_perimeter("Update"),
    filename_mask("Filename expression"),
    path_mask(false),
    ea_mask("Extended Attribute expression"),
    form_same_fs("Update"),
    form_compr("Update"),
    form_slicing("Update"),
    form_crypto("Update"),
    archtype_fs(""),
    fixed_date("Only record files changed since:"),
    wrap_ref(""),
    reference("Archive of reference"),
    delta_fs(""),
    delta_sig("Compute binary delta signature", html_form_input::check, "", 1),
    delta_sig_min_size("Avoid calculating delta signature for file smaller than", html_form_input::number, "0", 30),
    delta_mask("Filename expression"),
    archgen_fs(""),
    allow_over("Allow slice overwriting", html_form_input::check, "", 1),
    warn_over("Warn before overwriting", html_form_input::check, "", 1),
    pause("Pause at each N slices (zero = no pause)", html_form_input::number, "", 3),
    slice_permission("Slice permission (octal)", html_form_input::text, "", 10),
    slice_user_ownership("Slice user ownership", html_form_input::text, "", 10),
    slice_group_ownership("slice group ownership", html_form_input::text, "", 10),
    retry_on_change_times("Max retries saving files that changed", html_form_input::number, "", 3),
    retry_on_change_overhead("Max wasted bytes retrying saving changed files", html_form_input::number, "", 10),
    sequential_marks("Add sequential marks", html_form_input::check, "", 1),
    user_comment("User comment in slice header", html_form_input::text, "", 40),
    slice_min_digits("Minimum digits in slice filenames", html_form_input::number, "", 3),
    sparse_file_min_size("Minimum size of holes to lookup in sparse files", html_form_input::number, "", 10),
    hash_algo("Hashing algorithm"),
    execute("Command to execute after each slice", html_form_input::text, "", 30),
    hourshift("Hour shift", html_form_input::number, "", 5),
    shown_fs(""),
    info_details("Detailed informations", html_form_input::check, "1", 1),
    display_treated("Display treated files", html_form_input::check, "1", 1),
    display_treated_only_dir("Display only treated directories", html_form_input::check, "", 1),
    display_dir_summary("Display summary at the end of each directory", html_form_input::check, "", 1),
    empty("Dry run execution", html_form_input::check, "", 1),
    display_skipped("Display skipped files", html_form_input::check, "1", 1),
    security_check("Security warning", html_form_input::check, "", 1),
    dont_ignore_unknown_inode_type("Do not ignore unknown inode type instead of warning", html_form_input::check, "1", 1),
    perimeter_fs(""),
    empty_dir("Store ignored directories as empty directories", html_form_input::check, "1", 1),
    cache_directory_tagging("Ignore directories that use a cache directory tag", html_form_input::check, "1", 1),
    nodump("Avoid saving files having the 'Nodump' flag set", html_form_input::check, "1", 1),
    exclu_by_ea_fs("Avoid saving files having a given Extended Attribute"),
    exclude_by_ea("enable", html_form_input::check, "1", 1),
    default_ea("Use the default excluding Extended Attribute", html_form_input::check, "1", 1),
    exclude_by_ea_name("Extended Attribute name", html_form_input::text, "", 30),
    fs_alter_atime("What to alter if furtive read mode is not used"),
    furtive_read_mode("Furtive read mode (if available)", html_form_input::check, "", 1),
    zeroing_neg_date("Automatically zeroing negative dates while reading", html_form_input::check, "", 1),
    fs_mod_data_detect("How file change is detected"),
    same_fs_fs("Select the filesystems based on their mount point"),
    compr_fs(""),
    compression("Compression algorithm"),
    compression_level("Compression level", html_form_input::number, "", 3),
    min_compr_size("Minimum file sized compressed", html_form_input::number, "", 30),
    compression_block("Block compression for parallel compression (zero to zero to disable)", html_form_input::number, "0", 30),
    never_resave_uncompressed("Never resave uncompressed if compressed file took more place than uncompressed", html_form_input::check, "", 1),
    compr_threads("Number of threads for compression", html_form_input::number, "2", 5),
    compr_mask("Filename expression"),
    slicing_fs(""),
    slicing("Sliced archive", html_form_input::check, "", 1),
    slice_size("Slice size", html_form_input::number, "", 6),
    different_first_slice("Specific size for first slice", html_form_input::check, "", 1),
    first_slice_size("Slice size", html_form_input::number, "", 6),
    crypto_fs(""),
    crypto_type("type of cryptography"),
    crypto_algo("Cipher used"),
    crypto_pass1("Pass phrase", html_form_input::password, "", 30),
    crypto_pass2("Confirm pass phrase", html_form_input::password, "", 30),
    crypto_size("Cipher Block size", html_form_input::number, "", 30),
    crypto_threads("Number of threads for ciphering", html_form_input::number, "2", 5),
    crypto_fs_kdf_hash("Key Derivation Function"),
    iteration_count("Iteration count", html_form_input::number, "1", 30)
{
    libdar::archive_options_create defaults;

	// set field parameters

    archtype.add_choice("full", "Full backup");
    archtype.add_choice("diff", "Differential/Incremental backup");
    archtype.add_choice("diffdelta", "Differential/Incremental backup with binary delta (*)");
    archtype.add_choice("snap", "Snapshot backup");
    archtype.add_choice("date", "Date based differential backup");
    binary_delta_note.add_text(0, "(*) delta signatures need to be present in the backup of reference");
    alter_atime.add_choice("atime", "Data last access time (atime)");
    alter_atime.add_choice("ctime", "Inode last change time (ctime)");
    mod_data_detect.add_choice("any_inode_change", "Any inode change (behavior before rel. 2.6.0)");
    mod_data_detect.add_choice("mtime_size", "only mtime and file size change (default)");
    mod_data_detect.set_selected("mtime_size");
    compression_level.set_range(1, 9);
    pause.set_min_only(0);
    hourshift.set_min_only(0);
    compr_threads.set_min_only(1);
    crypto_threads.set_min_only(1);
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
    slice_size.set_min_only(1);
    slice_size.set_value_as_int(1);
    first_slice_size.set_no_CR();
    first_slice_size.set_min_only(1);
    first_slice_size.set_value_as_int(1);
    min_compr_size.set_no_CR();
    compression_block.set_no_CR();
    retry_on_change_overhead.set_no_CR();
    sparse_file_min_size.set_no_CR();
    delta_sig_min_size.set_no_CR();
    crypto_type.add_choice("sym", "Symmetric encryption");
    crypto_type.add_choice("asym", "Asymmetric encryption");
    crypto_pass1.set_value("");
    crypto_pass2.set_value("");
    crypto_size.set_value(webdar_tools_convert_to_string(defaults.get_crypto_size()));
    crypto_kdf_hash.add_choice("md5","md5");
    crypto_kdf_hash.add_choice("sha1","sha1");
    crypto_kdf_hash.add_choice("sha512","sha512");
    crypto_kdf_hash.add_choice("whirlpool","whirlpool");
    crypto_kdf_hash.add_choice("argon2","argon2");
    crypto_kdf_hash.set_selected("argon2");


	// set default values
    allow_over.set_value_as_bool(defaults.get_allow_over());
    warn_over.set_value_as_bool(defaults.get_warn_over());
    pause.set_value(libdar::deci(defaults.get_pause()).human());
    compression.set_value(defaults.get_compression());
    compression_level.set_value(webdar_tools_convert_to_string(defaults.get_compression_level()));
    compression_block.set_value(webdar_tools_convert_to_string(defaults.get_compression_block_size()));
    slicing.set_value_as_bool(defaults.get_slice_size() != 0);
    different_first_slice.set_value_as_bool(defaults.get_first_slice_size() != defaults.get_slice_size());
    execute.set_value(defaults.get_execute());
    crypto_algo.set_value(defaults.get_crypto_algo());
    delta_sig_min_size.set_value(webdar_tools_convert_to_string(defaults.get_delta_sig_min_size()));

    libdar::infinint tmpi = defaults.get_iteration_count();
    iteration_count.set_min_only(
	webdar_tools_convert_from_infinint<int>(tmpi,
						string("Value provided to iteration count exceeds the supported libdar integer flavor (infinint)")));
    iteration_count.set_value(libdar::deci(tmpi).human());

    min_compr_size.set_value(libdar::deci(defaults.get_min_compr_size()).human());
    what_to_check.set_value(defaults.get_comparison_fields());
    hourshift.set_value(libdar::deci(defaults.get_hourshift()).human());
    empty.set_value_as_bool(defaults.get_empty());
    if(defaults.get_alter_atime())
	alter_atime.set_selected(0);
    else
	alter_atime.set_selected(1);
    furtive_read_mode.set_value_as_bool(defaults.get_furtive_read_mode());
    zeroing_neg_date.set_value_as_bool(defaults.get_auto_zeroing_neg_dates());
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
    dont_ignore_unknown_inode_type.set_value_as_bool(! defaults.get_ignore_unknown_inode_type());

	// build tree dependancy

    static const char* sect_repo = "repo";
    static const char* sect_type = "type";
    static const char* sect_ref = "archive_ref";
    static const char* sect_delta = "delta_sig";
    static const char* sect_gen = "archive_gen_opt";
    static const char* sect_show = "archive show opt";
    static const char* sect_perimeter = "backup perimeter";
    static const char* sect_mask_file = "backup filename masks";
    static const char* sect_mask_path = "backup pathname masks";
    static const char* sect_mount_points = "mount point filtering";
    static const char* sect_ea_mask = "EA masks";
    static const char* sect_source = "source reading mode";
    static const char* sect_compr = "compression";
    static const char* sect_slice = "slicing";
    static const char* sect_cipher = "ciphering";

    deroule.add_section(sect_repo, "Backup Repository");
    deroule.add_section(sect_type, "Backup Type");
    deroule.add_section(sect_ref, "Backup of Reference");
    deroule.add_section(sect_delta, "Delta signatures");
    deroule.add_section(sect_source, "Source file reading mode");
    deroule.add_section(sect_gen, "Backup General Options");
    deroule.add_section(sect_show, "What to show during the operation");
    deroule.add_section(sect_perimeter, "What to take into consideration for backup");
    deroule.add_section(sect_mask_file, "Filename based filtering");
    deroule.add_section(sect_mask_path, "Filesystem filtering");
    deroule.add_section(sect_mount_points, "Mount point filtering");
    deroule.add_section(sect_ea_mask, "Extended Attributes filtering");
    deroule.add_section(sect_compr, "Compression options");
    deroule.add_section(sect_slice, "Slicing options");
    deroule.add_section(sect_cipher, "Encryption options");

    adopt(&deroule);

	// repo
    deroule.adopt_in_section(sect_repo, &entrep);

	// archive type and associated optional fields
    archtype_fs.adopt(&archtype);
    archtype_fs.adopt(&what_to_check);
    archtype_fs.adopt(&hourshift);
    archtype_fs.adopt(&fixed_date);
    archtype_fs.adopt(&binary_delta_note);
    form_archtype.adopt(&archtype_fs);
    deroule.adopt_in_section(sect_type, &form_archtype);

	// archive ref
    ref_placeholder.add_text(2, "Reference archive is only available for Differential/Incremental archive type");
    wrap_ref.adopt(&reference);
    wrap_ref.adopt(&ref_placeholder);
    deroule.adopt_in_section(sect_ref, &wrap_ref);

	// delta signatures
    delta_fs.adopt(&delta_sig);
    delta_fs.adopt(&delta_sig_min_size);
    delta_fs.adopt(&delta_sig_min_size_unit);
    delta_fs.adopt(&sig_block_size);
    form_delta_sig.adopt(&delta_fs);
    deroule.adopt_in_section(sect_delta, &form_delta_sig);
    deroule.adopt_in_section(sect_delta, &delta_mask);

	// source data
    fs_alter_atime.adopt(&alter_atime);
    form_reading.adopt(&furtive_read_mode);
    form_reading.adopt(&fs_alter_atime);
    form_reading.adopt(&zeroing_neg_date);
    form_reading.adopt(&follow_symlinks);
    fs_mod_data_detect.adopt(&mod_data_detect);
    form_reading.adopt(&fs_mod_data_detect);
    deroule.adopt_in_section(sect_source, &form_reading);


	// archive generation
    archgen_fs.adopt(&allow_over);
    archgen_fs.adopt(&warn_over);
    archgen_fs.adopt(&pause);
    archgen_fs.adopt(&slice_permission);
    archgen_fs.adopt(&slice_user_ownership);
    archgen_fs.adopt(&slice_group_ownership);
    archgen_fs.adopt(&retry_on_change_times);
    archgen_fs.adopt(&retry_on_change_overhead);
    archgen_fs.adopt(&retry_on_change_overhead_unit);
    archgen_fs.adopt(&sequential_marks);
    archgen_fs.adopt(&sparse_file_min_size);
    archgen_fs.adopt(&sparse_file_min_size_unit);
    archgen_fs.adopt(&user_comment);
    archgen_fs.adopt(&slice_min_digits);
    archgen_fs.adopt(&hash_algo);
    archgen_fs.adopt(&execute);
    archgen_fs.adopt(&empty);
    form_archgen.adopt(&archgen_fs);
    deroule.adopt_in_section(sect_gen, &form_archgen);


	// operation visibility
    shown_fs.adopt(&info_details);
    shown_fs.adopt(&display_treated);
    shown_fs.adopt(&display_treated_only_dir);
    shown_fs.adopt(&display_dir_summary);
    shown_fs.adopt(&display_skipped);
    shown_fs.adopt(&security_check);
    shown_fs.adopt(&dont_ignore_unknown_inode_type);
    form_shown.adopt(&shown_fs);
    deroule.adopt_in_section(sect_show, &form_shown);

	// perimeter
    perimeter_fs.adopt(&empty_dir);
    perimeter_fs.adopt(&cache_directory_tagging);
    perimeter_fs.adopt(&nodump);
    form_perimeter.adopt(&perimeter_fs);
    exclu_by_ea_fs.adopt(&exclude_by_ea);
    exclu_by_ea_fs.adopt(&default_ea);
    exclu_by_ea_fs.adopt(&exclude_by_ea_name);
    form_perimeter.adopt(&exclu_by_ea_fs);
    deroule.adopt_in_section(sect_perimeter, &form_perimeter);

	// filename based masks
    deroule.adopt_in_section(sect_mask_file, &filename_mask);

	// path based masks
    deroule.adopt_in_section(sect_mask_path, &path_mask);

	// mount point filtering
    same_fs_fs.adopt(&same_fs);
    form_same_fs.adopt(&same_fs_fs);
    deroule.adopt_in_section(sect_mount_points, &form_same_fs);

	// EA masks
    deroule.adopt_in_section(sect_ea_mask, &ea_mask);

	// compression
    compr_fs.adopt(&compression);
    compr_fs.adopt(&compression_level);
    compr_fs.adopt(&min_compr_size);
    compr_fs.adopt(&min_compr_size_unit);
    compr_fs.adopt(&compression_block);
    compr_fs.adopt(&compr_block_unit);
    compr_fs.adopt(&never_resave_uncompressed);
    compr_fs.adopt(&compr_threads);
    form_compr.adopt(&compr_fs);
    deroule.adopt_in_section(sect_compr, &form_compr);
    deroule.adopt_in_section(sect_compr,&compr_mask);


	// slicing
    slicing_fs.adopt(&slicing);
    slicing_fs.adopt(&slice_size);
    slicing_fs.adopt(&slice_size_unit);
    slicing_fs.adopt(&different_first_slice);
    slicing_fs.adopt(&first_slice_size);
    slicing_fs.adopt(&first_slice_size_unit);
    form_slicing.adopt(&slicing_fs);
    deroule.adopt_in_section(sect_slice, &form_slicing);

	// ciphering
    crypto_fs.adopt(&crypto_algo);
    crypto_fs.adopt(&crypto_type);
    crypto_fs.adopt(&crypto_pass1);
    crypto_fs.adopt(&crypto_pass2);
    crypto_fs.adopt(&gnupg);
    crypto_fs.adopt(&crypto_size);
    crypto_fs.adopt(&crypto_threads);
    form_crypto.adopt(&crypto_fs);
    crypto_fs_kdf_hash.adopt(&crypto_kdf_hash);
    crypto_fs_kdf_hash.adopt(&iteration_count);
    form_crypto.adopt(&crypto_fs_kdf_hash);
    deroule.adopt_in_section(sect_cipher, &form_crypto);

	// events and visibility
    register_name(entrepot_changed);

    archtype.record_actor_on_event(this, html_form_radio::changed);
    delta_sig.record_actor_on_event(this, html_form_input::changed);
    display_treated.record_actor_on_event(this, html_form_input::changed);
    exclude_by_ea.record_actor_on_event(this, html_form_input::changed);
    default_ea.record_actor_on_event(this, html_form_input::changed);
    compression.record_actor_on_event(this, html_compression::changed);
    slicing.record_actor_on_event(this, html_form_input::changed);
    different_first_slice.record_actor_on_event(this, html_form_input::changed);
    crypto_algo.record_actor_on_event(this, html_crypto_algo::changed);
    entrep.record_actor_on_event(this, html_entrepot::changed);
    crypto_type.record_actor_on_event(this, html_form_select::changed);
    crypto_kdf_hash.set_change_event_name(kdf_algo_changed);
    crypto_kdf_hash.record_actor_on_event(this, kdf_algo_changed);

    on_event(html_form_radio::changed); // used to initialize the html components visibility

	// css
    webdar_css_style::grey_button(deroule, true);
    display_treated_only_dir.add_css_class(css_indent);
}

libdar::archive_options_create html_options_create::get_options(shared_ptr<html_web_user_interaction> & webui) const
{
    libdar::archive_options_create ret;
    shared_ptr<libdar::archive> ref_arch; // used locally to pass the archive of reference we may build for diff/incr backup
    libdar::infinint compr_bs = libdar::deci(compression_block.get_value()).computer() * compr_block_unit.get_value();
    libdar::U_I val = 0;

    switch(archtype.get_selected_num())
    {
    case 0: // full backup
	break;
    case 2: // diff/incremental backup with binary delta
	ret.set_delta_diff(true);
	    /* no break ! */
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
    case 3: // snapshot
	ret.set_snapshot(true);
	break;
    case 4: // fixed date
	ret.set_fixed_date(fixed_date.get_value());
	break;
    default:
	throw WEBDAR_BUG;
    }

    ret.set_hourshift(libdar::deci(hourshift.get_value()).computer());
    ret.set_allow_over(allow_over.get_value_as_bool());
    ret.set_warn_over(warn_over.get_value_as_bool());
    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_display_treated(display_treated.get_value_as_bool(), display_treated_only_dir.get_value_as_bool());
    ret.set_display_finished(display_dir_summary.get_value_as_bool());
    ret.set_pause(libdar::deci(pause.get_value()).computer());
    ret.set_empty_dir(empty_dir.get_value_as_bool());
    ret.set_compression(compression.get_value());
    ret.set_compression_level(webdar_tools_convert_to_int(compression_level.get_value()));
    ret.set_min_compr_size(libdar::deci(min_compr_size.get_value()).computer() * min_compr_size_unit.get_value());
    ret.set_never_resave_uncompressed(never_resave_uncompressed.get_value_as_bool());
    ret.set_multi_threaded_compress(webdar_tools_convert_to_int(compr_threads.get_value()));
    ret.set_multi_threaded_crypto(webdar_tools_convert_to_int(crypto_threads.get_value()));

    val = webdar_tools_convert_from_infinint<libdar::U_I>(compr_bs,
							  string("compression block size is too large for the underlying operating system, please reduce"));

    if(val < tokens_min_compr_bs && val != 0)
	throw exception_range(libdar::tools_printf("compression block size is too small, select either zero to disable compression per block or a block size greater or equal to %d", tokens_min_compr_bs));
    ret.set_compression_block_size(val);

    if(compression.get_value() != libdar::compression::none)
    {
	unique_ptr<libdar::mask> libcompmask = compr_mask.get_mask();
	if(!libcompmask)
	    throw WEBDAR_BUG;
	ret.set_compr_mask(*libcompmask);
    }

    if(slicing.get_value_as_bool())
    {
	libdar::infinint s_size = libdar::deci(slice_size.get_value()).computer() * slice_size_unit.get_value();
	libdar::infinint f_s_size = 0;

	if(different_first_slice.get_value_as_bool())
	    f_s_size = libdar::deci(first_slice_size.get_value()).computer() * first_slice_size_unit.get_value();

	ret.set_slicing(s_size, f_s_size);
    }

    ret.set_ea_mask(*(ea_mask.get_mask()));

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
    ret.set_nodump(nodump.get_value_as_bool());
    if(exclude_by_ea.get_value_as_bool())
	ret.set_exclude_by_ea(exclude_by_ea_name.get_value());
    ret.set_what_to_check(what_to_check.get_value());
    ret.set_empty(empty.get_value_as_bool());
    ret.set_alter_atime(alter_atime.get_selected_id() == "atime");
    ret.set_furtive_read_mode(furtive_read_mode.get_value_as_bool());
    ret.set_auto_zeroing_neg_dates(zeroing_neg_date.get_value_as_bool());
    if(!follow_symlinks.empty())
	ret.set_ignored_as_symlink(follow_symlinks.get_symlink_list());
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
    ret.set_ignore_unknown_inode_type(! dont_ignore_unknown_inode_type.get_value_as_bool());
    ret.set_execute(execute.get_value());
    ret.set_entrepot(entrep.get_entrepot(webui));
    ret.set_selection(*(filename_mask.get_mask()));
    ret.set_subtree(*(path_mask.get_mask()));

    vector<string> mp = same_fs.get_included_fs_path();
    for(vector<string>::iterator it = mp.begin(); it != mp.end(); ++it)
	ret.set_same_fs_include(*it);

    mp = same_fs.get_excluded_fs_path();
    for(vector<string>::iterator it = mp.begin(); it != mp.end(); ++it)
	ret.set_same_fs_exclude(*it);

    if(delta_sig.get_value_as_bool())
    {
	unique_ptr<libdar::mask> dmask = delta_mask.get_mask();
	libdar::infinint min_sz = libdar::deci(delta_sig_min_size.get_value()).computer() * delta_sig_min_size_unit.get_value();

	if(dmask)
	{
	    ret.set_delta_signature(true);
	    ret.set_delta_mask(*dmask);
	}
	else
	    throw WEBDAR_BUG;

	ret.set_sig_block_len(sig_block_size.get_value());
	ret.set_delta_sig_min_size(min_sz);
    }

    if(mod_data_detect.get_selected_id() == "any_inode_change")
	ret.set_modified_data_detection(libdar::modified_data_detection::any_inode_change);
    else if(mod_data_detect.get_selected_id() == "mtime_size")
	ret.set_modified_data_detection(libdar::modified_data_detection::mtime_size);
    else
	throw WEBDAR_BUG;

    return ret;
}


string html_options_create::inherited_get_body_part(const chemin & path,
						    const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_options_create::on_event(const string & event_name)
{
    if(event_name == html_form_radio::changed
       || event_name == html_compression::changed
       || event_name == html_form_input::changed
       || event_name == html_crypto_algo::changed
       || event_name == html_form_select::changed)
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
	case 2: // diff+delta
	    reference.set_visible(true);
	    ref_placeholder.set_visible(false);
	    hourshift.set_visible(true);
	    fixed_date.set_visible(false);
	    what_to_check.set_visible(true);
	    break;
	case 3: // snapshot
	    reference.set_visible(false);
	    ref_placeholder.set_visible(true);
	    hourshift.set_visible(false);
	    fixed_date.set_visible(false);
	    what_to_check.set_visible(false);
	    break;
	case 4: // date
	    reference.set_visible(false);
	    ref_placeholder.set_visible(true);
	    hourshift.set_visible(true);
	    fixed_date.set_visible(true);
	    what_to_check.set_visible(false);
	    break;
	default:
	    throw WEBDAR_BUG;
	}

	delta_mask.set_visible(delta_sig.get_value_as_bool());
	delta_sig_min_size.set_visible(delta_sig.get_value_as_bool());
	delta_sig_min_size_unit.set_visible(delta_sig.get_value_as_bool());
	sig_block_size.set_visible(delta_sig.get_value_as_bool());
	display_treated_only_dir.set_visible(display_treated.get_value_as_bool());

	if(exclude_by_ea.get_value_as_bool())
	{
	    default_ea.set_visible(true);
	    exclude_by_ea_name.set_visible(true);
	    if(default_ea.get_value_as_bool())
	    {
		exclude_by_ea_name.set_value(libdar::archive_options_create::default_excluding_ea);
		exclude_by_ea_name.set_enabled(false);
	    }
	    else
		exclude_by_ea_name.set_enabled(true);
	}
	else
	{
	    default_ea.set_visible(false);
	    exclude_by_ea_name.set_visible(false);
	}

	switch(compression.get_value())
	{
	case libdar::compression::none:
	    compression_level.set_visible(false);
	    compression.set_no_CR(false);
	    min_compr_size.set_visible(false);
	    min_compr_size_unit.set_visible(false);
	    compression_block.set_visible(false);
	    compr_block_unit.set_visible(false);
	    never_resave_uncompressed.set_visible(false);
	    compr_threads.set_visible(false);
	    compr_mask.set_visible(false);
	    break;
	case libdar::compression::lzo1x_1_15:
	case libdar::compression::lzo1x_1:
	    compression.set_no_CR(false);
	    compression_level.set_visible(false);
	    min_compr_size.set_visible(true);
	    min_compr_size_unit.set_visible(true);
	    compression_block.set_visible(true);
	    compr_block_unit.set_visible(true);
	    never_resave_uncompressed.set_visible(true);
	    compr_threads.set_visible(true);
	    compr_mask.set_visible(true);
	    break;
	case libdar::compression::gzip:
	case libdar::compression::bzip2:
	case libdar::compression::lzo:
	case libdar::compression::xz:
	case libdar::compression::zstd:
	case libdar::compression::lz4:
	    compression.set_no_CR(true);
	    compression_level.set_visible(true);
	    min_compr_size.set_visible(true);
	    min_compr_size_unit.set_visible(true);
	    compression_block.set_visible(true);
	    compr_block_unit.set_visible(true);
	    never_resave_uncompressed.set_visible(true);
	    compr_threads.set_visible(true);
	    compr_mask.set_visible(true);
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
	    crypto_threads.set_visible(false);
	    gnupg.set_visible(false);
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
    {
	act(entrepot_changed);
    }
    else if(event_name == kdf_algo_changed)
    {
	libdar::infinint cur_it = libdar::deci(iteration_count.get_value()).computer();

	if(crypto_kdf_hash.get_selected_id() != "argon2")
	{
	    if(cur_it < libdar::default_iteration_count)
		iteration_count.set_value(libdar::deci(libdar::default_iteration_count).human());
	    iteration_count.set_min_only(
		webdar_tools_convert_from_infinint<int>(
		    libdar::default_iteration_count,
		    string("too large integer default value provided by libdar")));
	}
	else
	{
	    if(cur_it < libdar::default_iteration_count_argon2)
		iteration_count.set_value(libdar::deci(libdar::default_iteration_count_argon2).human());
	    iteration_count.set_min_only(
		webdar_tools_convert_from_infinint<int>(
		    libdar::default_iteration_count_argon2,
		    string("too large integer default value provided by libdar")));

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

void html_options_create::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);

    css tmp;
    tmp.css_margin_left("4em");

    if(! csslib->class_exists(css_indent))
	csslib->add(css_indent, tmp);
}
