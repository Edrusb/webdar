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

const string html_options_merge::landing_path_changed = "html_options_merge_landing_changed";
const string html_options_merge::entrepot_changed = "html_options_merge_entrep_changed";
const string html_options_merge::changed = "html_options_merge_changed";

html_options_merge::html_options_merge():
    form_archgen("Update"),
    fs_archgen(""),
    pause("Pause at each N slices (zero = no pause)", html_form_input::number, "", "5", ""),
    allow_over("Allow slice overwriting", html_form_input::check, "", "", ""),
    warn_over("Warn before overwriting", html_form_input::check, "", "", ""),
    sequential_marks("Add sequential marks", html_form_input::check, "", "", ""),
    sparse_file_min_size("Minimum size of holes to lookup in sparse files", 0, "30"),
    user_comment("User comment in slice header", html_form_input::text, "", "", webdar_css_style::width_100vw),
    hash_algo("Hashing algorithm"),
    execute("Command to execute after each slice", html_form_input::text, "", "", webdar_css_style::width_100vw),
    empty("Dry run execution", html_form_input::check, "", "", ""),
    has_aux_fs(""),
    has_aux("Use an auxiliary archive", html_form_input::check, "", "", ""),
    decremental("Build a decremental backup", html_form_input::check, "", "", ""),
    aux_form("Update"),
    aux_block(""),
    auxiliary("Auxiliary archive of reference"),
    form_delta_sig("Update"),
    delta_fs(""),
    delta_sig("delta signatures"),
    form_shown("Update"),
    fs_shown(""),
    info_details("Detailed informations", html_form_input::check, "1", "", ""),
    display_treated("Display treated files", html_form_input::check, "1", "", ""),
    display_treated_only_dir("Display only treated directories", html_form_input::check, "", "", ""),
    display_skipped("Display skipped files", html_form_input::check, "1", "", ""),
    form_perimeter("Update"),
    fs_perimeter(""),
    empty_dir("Store ignored directories as empty directories", html_form_input::check, "", "", ""),
    ignore_events(false)
{

    entrep.reset(new (nothrow) html_entrepot_landing());
    if(!entrep)
	throw exception_memory();

    delta_mask.reset(new (nothrow) html_mask_form_path(false));
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

    slicing.reset(new (nothrow) html_slicing());
    if(! slicing)
	throw exception_memory();

    ciphering.reset(new (nothrow) html_ciphering());
    if(! ciphering)
	throw exception_memory();

    sig_block_size.reset(new (nothrow) html_form_sig_block_size());
    if(! sig_block_size)
	throw exception_memory();

    overwriting_policy.reset(new (nothrow) html_form_overwrite_action(""));
    if(! overwriting_policy)
	throw exception_memory();

	// components setups
    pause.set_min_only(0);

	// setting default values from libdar
    delta_sig.add_choice("drop", "Drop all signatures");
    delta_sig.add_choice("transfer", "Transfer signatures as is");
    delta_sig.add_choice("compute", "Define signatures to transfer or compute");

    delta_filter_params.add_paragraph();
    delta_filter_params.add_text(3, "Delta signature parameters");
    delta_filter_title.add_paragraph();
    delta_filter_title.add_text(3, "Delta signature filename based filtering");
    compr_filter_title.add_paragraph();
    compr_filter_title.add_text(3, "Compression filename based filtering");

    libdar::archive_options_merge defaults;
    ciphering->set_crypto_size_range(defaults.get_crypto_size(), libdar::infinint(4294967296)); // max is 2^32
    sig_block_size->set_delta_sig_min_size(defaults.get_delta_sig_min_size());

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
    form_delta_sig.adopt(&delta_fs);
    deroule.adopt_in_section(sect_delta, &form_delta_sig);
    deroule.adopt_in_section(sect_delta, &delta_filter_params);
    deroule.adopt_in_section(sect_delta, &guichet_sig_block_size);
    deroule.adopt_in_section(sect_delta, &delta_filter_title);
    deroule.adopt_in_section(sect_delta, &guichet_delta_mask);

	// by-passing deroule/html_derouleur anchor assigment
    delta_filter_params.assign_anchor(true);
    guichet_sig_block_size.bind_to_anchor(delta_filter_params.get_assigned_anchor());
    delta_filter_title.assign_anchor(true);
    guichet_delta_mask.bind_to_anchor(delta_filter_title.get_assigned_anchor());

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


    deroule.adopt_in_section(sect_overwrite, &guichet_overwriting_policy);

    deroule.adopt_in_section(sect_compr, &guichet_compr_params);
    deroule.adopt_in_section(sect_compr, &compr_filter_title);
    deroule.adopt_in_section(sect_compr, &guichet_compr_mask);

	// by-passing anchor assigned by deroule
    compr_filter_title.assign_anchor(true);
    guichet_compr_mask.bind_to_anchor(compr_filter_title.get_assigned_anchor());


    deroule.adopt_in_section(sect_slice, &guichet_slicing);

    deroule.adopt_in_section(sect_cipher, &guichet_ciphering);

    adopt(&deroule);

	// events and visibility
    register_name(landing_path_changed);
    register_name(entrepot_changed);
    register_name(changed);

    delta_sig.record_actor_on_event(this, html_form_select::changed);
    display_treated.record_actor_on_event(this, html_form_input::changed);
    compr_params->record_actor_on_event(this, html_compression_params::changed);
    has_aux.record_actor_on_event(this, html_form_input::changed);
    entrep->record_actor_on_event(this, html_entrepot_landing::entrepot_only_changed);
    entrep->record_actor_on_event(this, html_entrepot_landing::landing_path_only_changed);

    allow_over.record_actor_on_event(this, html_form_input::changed);
    warn_over.record_actor_on_event(this, html_form_input::changed);
    pause.record_actor_on_event(this, html_form_input::changed);
    sequential_marks.record_actor_on_event(this, html_form_input::changed);
    sparse_file_min_size.record_actor_on_event(this, html_form_input_unit::changed);
    user_comment.record_actor_on_event(this, html_form_input::changed);
    hash_algo.record_actor_on_event(this, html_hash_algo::changed);
    execute.record_actor_on_event(this, html_form_input::changed);
    empty.record_actor_on_event(this, html_form_input::changed);
    decremental.record_actor_on_event(this, html_form_input::changed);
    auxiliary.record_actor_on_event(this, html_archive_read::changed);
    sig_block_size->record_actor_on_event(this, html_form_sig_block_size::changed);
    delta_mask->record_actor_on_event(this, html_mask_form_path::changed);
    info_details.record_actor_on_event(this, html_form_input::changed);
    display_treated_only_dir.record_actor_on_event(this, html_form_input::changed);
    display_skipped.record_actor_on_event(this, html_form_input::changed);
    empty_dir.record_actor_on_event(this, html_form_input::changed);
    filename_mask->record_actor_on_event(this, html_mask_form_filename::changed);
    path_mask->record_actor_on_event(this, html_mask_form_path::changed);
    ea_mask->record_actor_on_event(this, html_mask_form_filename::changed);
    fsa_scope.record_actor_on_event(this, html_fsa_scope::changed);
    overwriting_policy->record_actor_on_event(this, html_form_overwrite_action::changed);
    compr_mask->record_actor_on_event(this, html_mask_form_filename::changed);
    slicing->record_actor_on_event(this, html_slicing::changed);
    ciphering->record_actor_on_event(this, html_ciphering::changed);

    on_event(html_form_input::changed);

	// CSS
    webdar_css_style::grey_button(deroule, true);
    display_treated_only_dir.add_css_class(webdar_css_style::indent);
    allow_over.add_css_class(webdar_css_style::width_100vw_8em);
    warn_over.add_css_class(webdar_css_style::width_100vw_8em);
    pause.add_css_class(webdar_css_style::width_100vw_8em);
    sequential_marks.add_css_class(webdar_css_style::width_100vw_8em);
    user_comment.add_css_class(webdar_css_style::width_100vw_8em);
    execute.add_css_class(webdar_css_style::width_100vw_8em);
    empty.add_css_class(webdar_css_style::width_100vw_8em);
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

    delta_mask->set_child(ptr, bibliotheque::pathfilter);
    guichet_delta_mask.set_child(ptr,
				 bibliotheque::pathfilter,
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
    guichet_slicing.set_child(ptr,
			      bibliotheque::slicing,
			      slicing,
			      false);
    guichet_ciphering.set_child(ptr,
				bibliotheque::ciphering,
				ciphering,
				false);
    guichet_sig_block_size.set_child(ptr,
				     bibliotheque::delta_sig,
				     sig_block_size,
				     true);
    guichet_overwriting_policy.set_child(ptr,
					 bibliotheque::over_policy,
					 overwriting_policy,
					 true);

    init();
}

void html_options_merge::load_json(const json & source)
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
	    allow_over.set_value_as_bool(config.at(jlabel_allow_over));
	    warn_over.set_value_as_bool(config.at(jlabel_warn_over));
	    pause.set_value_as_bool(config.at(jlabel_pause));
	    sequential_marks.set_value_as_bool(config.at(jlabel_seq_marks));
	    sparse_file_min_size.set_value_as_infinint(libdar::deci(config.at(jlabel_sparse_min_size)).computer());
	    user_comment.set_value(config.at(jlabel_user_comment));
	    hash_algo.set_selected_id(config.at(jlabel_hash_algo));
	    execute.set_value(config.at(jlabel_execute));
	    empty.set_value_as_bool(config.at(jlabel_empty));
	    has_aux.set_value_as_bool(config.at(jlabel_has_aux));
	    if(has_aux.get_value_as_bool())
		auxiliary.load_json(config.at(jlabel_auxiliary));
	    else
		auxiliary.clear_json();
	    decremental.set_value_as_bool(config.at(jlabel_decremental));
	    delta_sig.set_selected_id(config.at(jlabel_delta_sig));
	    if(delta_sig.get_selected_id() == "compute")
	    {
		guichet_sig_block_size.load_json(config.at(jlabel_sig_block_size));
		guichet_delta_mask.load_json(config.at(jlabel_delta_mask));
	    }
	    else
	    {
		guichet_sig_block_size.clear_json();
		guichet_delta_mask.clear_json();
	    }
	    info_details.set_value_as_bool(config.at(jlabel_info_details));
	    display_treated.set_value_as_bool(config.at(jlabel_display_treated));
	    display_treated_only_dir.set_value_as_bool(config.at(jlabel_display_only_dir));
	    display_skipped.set_value_as_bool(config.at(jlabel_display_skipped));
	    empty_dir.set_value_as_bool(config.at(jlabel_empty_dir));
	    guichet_filename_mask.load_json(config.at(jlabel_filename_mask));
	    guichet_path_mask.load_json(config.at(jlabel_path_mask));
	    guichet_ea_mask.load_json(config.at(jlabel_ea_mask));
	    fsa_scope.load_json(config.at(jlabel_fsa_scope));
	    guichet_overwriting_policy.load_json(config.at(jlabel_overwrite_policy));
	    guichet_compr_params.load_json(config.at(jlabel_compr_params));
	    guichet_compr_mask.load_json(config.at(jlabel_compr_mask));
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

json html_options_merge::save_json() const
{
    json config;
    html_options_merge* me = const_cast<html_options_merge*>(this);

    if(me == nullptr)
	throw WEBDAR_BUG;

    config[jlabel_entrep] = guichet_entrep.save_json();
    config[jlabel_allow_over] = allow_over.get_value_as_bool();
    config[jlabel_warn_over] = warn_over.get_value_as_bool();
    config[jlabel_pause] = pause.get_value_as_bool();
    config[jlabel_seq_marks] = sequential_marks.get_value_as_bool();
    config[jlabel_sparse_min_size] = libdar::deci(sparse_file_min_size.get_value_as_infinint()).human();
    config[jlabel_user_comment] = user_comment.get_value();
    config[jlabel_hash_algo] = hash_algo.get_selected_id();
    config[jlabel_execute] = execute.get_value();
    config[jlabel_empty] = empty.get_value_as_bool();
    config[jlabel_has_aux] = has_aux.get_value_as_bool();
    if(has_aux.get_value_as_bool())
	config[jlabel_auxiliary] = auxiliary.save_json();
    else
	me->auxiliary.clear_json();
    config[jlabel_decremental] = decremental.get_value_as_bool();
    config[jlabel_delta_sig] = delta_sig.get_selected_id();
    if(delta_sig.get_selected_id() == "compute")
    {
	config[jlabel_sig_block_size] = guichet_sig_block_size.save_json();
	config[jlabel_delta_mask] = guichet_delta_mask.save_json();
    }
    else
    {
	me->guichet_sig_block_size.clear_json();
	me->guichet_delta_mask.clear_json();
    }
    config[jlabel_info_details] = info_details.get_value_as_bool();
    config[jlabel_display_treated] = display_treated.get_value_as_bool();
    config[jlabel_display_only_dir] = display_treated_only_dir.get_value_as_bool();
    config[jlabel_display_skipped] = display_skipped.get_value_as_bool();
    config[jlabel_empty_dir] = empty_dir.get_value_as_bool();
    config[jlabel_filename_mask] = guichet_filename_mask.save_json();
    config[jlabel_path_mask] = guichet_path_mask.save_json();
    config[jlabel_ea_mask] = guichet_ea_mask.save_json();
    config[jlabel_fsa_scope] = fsa_scope.save_json();
    config[jlabel_overwrite_policy] = guichet_overwriting_policy.save_json();
    config[jlabel_compr_params] = guichet_compr_params.save_json();
    config[jlabel_compr_mask] = guichet_compr_mask.save_json();
    config[jlabel_slicing] = guichet_slicing.save_json();
    config[jlabel_ciphering] = guichet_ciphering.save_json();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					config);
}

