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
#include "html_form_mask_expression.hpp"

    //
#include "html_options_merge.hpp"


using namespace std;

const string html_options_merge::entrepot_changed = "html_options_merge_entrep_changed";

html_options_merge::html_options_merge():
    form_archgen("Update"),
    fs_archgen(""),
    allow_over("Allow slice overwriting", html_form_input::check, "", "1"),
    warn_over("Warn before overwriting", html_form_input::check, "", "1"),
    pause("Pause at each N slices (zero = no pause)", html_form_input::number, "", "5"),
    sequential_marks("Add sequential marks", html_form_input::check, "", "1"),
    sparse_file_min_size("Minimum size of holes to lookup in sparse files", 0, "30"),
    user_comment("User comment in slice header", html_form_input::text, "", "80%"),
    hash_algo("Hashing algorithm"),
    execute("Command to execute after each slice", html_form_input::text, "", "80%"),
    empty("Dry run execution", html_form_input::check, "", "1"),
    has_aux_fs(""),
    has_aux("Use an auxiliary archive", html_form_input::check, "", "1"),
    decremental("Build a decremental backup", html_form_input::check, "", "1"),
    aux_form("Update"),
    aux_block(""),
    auxiliary("Auxiliary archive of reference"),
    form_delta_sig("Update"),
    delta_fs(""),
    delta_sig("delta signatures"),
    delta_sig_min_size("Avoid calculating delta signature for file smaller than", 0, "30"),
    form_shown("Update"),
    fs_shown(""),
    info_details("Detailed informations", html_form_input::check, "1", "1"),
    display_treated("Display treated files", html_form_input::check, "1", "1"),
    display_treated_only_dir("Display only treated directories", html_form_input::check, "", "1"),
    display_skipped("Display skipped files", html_form_input::check, "1", "1"),
    form_perimeter("Update"),
    fs_perimeter(""),
    empty_dir("Store ignored directories as empty directories", html_form_input::check, "", "1"),
    overwriting_policy(""),
    form_overwriting("Update")
{

    entrep.reset(new (nothrow) html_entrepot());
    if(!entrep)
	throw exception_memory();

    delta_mask.reset(new (nothrow) html_mask_form_filename("file name"));
    if(!delta_mask)
	throw exception_memory();

    filename_mask.reset(new (nothrow) html_mask_form_filename("file name"));
    if(!filename_mask)
	throw exception_memory();

    ea_mask.reset(new (nothrow) html_mask_form_filename("extended attribute"));
    if(!ea_mask)
	throw exception_memory();

    compr_params.reset(new (nothrow) html_compression_params(true, true, true));
    if(!compr_params)
	throw exception_memory();

    compr_mask.reset(new (nothrow) html_mask_form_filename("file name"));
    if(!compr_mask)
	throw exception_memory();

    path_mask.reset(new (nothrow) html_mask_form_path(false));
    if(!path_mask)
	throw exception_memory();

	// components setups
    pause.set_min_only(0);

    libdar::archive_options_merge defaults;

	// setting default values from libdar
    delta_sig.add_choice("drop", "Drop all signatures");
    delta_sig.add_choice("transfer", "Transfer signatures as is");
    delta_sig.add_choice("compute", "Define signatures to transfer or compute");
    delta_sig.set_selected(1); // transfer by default

    allow_over.set_value_as_bool(defaults.get_allow_over());
    warn_over.set_value_as_bool(defaults.get_warn_over());
    pause.set_value(libdar::deci(defaults.get_pause()).human());
    sequential_marks.set_value_as_bool(defaults.get_sequential_marks());
    sparse_file_min_size.set_value_as_infinint(defaults.get_sparse_file_min_size());
    user_comment.set_value(defaults.get_user_comment());
    hash_algo.set_value(defaults.get_hash_algo());
    execute.set_value(defaults.get_execute());
    empty.set_value_as_bool(defaults.get_empty());
    empty_dir.set_value_as_bool(defaults.get_empty_dir());
    delta_sig_min_size.set_value_as_infinint(defaults.get_delta_sig_min_size());

    delta_filter_title.add_paragraph();
    delta_filter_title.add_text(3, "Delta signature filename based filtering");
    compr_filter_title.add_paragraph();
    compr_filter_title.add_text(3, "Compression filename based filtering");

    slicing.set_permission(defaults.get_slice_permission());
    slicing.set_user_ownership(defaults.get_slice_user_ownership());
    slicing.set_group_ownership(defaults.get_slice_group_ownership());
    slicing.set_min_digits(defaults.get_slice_min_digits());
    slicing.set_slicing(defaults.get_slice_size(), defaults.get_first_slice_size());
    ciphering.set_crypto_size_range(defaults.get_crypto_size(), libdar::infinint(4294967296)); // max is 2^32

	// building HTML structure

    static const char* sect_entrep = "entrepot";
    static const char* sect_general = "general";
    static const char* sect_aux = "auxiliary";
    static const char* sect_delta = "delta_sig";
    static const char* sect_show = "show";
    static const char* sect_filter = "filter";
    static const char* sect_mask_file = "mask_file";
    static const char* sect_mask_path = "mask_path";
    static const char* sect_ea_mask = "EA masks";
    static const char* sect_fsa_scope = "FSA Scope";
    static const char* sect_overwrite = "overwrite";
    static const char* sect_compr = "slicing";
    static const char* sect_slice = "compression";
    static const char* sect_cipher = "ciphering";

    deroule.add_section(sect_entrep, "Backup Repository");
    deroule.add_section(sect_general, "How to process the operation");
    deroule.add_section(sect_show, "What to show during the operation");
    deroule.add_section(sect_aux, "Auxiliary Backup of Reference");
    deroule.add_section(sect_delta, "Delta signatures");
    deroule.add_section(sect_filter, "What to take into consideration for Merging");
    deroule.add_section(sect_mask_file, "File name based filtering");
    deroule.add_section(sect_mask_path, "Path based filterting");
    deroule.add_section(sect_ea_mask, "Extended Attributes filtering");
    deroule.add_section(sect_fsa_scope, "File system Specific Attributes filtering");
    deroule.add_section(sect_overwrite, "Overwriting policy");
    deroule.add_section(sect_compr, "Compression Options");
    deroule.add_section(sect_slice, "Slicing Options");
    deroule.add_section(sect_cipher, "Encryption Options");

    deroule.adopt_in_section(sect_entrep, &guichet_entrep);

    fs_archgen.adopt(&allow_over);
    fs_archgen.adopt(&warn_over);
    fs_archgen.adopt(&pause);
    fs_archgen.adopt(&sequential_marks);
    fs_archgen.adopt(&sparse_file_min_size);
    fs_archgen.adopt(&user_comment);
    fs_archgen.adopt(&hash_algo);
    fs_archgen.adopt(&execute);
    fs_archgen.adopt(&empty);
    form_archgen.adopt(&fs_archgen);
    deroule.adopt_in_section(sect_general, &form_archgen);

    has_aux_fs.adopt(&has_aux);
    has_aux_fs.adopt(&decremental);
    aux_form.adopt(&has_aux_fs);
    aux_block.adopt(&aux_form);
    aux_block.adopt(&auxiliary);
    deroule.adopt_in_section(sect_aux, &aux_block);

    delta_fs.adopt(&delta_sig);
    delta_fs.adopt(&delta_sig_min_size);
    delta_fs.adopt(&sig_block_size);
    form_delta_sig.adopt(&delta_fs);
    deroule.adopt_in_section(sect_delta, &form_delta_sig);
    deroule.adopt_in_section(sect_delta, &delta_filter_title);
    deroule.adopt_in_section(sect_delta, &guichet_delta_mask);

    fs_shown.adopt(&info_details);
    fs_shown.adopt(&display_treated);
    fs_shown.adopt(&display_treated_only_dir);
    fs_shown.adopt(&display_skipped);
    form_shown.adopt(&fs_shown);
    deroule.adopt_in_section(sect_show, &form_shown);

    fs_perimeter.adopt(&empty_dir);
    form_perimeter.adopt(&fs_perimeter);
    deroule.adopt_in_section(sect_filter, &form_perimeter);

    deroule.adopt_in_section(sect_mask_file, &guichet_filename_mask);
    deroule.adopt_in_section(sect_mask_path, &guichet_path_mask);
    deroule.adopt_in_section(sect_ea_mask, &guichet_ea_mask);
    deroule.adopt_in_section(sect_fsa_scope, &fsa_scope);

    form_overwriting.adopt(&overwriting_policy);
    deroule.adopt_in_section(sect_overwrite, &form_overwriting);

    deroule.adopt_in_section(sect_compr, &guichet_compr_params);
    deroule.adopt_in_section(sect_compr, &compr_filter_title);
    deroule.adopt_in_section(sect_compr, &guichet_compr_mask);

    deroule.adopt_in_section(sect_slice, &slicing);

    deroule.adopt_in_section(sect_cipher, &ciphering);

    adopt(&deroule);

	// events and visibility
    register_name(entrepot_changed);

    delta_sig.record_actor_on_event(this, html_form_select::changed);
    display_treated.record_actor_on_event(this, html_form_input::changed);
    compr_params->record_actor_on_event(this, html_compression_params::changed);
    has_aux.record_actor_on_event(this, html_form_input::changed);
    entrep->record_actor_on_event(this, html_entrepot::changed);

    on_event(html_form_input::changed);

	// CSS
    webdar_css_style::grey_button(deroule, true);
    display_treated_only_dir.add_css_class(webdar_css_style::wcs_indent);
}

