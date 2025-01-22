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
    form_same_fs("Update"),
    archtype_fs(""),
    fixed_date("Only record files changed since:"),
    wrap_ref(""),
    reference("Archive of reference"),
    delta_fs(""),
    delta_sig("Compute binary delta signature", html_form_input::check, "", "1"),
    delta_sig_min_size("Avoid calculating delta signature for file smaller than", 0, "30"),
    archgen_fs(""),
    allow_over("Allow slice overwriting", html_form_input::check, "", "1"),
    warn_over("Warn before overwriting", html_form_input::check, "", "1"),
    pause("Pause at each N slices (zero = no pause)", html_form_input::number, "", "5"),
    retry_on_change_times("Max retries saving files that changed", html_form_input::number, "", "5"),
    retry_on_change_overhead("Max wasted bytes retrying saving changed files", 0, "10"),
    sequential_marks("Add sequential marks", html_form_input::check, "", "1"),
    user_comment("User comment in slice header", html_form_input::text, "", "80%"),
    sparse_file_min_size("Minimum size of holes to lookup in sparse files", 0, "10"),
    hash_algo("Hashing algorithm"),
    execute("Command to execute after each slice", html_form_input::text, "", "80%"),
    hourshift("Ignore difference of exactly +/- N hour(s)", html_form_input::number, "", "5"),
    shown_fs(""),
    info_details("Detailed informations", html_form_input::check, "1", "1"),
    display_treated("Display treated files", html_form_input::check, "1", "1"),
    display_treated_only_dir("Display only treated directories", html_form_input::check, "", "1"),
    display_dir_summary("Display summary at the end of each directory", html_form_input::check, "", "1"),
    empty("Dry run execution", html_form_input::check, "", "1"),
    display_skipped("Display skipped files", html_form_input::check, "1", "1"),
    security_check("Security warning", html_form_input::check, "", "1"),
    dont_ignore_unknown_inode_type("Do not ignore unknown inode type instead of warning", html_form_input::check, "1", "1"),
    perimeter_fs(""),
    empty_dir("Store ignored directories as empty directories", html_form_input::check, "1", "1"),
    cache_directory_tagging("Ignore directories that use a cache directory tag", html_form_input::check, "1", "1"),
    nodump("Avoid saving files having the 'Nodump' flag set", html_form_input::check, "1", "1"),
    exclu_by_ea_fs("Avoid saving files having a given Extended Attribute"),
    exclude_by_ea("enable", html_form_input::check, "1", "1"),
    default_ea("Use the default excluding Extended Attribute", html_form_input::check, "1", "1"),
    exclude_by_ea_name("Extended Attribute name", html_form_input::text, "", "80%"),
    fs_alter_atime("What to alter if furtive read mode is not used"),
    furtive_read_mode("Furtive read mode (if available)", html_form_input::check, "", "1"),
    zeroing_neg_date("Automatically zeroing negative dates while reading", html_form_input::check, "", "1"),
    fs_mod_data_detect("How data (not metadata) changes are detected"),
    same_fs_fs("Select the file systems based on their mount point")
{
    libdar::archive_options_create defaults;

	// set field parameters

    entrep.reset(new (nothrow) html_entrepot());
    if(!entrep)
	throw exception_memory();

    filename_mask.reset(new (nothrow) html_mask_form_filename("file name"));
    if(!filename_mask)
	throw exception_memory();

    ea_mask.reset(new (nothrow) html_mask_form_filename("extended attribute"));
    if(!ea_mask)
	throw exception_memory();

    delta_mask.reset(new (nothrow) html_mask_form_filename("file name"));
    if(!delta_mask)
	throw exception_memory();

    compr_params.reset(new (nothrow) html_compression_params(true, true, false));
    if(!compr_params)
	throw exception_memory();

    compr_mask.reset(new (nothrow) html_mask_form_filename("file name"));
    if(!compr_mask)
	throw exception_memory();

    path_mask.reset(new (nothrow) html_mask_form_path(true));
    if(!path_mask)
	throw exception_memory();


    archtype.add_choice("full", "Full backup");
    archtype.add_choice("diff", "Differential/Incremental backup");
    archtype.add_choice("diffdelta", "Differential/Incremental backup with binary delta (*)");
    archtype.add_choice("snap", "Snapshot backup");
    archtype.add_choice("date", "Date based differential backup");
    binary_delta_note.add_text(0, "(*) delta signatures need to be present in the backup of reference");
    delta_filter_title.add_paragraph();
    delta_filter_title.add_text(3, "Delta signature filename based filtering");
    compr_filter_title.add_paragraph();
    compr_filter_title.add_text(3, "Compression filename based filtering");
    alter_atime.add_choice("atime", "Data last access time (atime)");
    alter_atime.add_choice("ctime", "Inode last change time (ctime)");
    mod_data_detect.add_choice("any_inode_change", "Any inode change (behavior before libdar 2.6.0)");
    mod_data_detect.add_choice("mtime_size", "only mtime and file size change (default)");
    mod_data_detect.set_selected_id("mtime_size");
    pause.set_min_only(0);
    hourshift.set_range(0, 23);
    if(defaults.get_reference() != nullptr)
	throw WEBDAR_BUG; // not expected default value!!!
    else
	if(defaults.get_snapshot())
	    archtype.set_selected_num(2);
	else
	    if(defaults.get_fixed_date() != 0)
		archtype.set_selected_num(3);
	    else
		archtype.set_selected_num(0);
    retry_on_change_times.set_min_only(0);

	// set default values
    allow_over.set_value_as_bool(defaults.get_allow_over());
    warn_over.set_value_as_bool(defaults.get_warn_over());
    pause.set_value(libdar::deci(defaults.get_pause()).human());
    execute.set_value(defaults.get_execute());
    delta_sig_min_size.set_value_as_infinint(defaults.get_delta_sig_min_size());
    what_to_check.set_value(defaults.get_comparison_fields());
    hourshift.set_value(libdar::deci(defaults.get_hourshift()).human());
    empty.set_value_as_bool(defaults.get_empty());
    if(defaults.get_alter_atime())
	alter_atime.set_selected_num(0);
    else
	alter_atime.set_selected_num(1);
    furtive_read_mode.set_value_as_bool(defaults.get_furtive_read_mode());
    zeroing_neg_date.set_value_as_bool(defaults.get_auto_zeroing_neg_dates());
    fixed_date.set_value(defaults.get_fixed_date());
    retry_on_change_times.set_value(libdar::deci(defaults.get_repeat_count()).human());
    retry_on_change_overhead.set_value_as_infinint(libdar::infinint(0));
    sequential_marks.set_value_as_bool(defaults.get_sequential_marks());
    sparse_file_min_size.set_value_as_infinint(defaults.get_sparse_file_min_size());
    security_check.set_value_as_bool(defaults.get_security_check());
    user_comment.set_value(defaults.get_user_comment());
    hash_algo.set_value(defaults.get_hash_algo());
    dont_ignore_unknown_inode_type.set_value_as_bool(! defaults.get_ignore_unknown_inode_type());
    slicing.set_slicing(defaults.get_slice_size(), defaults.get_first_slice_size());
    slicing.set_permission(defaults.get_slice_permission());
    slicing.set_user_ownership(defaults.get_slice_user_ownership());
    slicing.set_group_ownership(defaults.get_slice_group_ownership());
    slicing.set_min_digits(defaults.get_slice_min_digits());
    ciphering.set_crypto_size_range(defaults.get_crypto_size(), libdar::infinint(4294967296)); // max is 2^32

	// build tree dependancy

    static const char* sect_repo = "repo";
    static const char* sect_type = "type";
    static const char* sect_ref = "archive_ref";
    static const char* sect_delta = "delta_sig";
    static const char* sect_gen = "archive_gen_opt";
    static const char* sect_show = "archive show opt";
    static const char* sect_perimeter = "backup perimeter";
    static const char* sect_mask_file = "backup file name masks";
    static const char* sect_mask_path = "backup pathname masks";
    static const char* sect_mount_points = "mount point filtering";
    static const char* sect_ea_mask = "EA masks";
    static const char* sect_fsa_scope = "FSA Scope";
    static const char* sect_source = "source reading mode";
    static const char* sect_compr = "compression";
    static const char* sect_slice = "slicing";
    static const char* sect_cipher = "ciphering";

    deroule.add_section(sect_repo, "Backup Repository");
    deroule.add_section(sect_type, "Backup Type");
    deroule.add_section(sect_ref, "Backup of Reference");
    deroule.add_section(sect_delta, "Delta signatures");
    deroule.add_section(sect_source, "File system reading mode");
    deroule.add_section(sect_gen, "How to process the operation");
    deroule.add_section(sect_show, "What to show during the operation");
    deroule.add_section(sect_perimeter, "What to take into consideration for backup");
    deroule.add_section(sect_mask_file, "File name based filtering");
    deroule.add_section(sect_mask_path, "Path based filtering");
    deroule.add_section(sect_mount_points, "Mount point filtering");
    deroule.add_section(sect_ea_mask, "Extended Attributes filtering");
    deroule.add_section(sect_fsa_scope, "File system Specific Attributes filtering");
    deroule.add_section(sect_compr, "Compression options");
    deroule.add_section(sect_slice, "Slicing options");
    deroule.add_section(sect_cipher, "Encryption options");

    adopt(&deroule);

	// repo
    deroule.adopt_in_section(sect_repo, &guichet_entrep);

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
    delta_fs.adopt(&sig_block_size);
    form_delta_sig.adopt(&delta_fs);
    deroule.adopt_in_section(sect_delta, &form_delta_sig);
    deroule.adopt_in_section(sect_delta, &delta_filter_title);
    deroule.adopt_in_section(sect_delta, &guichet_delta_mask);

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
    archgen_fs.adopt(&retry_on_change_times);
    archgen_fs.adopt(&retry_on_change_overhead);
    archgen_fs.adopt(&sequential_marks);
    archgen_fs.adopt(&sparse_file_min_size);
    archgen_fs.adopt(&user_comment);
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
    deroule.adopt_in_section(sect_mask_file, &guichet_filename_mask);

	// path based masks
    deroule.adopt_in_section(sect_mask_path, &guichet_path_mask);

	// mount point filtering
    same_fs_fs.adopt(&same_fs);
    form_same_fs.adopt(&same_fs_fs);
    deroule.adopt_in_section(sect_mount_points, &form_same_fs);

	// EA masks
    deroule.adopt_in_section(sect_ea_mask, &guichet_ea_mask);

	// FSA scope
    deroule.adopt_in_section(sect_fsa_scope, &fsa_scope);

	// compression
    deroule.adopt_in_section(sect_compr, &guichet_compr_params);
    deroule.adopt_in_section(sect_compr, &compr_filter_title);
    deroule.adopt_in_section(sect_compr,&guichet_compr_mask);


	// slicing

    deroule.adopt_in_section(sect_slice, &slicing);

	// ciphering
    deroule.adopt_in_section(sect_cipher, &ciphering);

	// events and visibility
    register_name(entrepot_changed);

    archtype.record_actor_on_event(this, html_form_radio::changed);
    delta_sig.record_actor_on_event(this, html_form_input::changed);
    display_treated.record_actor_on_event(this, html_form_input::changed);
    exclude_by_ea.record_actor_on_event(this, html_form_input::changed);
    default_ea.record_actor_on_event(this, html_form_input::changed);
    compr_params->record_actor_on_event(this, html_compression_params::changed);
    entrep->record_actor_on_event(this, html_entrepot::changed);

    on_event(html_form_radio::changed); // used to initialize the html components visibility

	// css
    webdar_css_style::grey_button(deroule, true);
    display_treated_only_dir.add_css_class(webdar_css_style::wcs_indent);
}