void html_options_merge::clear_json()
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
    ignore_events = true;
    trigger_changed();
}

bibliotheque::using_set html_options_merge::get_using_set() const
{
    bibliotheque::using_set ret;

    subconfig_add_to(ret, guichet_entrep.get_using_set());
    subconfig_add_to(ret, auxiliary.get_using_set());
    subconfig_add_to(ret, guichet_sig_block_size.get_using_set());
    subconfig_add_to(ret, guichet_delta_mask.get_using_set());
    subconfig_add_to(ret, guichet_filename_mask.get_using_set());
    subconfig_add_to(ret, guichet_path_mask.get_using_set());
    subconfig_add_to(ret, guichet_ea_mask.get_using_set());
    subconfig_add_to(ret, guichet_overwriting_policy.get_using_set());
    subconfig_add_to(ret, guichet_compr_mask.get_using_set());
    subconfig_add_to(ret, guichet_slicing.get_using_set());
    subconfig_add_to(ret, guichet_ciphering.get_using_set());

    return ret;
}

void html_options_merge::on_event(const string & event_name)
{
    if(event_name == html_form_input::changed
       || event_name == html_form_select::changed
       || event_name == html_compression_params::changed)
    {
	auxiliary.set_visible(has_aux.get_value_as_bool());
	decremental.set_visible(has_aux.get_value_as_bool());
	delta_filter_params.set_visible(delta_sig.get_selected_num() == 2);
	delta_filter_title.set_visible(delta_sig.get_selected_num() == 2);
	guichet_delta_mask.set_visible(delta_sig.get_selected_num() == 2);
	guichet_sig_block_size.set_visible(delta_sig.get_selected_num() == 2);
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
	trigger_changed();
    }
    else if(event_name == html_entrepot_landing::entrepot_only_changed)
    {
	act(entrepot_changed);
	trigger_changed();
    }
    else if(event_name == html_hash_algo::changed
	    || event_name == html_archive_read::changed
	    || event_name == html_form_sig_block_size::changed
	    || event_name == html_mask_form_filename::changed
	    || event_name == html_mask_form_path::changed
	    || event_name == html_fsa_scope::changed
	    || event_name == html_form_overwrite_action::changed
	    || event_name == html_slicing::changed
	    || event_name == html_ciphering::changed)
    {
	trigger_changed();
    }
    else if(event_name == html_entrepot_landing::landing_path_only_changed)
    {
	act(landing_path_changed);
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
    ret.set_slice_permission(slicing->get_permission());
    ret.set_slice_user_ownership(slicing->get_user_ownership());
    ret.set_slice_group_ownership(slicing->get_group_ownership());
    ret.set_sequential_marks(sequential_marks.get_value_as_bool());
    ret.set_sparse_file_min_size(sparse_file_min_size.get_value_as_infinint());
    ret.set_user_comment(user_comment.get_value());
    ret.set_slice_min_digits(slicing->get_min_digits());
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
    ret.set_overwriting_rules(*(overwriting_policy->get_overwriting_action()));
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
    slicing->get_slicing(s_size, f_s_size);
    ret.set_slicing(s_size, f_s_size);
    ret.set_ea_mask(*(ea_mask->get_mask()));
    ret.set_fsa_scope(fsa_scope.get_scope());

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

	ret.set_sig_block_len(sig_block_size->get_value());
	ret.set_delta_sig_min_size(sig_block_size->get_delta_sig_min_size());
	break;
    default:
	throw WEBDAR_BUG;
    }

    return ret;
}