void html_options_merge::set_biblio(const std::shared_ptr<bibliotheque> & ptr)
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
    auxiliary.set_biblio(ptr);
}

void html_options_merge::on_event(const string & event_name)
{
    if(event_name == html_form_input::changed
       || event_name == html_form_select::changed
       || event_name == html_compression_params::changed)
    {
	auxiliary.set_visible(has_aux.get_value_as_bool());
	decremental.set_visible(has_aux.get_value_as_bool());
	delta_filter_title.set_visible(delta_sig.get_selected_num() == 2);
	guichet_delta_mask.set_visible(delta_sig.get_selected_num() == 2);
	delta_sig_min_size.set_visible(delta_sig.get_selected_num() == 2);
	sig_block_size.set_visible(delta_sig.get_selected_num() == 2);
	display_treated_only_dir.set_visible(display_treated.get_value_as_bool());

	if(! compr_params->get_keep_compressed()
	   && compr_params->get_compression_algo() != libdar::compression::none)
	{
	    compr_filter_title.set_visible(true);
	    guichet_compr_mask.set_visible(true);
	}
	else
	{
	    compr_filter_title.set_visible(false);
	    guichet_compr_mask.set_visible(false);
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

    ret.set_entrepot(entrep->get_entrepot(webui));
    ret.set_allow_over(allow_over.get_value_as_bool());
    ret.set_warn_over(warn_over.get_value_as_bool());
    ret.set_pause(libdar::deci(pause.get_value()).computer());
    ret.set_slice_permission(slicing.get_permission());
    ret.set_slice_user_ownership(slicing.get_user_ownership());
    ret.set_slice_group_ownership(slicing.get_group_ownership());
    ret.set_sequential_marks(sequential_marks.get_value_as_bool());
    ret.set_sparse_file_min_size(sparse_file_min_size.get_value_as_infinint());
    ret.set_user_comment(user_comment.get_value());
    ret.set_slice_min_digits(slicing.get_min_digits());
    ret.set_hash_algo(hash_algo.get_value());
    ret.set_empty(empty.get_value_as_bool());
    ret.set_execute(execute.get_value());
    ret.set_info_details(info_details.get_value_as_bool());
    ret.set_display_treated(display_treated.get_value_as_bool(),
			    display_treated_only_dir.get_value_as_bool());
    ret.set_display_skipped(display_skipped.get_value_as_bool());
    ret.set_empty_dir(empty_dir.get_value_as_bool());
    ret.set_selection(*(filename_mask->get_mask()));
    ret.set_subtree(*(path_mask->get_mask()));
    ret.set_overwriting_rules(*(overwriting_policy.get_overwriting_action()));
    if(! compr_params->get_keep_compressed())
    {
	ret.set_keep_compressed(false);
	ret.set_compression(compr_params->get_compression_algo());
	ret.set_compression_level(compr_params->get_compression_level());
	ret.set_min_compr_size(compr_params->get_min_compression_size());

	libdar::U_I val = webdar_tools_convert_from_infinint<libdar::U_I>(compr_params->get_compression_block(),
									  string("compression block size is too large for the underlying operating system, please reduce"));

	if(val < tokens_min_compr_bs && val != 0)
	    throw exception_range(libdar::tools_printf("compression block size is too small, select either zero to disable compression per block or a block size greater or equal to %d", tokens_min_compr_bs));
	ret.set_compression_block_size(val);

	unique_ptr<libdar::mask> libcompmask = compr_mask->get_mask();
	if(!libcompmask)
	    throw WEBDAR_BUG;
	ret.set_compr_mask(*libcompmask);
	ret.set_multi_threaded_compress(compr_params->get_num_threads());
    }
    else
	ret.set_keep_compressed(true);


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

	ret.set_decremental_mode(decremental.get_value_as_bool());
    }

    unique_ptr<libdar::mask> dmask;
    switch(delta_sig.get_selected_num())
    {
    case 0: // drop
	ret.set_delta_signature(false);
	break;
    case 1: // transfer
	ret.set_delta_signature(true);
	break;
    case 2: // transfer and recompute
	dmask = delta_mask->get_mask();

	if(dmask)
	{
	    ret.set_delta_signature(true);
	    ret.set_delta_mask(*dmask);
	}
	else
	    throw WEBDAR_BUG;

	ret.set_sig_block_len(sig_block_size.get_value());
	ret.set_delta_sig_min_size(delta_sig_min_size.get_value_as_infinint());
	break;
    default:
	throw WEBDAR_BUG;
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