void html_options_create::set_biblio(const shared_ptr<bibliotheque> & ptr)
{
    guichet_entrep.set_child(ptr,
			     bibliotheque::repo,
			     entrep,
			     false);

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

    delta_mask->set_child(ptr, bibliotheque::filefilter);
    guichet_delta_mask.set_child(ptr,
				 bibliotheque::filefilter,
				 delta_mask,
				 false);
    guichet_compr_params.set_child(ptr,
				   bibliotheque::compress,
				   compr_params,
				   false);
    compr_mask->set_child(ptr, bibliotheque::filefilter);
    guichet_compr_mask.set_child(ptr,
				 bibliotheque::filefilter,
				 compr_mask,
				 false);

    guichet_path_mask.set_child(ptr,
				bibliotheque::pathfilter,
				path_mask,
				false);
    reference.set_biblio(ptr);
}

libdar::archive_options_create html_options_create::get_options(shared_ptr<html_web_user_interaction> & webui) const
{
    libdar::archive_options_create ret;
    shared_ptr<libdar::archive> ref_arch; // used locally to pass the archive of reference we may build for diff/incr backup
    libdar::infinint compr_bs = compr_params->get_compression_block();
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
    ret.set_compression(compr_params->get_compression_algo());
    ret.set_compression_level(compr_params->get_compression_level());
    ret.set_min_compr_size(compr_params->get_min_compression_size());
    ret.set_never_resave_uncompressed(compr_params->get_resave_uncompressed());
    ret.set_multi_threaded_compress(compr_params->get_num_threads());

    val = webdar_tools_convert_from_infinint<libdar::U_I>(compr_bs,
							  string("compression block size is too large for the underlying operating system, please reduce"));

    if(val < tokens_min_compr_bs && val != 0)
	throw exception_range(libdar::tools_printf("compression block size is too small, select either zero to disable compression per block or a block size greater or equal to %d", tokens_min_compr_bs));
    ret.set_compression_block_size(val);

    if(compr_params->get_compression_algo() != libdar::compression::none)
    {
	unique_ptr<libdar::mask> libcompmask = compr_mask->get_mask();
	if(!libcompmask)
	    throw WEBDAR_BUG;
	ret.set_compr_mask(*libcompmask);
    }

    libdar::infinint s_size;
    libdar::infinint f_s_size;
    slicing.get_slicing(s_size, f_s_size);
    ret.set_slicing(s_size, f_s_size);

    ret.set_ea_mask(*(ea_mask->get_mask()));
    ret.set_fsa_scope(fsa_scope.get_scope());

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
    ret.set_slice_permission(slicing.get_permission());
    ret.set_slice_user_ownership(slicing.get_user_ownership());
    ret.set_slice_group_ownership(slicing.get_group_ownership());
    ret.set_retry_on_change(libdar::deci(retry_on_change_times.get_value()).computer(),
			    retry_on_change_overhead.get_value_as_infinint());
    ret.set_sequential_marks(sequential_marks.get_value_as_bool());
    ret.set_sparse_file_min_size(sparse_file_min_size.get_value_as_infinint());
    ret.set_security_check(security_check.get_value_as_bool());
    ret.set_user_comment(user_comment.get_value());
    ret.set_hash_algo(hash_algo.get_value());
    ret.set_slice_min_digits(slicing.get_min_digits());
    ret.set_ignore_unknown_inode_type(! dont_ignore_unknown_inode_type.get_value_as_bool());
    ret.set_execute(execute.get_value());
    ret.set_entrepot(entrep->get_entrepot(webui));
    ret.set_selection(*(filename_mask->get_mask()));
    ret.set_subtree(*(path_mask->get_mask()));

    vector<string> mp = same_fs.get_included_fs_path();
    for(vector<string>::iterator it = mp.begin(); it != mp.end(); ++it)
	ret.set_same_fs_include(*it);

    mp = same_fs.get_excluded_fs_path();
    for(vector<string>::iterator it = mp.begin(); it != mp.end(); ++it)
	ret.set_same_fs_exclude(*it);

    if(delta_sig.get_value_as_bool())
    {
	unique_ptr<libdar::mask> dmask = delta_mask->get_mask();

	if(dmask)
	{
	    ret.set_delta_signature(true);
	    ret.set_delta_mask(*dmask);
	}
	else
	    throw WEBDAR_BUG;

	ret.set_sig_block_len(sig_block_size.get_value());
	ret.set_delta_sig_min_size(delta_sig_min_size.get_value_as_infinint());
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
       || event_name == html_compression_params::changed
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

	delta_filter_title.set_visible(delta_sig.get_value_as_bool());
	guichet_delta_mask.set_visible(delta_sig.get_value_as_bool());
	delta_sig_min_size.set_visible(delta_sig.get_value_as_bool());
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

	compr_filter_title.set_visible(compr_params->get_compression_algo() != libdar::compression::none);
	guichet_compr_mask.set_visible(compr_params->get_compression_algo() != libdar::compression::none);

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