void html_options_merge::set_to_webdar_defaults()
{
    clear_json();

    guichet_entrep.load_from_bibliotheque(bibliotheque::default_config_name);
    allow_over.set_value_as_bool(true);
    warn_over.set_value_as_bool(true);
    sequential_marks.set_value_as_bool(true);
    hash_algo.set_value(libdar::hash_algo::whirlpool);
    delta_sig.set_selected_id("transfer");
    info_details.set_value_as_bool(true);
    display_treated.set_value_as_bool(true);
    display_treated_only_dir.set_value_as_bool(false);
    display_skipped.set_value_as_bool(true);
    empty_dir.set_value_as_bool(true);
    guichet_overwriting_policy.load_from_bibliotheque(bibliotheque::default_config_name);
    guichet_compr_params.clear_json();
    compr_params->set_keep_compressed(true);
    guichet_slicing.load_from_bibliotheque(bibliotheque::default_config_name);
    guichet_ciphering.load_from_bibliotheque(bibliotheque::default_config_name);
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

void html_options_merge::init()
{
    libdar::archive_options_merge defaults;

    guichet_entrep.clear_json();
    allow_over.set_value_as_bool(defaults.get_allow_over());
    warn_over.set_value_as_bool(defaults.get_warn_over());
    pause.set_value(libdar::deci(defaults.get_pause()).human());
    sequential_marks.set_value_as_bool(defaults.get_sequential_marks());
    sparse_file_min_size.set_value_as_infinint(defaults.get_sparse_file_min_size());
    user_comment.set_value(defaults.get_user_comment());
    hash_algo.set_value(defaults.get_hash_algo());
    execute.set_value(defaults.get_execute());
    empty.set_value_as_bool(defaults.get_empty());
    has_aux.set_value_as_bool(false);
    decremental.set_value_as_bool(defaults.get_decremental_mode());
    auxiliary.clear_json();
    delta_sig.set_selected_num(1); // transfer by default
    guichet_sig_block_size.clear_json();
    guichet_delta_mask.clear_json();
    info_details.set_value_as_bool(defaults.get_info_details());
    display_treated.set_value_as_bool(defaults.get_display_treated());
    display_treated_only_dir.set_value_as_bool(defaults.get_display_treated_only_dir());
    display_skipped.set_value_as_bool(defaults.get_display_skipped());
    empty_dir.set_value_as_bool(defaults.get_empty_dir());
    guichet_filename_mask.clear_json();
    guichet_path_mask.clear_json();
    guichet_ea_mask.clear_json();
    fsa_scope.clear_json();
    guichet_overwriting_policy.clear_json();
    guichet_compr_params.clear_json();
    guichet_compr_mask.clear_json();
    guichet_slicing.clear_json();
    guichet_ciphering.clear_json();
}

void html_options_merge::trigger_changed()
{
    if(! ignore_events)
	act(changed);
}
