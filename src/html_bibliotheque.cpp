/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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
#include <fstream>
#include <dar/tools.hpp>

    // webdar headers
#include "environment.hpp"
#include "tokens.hpp"
#include "html_form_overwrite_action.hpp"
#include "html_form_overwrite_chain_action.hpp"
#include "html_form_overwrite_conditional_action.hpp"
#include "html_form_overwrite_combining_criterium.hpp"
#include "html_form_overwrite_base_criterium.hpp"
#include "html_form_overwrite_constant_action.hpp"

    //
#include "html_bibliotheque.hpp"

using namespace std;

const string html_bibliotheque::event_download = "html_bibl_download";


html_bibliotheque::html_bibliotheque(shared_ptr<bibliotheque> & ptr,
				     const string & bib_path):
    top_fs(""),
    form("Update"),
    filename_fs(""),
    filename("Filename", bib_path, "", webdar_css_style::width_80vw, "Select file used to store configurations"),
    autosave("Auto-saving to file", html_form_input::check, "x", "", ""),
    save("Save", event_save),
    load("Load", event_load),
    bot_fs(""),
    upload_form("Upload"),
    upload_file("Webdar configuration to upload", html_form_input::file, "", "", ""),
    down_fs(""),
    download("Download", event_download),
    clear_fs(""),
    clear_conf("Clear all configurations", event_clear),
    generate_defaults("Regenerate missing default configs", event_defaults),
    expect_upload(false),
    q_context(context_undefined)
{
    unique_ptr<html_entrepot_landing> tmp_e;
    unique_ptr<html_mask_form_filename> tmp_fm;
    unique_ptr<html_mask_form_path> tmp_fp;
    unique_ptr<html_compression_params> tmp_compr;
    unique_ptr<html_slicing> tmp_slicing;
    unique_ptr<html_ciphering> tmp_ciphering;
    unique_ptr<html_form_sig_block_size> tmp_delta_sig;
    unique_ptr<html_form_overwrite_action> tmp_over_policy;
    unique_ptr<html_options_test> tmp_options_test;
    unique_ptr<html_options_compare> tmp_options_compare;
    unique_ptr<html_options_read> tmp_options_read;
    unique_ptr<html_options_create> tmp_options_create;
    unique_ptr<html_options_isolate> tmp_options_isolate;
    unique_ptr<html_options_merge> tmp_options_merge;
    unique_ptr<html_options_repair> tmp_options_repair;
    unique_ptr<html_options_extract> tmp_options_extract;

    if(!ptr)
	throw WEBDAR_BUG;

    biblio = ptr;
    if(! biblio)
	throw WEBDAR_BUG;

    upload_form.set_enctype("multipart/form-data");
    ok_loaded.add_text(0, "Configuration loaded successfully!");
    ok_saved.add_text(0, "Configuration saved successfully!");
    ok_uploaded.add_text(0, "Configuration uploaded successfully!");
    ok_cleared.add_text(0, "Configuration cleared successfully!");
    download.set_download(true);
    download.set_filename("webdar.json");

    intro.add_text(3, "Configurations");
    intro.add_text(0, "This main tab gives the ability to save to file, load from file,");
    intro.add_text(0, "download and upload through HTTP(s) the whole configurations defined in all the following tabs.");
    intro.add_paragraph();
    intro.add_text(0, "Note: If saving to file or loading from file should usually be fine in term of security (this is saved locally, on the host where webdar is running),");
    intro.add_text(0, "this is not true while downloading or uploading over a non-secured session (not secured for http sessions, but OK for https session):");
    intro.add_text(0, "All configuration information, including login and passwords used to cipher archive and backups or to connect to remote repositories, if you have enterred such type of information");
    intro.add_text(0, "in the followin tabs, would be transmitted in clear and accessible to anyone");
    intro.add_text(0, "having access to the underneath network used between Webdar and your browser.");
    intro.add_paragraph();
    intro.add_text(0, "This security warning also applies when using untrusted SSL certificates for Webdar (risk of man-in-the-middle attack).");

    saved_status.add_text(0, "All configurations saved to file");
    unsaved_status.add_text(0, "Not all configurations saved to file!");

	// main tab

    filename_fs.adopt(&filename);
    filename_fs.adopt(&autosave);
    form.adopt(&filename_fs);
    top_fs.adopt(&form);
    top_fs.adopt(&save);
    top_fs.adopt(&load);
    top_fs.adopt(&ok_loaded);
    top_fs.adopt(&ok_saved);

    upload_form.adopt(&upload_file);
    bot_fs.adopt(&upload_form);
    bot_fs.adopt(&ok_uploaded);

    down_fs.adopt(&download);

    clear_fs.adopt(&clear_conf);
    clear_fs.adopt(&generate_defaults);
    clear_fs.adopt(&ok_cleared);

	// entrepot tab

    tmp_e.reset(new (nothrow) html_entrepot_landing());
    if(! tmp_e)
	throw exception_memory();

    ab_entrepot.reset(new (nothrow) arriere_boutique<html_entrepot_landing>(ptr,
									    bibliotheque::repo,
									    tmp_e,
									    html_entrepot_landing::changed,
									    false));
    if(! ab_entrepot)
	throw exception_memory();

	// file mask tab

    tmp_fm.reset(new (nothrow) html_mask_form_filename("mask"));
    if(!tmp_fm)
	throw exception_memory();
    else
	tmp_fm->set_child(ptr, bibliotheque::filefilter);

    ab_filemask.reset(new (nothrow) arriere_boutique<html_mask_form_filename>(ptr,
									      bibliotheque::filefilter,
									      tmp_fm,
									      html_mask_form_filename::changed,
									      false));
    if(!ab_filemask)
	throw exception_memory();

	// path mask tab

    tmp_fp.reset(new (nothrow) html_mask_form_path(true));
	// allowing forbidding absolute path in bibliotheque, if used in a context where not allowed
	// this will trigger error and red-highlight the concerned fields.
    if(! tmp_fp)
	throw exception_memory();
    else
	tmp_fp->set_child(ptr, bibliotheque::pathfilter);

    ab_pathmask.reset(new (nothrow) arriere_boutique<html_mask_form_path>(ptr,
									  bibliotheque::pathfilter,
									  tmp_fp,
									  html_mask_form_path::changed,
									  false));
    if(! ab_pathmask)
	throw exception_memory();

	// compression tab

    tmp_compr.reset(new (nothrow) html_compression_params(true,   // resave uncompressed
							  true,   // min size
							  true)); // show keep compressed
    if(! tmp_compr)
	throw exception_memory();

    ab_compr.reset(new (nothrow) arriere_boutique<html_compression_params>(ptr,
									   bibliotheque::compress,
									   tmp_compr,
									   html_compression_params::changed,
									   false));
    if(!ab_compr)
	throw exception_memory();

	// slicing tab

    tmp_slicing.reset(new (nothrow) html_slicing());
    if(! tmp_slicing)
	throw exception_memory();

    ab_slicing.reset(new (nothrow) arriere_boutique<html_slicing>(ptr,
								  bibliotheque::slicing,
								  tmp_slicing,
								  html_slicing::changed,
								  false));
    if(! ab_slicing)
	throw exception_memory();

	// ciphering tab

    tmp_ciphering.reset(new (nothrow) html_ciphering());
    if(! tmp_ciphering)
	throw exception_memory();

    ab_ciphering.reset(new (nothrow) arriere_boutique<html_ciphering>(ptr,
								      bibliotheque::ciphering,
								      tmp_ciphering,
								      html_ciphering::changed,
								      false));
    if(!ab_ciphering)
	throw exception_memory();

	// delta sig tab

    tmp_delta_sig.reset(new (nothrow) html_form_sig_block_size());
    if(! tmp_delta_sig)
	throw exception_memory();

    ab_delta_sig.reset(new (nothrow) arriere_boutique<html_form_sig_block_size>(ptr,
										bibliotheque::delta_sig,
										tmp_delta_sig,
										html_form_sig_block_size::changed,
										true));
    if(!ab_delta_sig)
	throw exception_memory();

	// overwriting policy

    tmp_over_policy.reset(new (nothrow) html_form_overwrite_action(""));
    if(! tmp_over_policy)
	throw exception_memory();

    ab_over_policy.reset(new (nothrow) arriere_boutique<html_form_overwrite_action>(ptr,
										    bibliotheque::over_policy,
										    tmp_over_policy,
										    html_form_overwrite_action::changed,
										    true));
    if(!ab_over_policy)
	throw exception_memory();

	// options test

    tmp_options_test.reset(new (nothrow) html_options_test());
    if(! tmp_options_test)
	throw exception_memory();
    else
	tmp_options_test->set_biblio(ptr);

    ab_options_test.reset(new (nothrow) arriere_boutique<html_options_test>(ptr,
									    bibliotheque::conftest,
									    tmp_options_test,
									    html_options_test::changed,
									    false));
    if(! ab_options_test)
	throw exception_memory();

	// options compare

    tmp_options_compare.reset(new (nothrow) html_options_compare());
    if(!tmp_options_compare)
	throw exception_memory();
    else
	tmp_options_compare->set_biblio(ptr);

    ab_options_compare.reset(new (nothrow) arriere_boutique<html_options_compare>(ptr,
										  bibliotheque::confdiff,
										  tmp_options_compare,
										  html_options_compare::changed,
										  false));
    if(! ab_options_compare)
	throw exception_memory();

	// options read

    tmp_options_read.reset(new (nothrow) html_options_read());
    if(! tmp_options_read)
	throw exception_memory();
    else
	tmp_options_read->set_biblio(ptr);

    ab_options_read.reset(new (nothrow) arriere_boutique<html_options_read>(ptr,
									    bibliotheque::confread,
									    tmp_options_read,
									    html_options_read::changed,
									    false));
    if(! ab_options_read)
	throw exception_memory();

	// options create

    tmp_options_create.reset(new (nothrow) html_options_create());
    if(! tmp_options_create)
	throw exception_memory();
    else
	tmp_options_create->set_biblio(ptr);

    ab_options_create.reset(new (nothrow) arriere_boutique<html_options_create>(ptr,
										bibliotheque::confsave,
										tmp_options_create,
										html_options_create::changed,
										false));
    if(! ab_options_create)
	throw exception_memory();

	// options isolate

    tmp_options_isolate.reset(new (nothrow) html_options_isolate());
    if(! tmp_options_isolate)
	throw exception_memory();
    else
	tmp_options_isolate->set_biblio(ptr);

    ab_options_isolate.reset(new (nothrow) arriere_boutique<html_options_isolate>(ptr,
										  bibliotheque::confisolate,
										  tmp_options_isolate,
										  html_options_isolate::changed,
										  false));
    if(! ab_options_isolate)
	throw exception_memory();

	// options merge

    tmp_options_merge.reset(new (nothrow) html_options_merge());
    if(! tmp_options_merge)
	throw exception_memory();
    else
	tmp_options_merge->set_biblio(ptr);

    ab_options_merge.reset(new (nothrow) arriere_boutique<html_options_merge>(ptr,
									      bibliotheque::confmerge,
									      tmp_options_merge,
									      html_options_merge::changed,
									      false));
    if(! ab_options_merge)
	throw exception_memory();

	// options repair

    tmp_options_repair.reset(new (nothrow) html_options_repair());
    if(! tmp_options_repair)
	throw exception_memory();
    else
	tmp_options_repair->set_biblio(ptr);

    ab_options_repair.reset(new (nothrow) arriere_boutique<html_options_repair>(ptr,
										bibliotheque::confrepair,
										tmp_options_repair,
										html_options_repair::changed,
										false));
    if(! ab_options_repair)
	throw exception_memory();

	// options extract

    tmp_options_extract.reset(new (nothrow) html_options_extract());
    if(! tmp_options_extract)
	throw exception_memory();
    else
	tmp_options_extract->set_biblio(ptr);

    ab_options_extract.reset(new (nothrow) arriere_boutique<html_options_extract>(ptr,
										  bibliotheque::confrest,
										  tmp_options_extract,
										  html_options_extract::changed,
										  false));
    if(! ab_options_extract)
	throw exception_memory();


    	// global component setups

    tabs.add_tab("Global Settings", tab_main);
    tabs.add_tab(bibliotheque::category_description(bibliotheque::repo, true), tab_repo);
    tabs.add_tab(bibliotheque::category_description(bibliotheque::delta_sig, true), tab_delta_sig);
    tabs.add_tab(bibliotheque::category_description(bibliotheque::filefilter, true), tab_filemask);
    tabs.add_tab(bibliotheque::category_description(bibliotheque::pathfilter, true), tab_pathmask);
    tabs.add_tab(bibliotheque::category_description(bibliotheque::compress, true), tab_compression);
    tabs.add_tab(bibliotheque::category_description(bibliotheque::slicing, true), tab_slicing);
    tabs.add_tab(bibliotheque::category_description(bibliotheque::ciphering, true), tab_ciphering);
    tabs.add_tab(bibliotheque::category_description(bibliotheque::over_policy, true), tab_over_policy);
    tabs.add_tab(bibliotheque::category_description(bibliotheque::confsave, true), tab_options_create);
    tabs.add_tab(bibliotheque::category_description(bibliotheque::confread, true), tab_options_read);
    tabs.add_tab(bibliotheque::category_description(bibliotheque::conftest, true), tab_options_test);
    tabs.add_tab(bibliotheque::category_description(bibliotheque::confdiff, true), tab_options_compare);
    tabs.add_tab(bibliotheque::category_description(bibliotheque::confisolate, true), tab_options_isolate);
    tabs.add_tab(bibliotheque::category_description(bibliotheque::confmerge, true), tab_options_merge);
    tabs.add_tab(bibliotheque::category_description(bibliotheque::confrepair, true), tab_options_repair);
    tabs.add_tab(bibliotheque::category_description(bibliotheque::confrest, true), tab_options_extract);

	//  global adoption tree

    statusbar.adopt(&saved_status);
    statusbar.adopt(&unsaved_status);
    adopt(&statusbar);

    tabs.adopt_in_section(tab_main, &intro);
    tabs.adopt_in_section(tab_main, &top_fs);
    tabs.adopt_in_section(tab_main, &bot_fs);
    tabs.adopt_in_section(tab_main, &down_fs);
    tabs.adopt_in_section(tab_main, &clear_fs);
    tabs.adopt_in_section(tab_main, &nok_message);

    tabs.adopt_in_section(tab_repo, ab_entrepot.get());
    tabs.adopt_in_section(tab_filemask, ab_filemask.get());
    tabs.adopt_in_section(tab_pathmask, ab_pathmask.get());
    tabs.adopt_in_section(tab_compression, ab_compr.get());
    tabs.adopt_in_section(tab_slicing, ab_slicing.get());
    tabs.adopt_in_section(tab_ciphering, ab_ciphering.get());
    tabs.adopt_in_section(tab_delta_sig, ab_delta_sig.get());
    tabs.adopt_in_section(tab_over_policy, ab_over_policy.get());
    tabs.adopt_in_section(tab_options_test, ab_options_test.get());
    tabs.adopt_in_section(tab_options_compare, ab_options_compare.get());
    tabs.adopt_in_section(tab_options_read, ab_options_read.get());
    tabs.adopt_in_section(tab_options_create, ab_options_create.get());
    tabs.adopt_in_section(tab_options_isolate, ab_options_isolate.get());
    tabs.adopt_in_section(tab_options_merge, ab_options_merge.get());
    tabs.adopt_in_section(tab_options_repair, ab_options_repair.get());
    tabs.adopt_in_section(tab_options_extract, ab_options_extract.get());

    adopt(&tabs);

	// the popup question box
    adopt(&question);

	// actors and events

    register_name(event_download);

    save.record_actor_on_event(this, event_save);
    load.record_actor_on_event(this, event_load);
    autosave.record_actor_on_event(this, html_form_input::changed);
    download.record_actor_on_event(this, event_download);
    upload_form.record_actor_on_event(this, html_form::changed);
    clear_conf.record_actor_on_event(this, event_clear);
    generate_defaults.record_actor_on_event(this, event_defaults);
    question.record_actor_on_event(this, html_yes_no_box::answer_yes);
    question.record_actor_on_event(this, html_yes_no_box::answer_no);

	// visibility
    clear_ok_messages();
    set_saved_status();

	// css
    webdar_css_style::normal_button(load);
    webdar_css_style::normal_button(save);
    webdar_css_style::normal_button(download);
    webdar_css_style::normal_button(clear_conf);
    webdar_css_style::normal_button(generate_defaults);

    load.add_css_class(webdar_css_style::float_right);
    save.add_css_class(webdar_css_style::float_right);
    download.add_css_class(webdar_css_style::float_right);
    clear_conf.add_css_class(webdar_css_style::float_right);
    generate_defaults.add_css_class(webdar_css_style::float_right);

    upload_form.add_button_css_class(webdar_css_style::btn_off);
    upload_form.add_button_css_class(webdar_css_style::url_normal);
    upload_form.add_button_css_class(webdar_css_style::width_8em);
    upload_form.add_button_css_class(webdar_css_style::float_right);

    ok_loaded.add_css_class(webdar_css_style::text_color_green);
    ok_saved.add_css_class(webdar_css_style::text_color_green);
    ok_uploaded.add_css_class(webdar_css_style::text_color_green);
    ok_cleared.add_css_class(webdar_css_style::text_color_green);
    nok_message.add_css_class(webdar_css_style::text_color_red);
    saved_status.add_css_class(webdar_css_style::text_color_green);
    unsaved_status.add_css_class(webdar_css_style::text_color_red);

    statusbar.add_css_class(css_statusbar);

	// loading the default configuration if present

    try
    {
	if(webdar_tools_exists_and_is_file(bib_path, true))
	    on_event(event_load);
	else
	    set_default_configs();
    }
    catch(exception_bug & e)
    {
	throw;
    }
    catch(exception_base & e)
    {
	nok_message.clear();
	nok_message.add_text(0, libdar::tools_printf("Error while loading the configuration file %s: %s",
						     bib_path.c_str(),
						     e.get_message().c_str()));
	nok_message.set_visible(true);
    }

	// tooltips

    filename.set_tooltip("Filename where the configuration has been read from and where it will be saved to. The configuration saved includes all tabs of the present page and only them, nothing is saved from the \"Create\" menu for example");
    autosave.set_tooltip("If this box is unchecked, any configuration change is just kept in memory, until you click on the \"save\" button just below, at which time the configuration as seen from all tabs is saved to the file which name is provided above. A status message shows at the top of the page telling whether the current configuration in memory has been saved to file or not. If checked, this operation is performed automatically upon change and the status message on top page does not show.");
    upload_file.set_tooltip("First select the configuration file your browser will upload to Webdar, then click the \"upload\" button");
}

void html_bibliotheque::on_event(const string & event_name)
{
    if(!biblio)
	throw WEBDAR_BUG;

    if(event_name == event_save)
    {
	clear_ok_messages();
	string config = biblio->save_json().dump();
	ofstream output(filename.get_value());
	if(output)
	{
	    output.write(config.c_str(), config.size());
	    if(!output)
		throw exception_system(libdar::tools_printf("Failed writing configuration to %s", filename.get_value().c_str()), errno);
	    output.close();
	}
	else
	    throw exception_system(libdar::tools_printf("Failed openning %s", filename.get_value().c_str()), errno);
	ok_saved.set_visible(true);
    }
    else if(event_name == event_load)
    {
	if(q_context != context_undefined)
	    throw WEBDAR_BUG;

	q_context = context_load_conf;
	if(! biblio->is_empty())
	    question.ask_question("Loading a configuration from a file will erase all already loaded configurations to memory, do you confirm this action?", false);
	else
	    on_event(html_yes_no_box::answer_yes);
    }
    else if(event_name == html_form_input::changed)
    {
	bool target_autosave = autosave.get_value_as_bool();
	biblio->set_autosave_status(target_autosave);
    }
    else if(event_name == html_form::changed)
    {
	clear_ok_messages();
	expect_upload = true;
    }
    else if(event_name == event_download)
    {
	clear_ok_messages();
	act(event_download);
    }
    else if(event_name == event_clear)
    {
	if(q_context != context_undefined)
	    throw WEBDAR_BUG;

	q_context = context_clear_conf;
	if(! biblio->is_empty())
	    question.ask_question("Warning, do you really wan to delete all configurations not saved to file?", false);
	else
	    on_event(html_yes_no_box::answer_yes);
    }
    else if(event_name == event_defaults)
    {
	clear_ok_messages();
	set_default_configs();
    }
    else if(event_name == html_yes_no_box::answer_yes)
    {
	switch(q_context)
	{
	case context_clear_conf:
	    clear_ok_messages();
	    if(! biblio)
		throw WEBDAR_BUG;
	    else
	    {
		biblio->clear();
		autosave.set_value_as_bool(biblio->get_autosave_status());
	    }
	    ok_cleared.set_visible(true);
	    break;
	case context_load_conf:
	    if(true) // used to declare the input ifstream variable inside a switch/case structure
	    {
		clear_ok_messages();
		ifstream input(filename.get_value());
		if(input)
		{
		    json config = json::parse(input);
		    if(!input)
			throw exception_system(libdar::tools_printf("Failed reading configuration from %s", filename.get_value().c_str()), errno);
		    input.close();
		    biblio->load_json(config);
		    autosave.set_value_as_bool(biblio->get_autosave_status());
		}
		else
		    throw exception_system(libdar::tools_printf("Failed openning %s", filename.get_value().c_str()), errno);
		ok_loaded.set_visible(true);
	    }
	    break;
	case context_upload_conf:

	    break;
	default:
	    throw WEBDAR_BUG;
	}

	q_context = context_undefined;
    }
    else if(event_name == html_yes_no_box::answer_no)
    {
	q_context = context_undefined;
    }
    else
	throw WEBDAR_BUG;
}

string html_bibliotheque::inherited_get_body_part(const chemin & path,
						  const request & req)
{
    string ret;

    expect_upload = false;
    ret = get_body_part_from_all_children(path, req);

    if(expect_upload)
    {
	try
	{
	    unsigned int num = req.get_multipart_number();

	    if(num >= 1) // we look only for the first part as a json configuration
	    {
		try
		{
		    troncon tronc = req.get_body_of_multipart(0);
		    istringstream str(string(tronc.begin, tronc.end));
		    json data = json::parse(str);

		    biblio->load_json(data);
		    autosave.set_value_as_bool(biblio->get_autosave_status());
		    ok_uploaded.set_visible(true);
		}
		catch(json::exception & e)
		{
		    throw exception_json("Error reading configuration from json data", e);
		}
	    }
	    else
		throw exception_input("Missing uploaded configuration in http request", STATUS_CODE_EXPECTATION_FAILED);
	}
	catch(...)
	{
	    expect_upload = false;
	    throw;
	}
	expect_upload = false;
    }

    set_saved_status();

    return ret;
}

void html_bibliotheque::new_css_library_available()
{
    css tmp;
    unique_ptr<css_library> & csslib = lookup_css_library();

    if(!csslib)
	throw WEBDAR_BUG;

    if(!csslib->class_exists(css_statusbar))
    {
	tmp.clear();
	tmp.css_text_h_align(css::al_center);
	tmp.css_margin("0px");
	tmp.css_padding_top("0.1em");
	tmp.css_padding_bottom("0.5em");
	tmp.css_box_sizing(css::bx_border);
	tmp.css_width("100%", false);
	tmp.css_background_color(COLOR_TOPBAR_BACK);
	tmp.css_font_weight_bold();
	csslib->add(css_statusbar, tmp);
    }

    webdar_css_style::update_library(*csslib);
}


void html_bibliotheque::set_saved_status()
{
    if(!biblio)
	throw WEBDAR_BUG;

    if(!autosave.get_value_as_bool())
    {
	statusbar.set_visible(true);
	saved_status.set_visible(biblio->get_saved_status());
	unsaved_status.set_visible(! biblio->get_saved_status());
    }
    else
    {
	statusbar.set_visible(false);
	if(! biblio->get_saved_status())
	    on_event(event_save); // auto saving the bibliotheque
    }
}


void html_bibliotheque::clear_ok_messages()
{
    ok_loaded.set_visible(false);
    ok_saved.set_visible(false);
    ok_uploaded.set_visible(false);
    ok_cleared.set_visible(false);
    nok_message.set_visible(false);
}

void html_bibliotheque::set_default_configs()
{
    unique_ptr<html_entrepot_landing> entrep;
    unique_ptr<html_compression_params> compr_params;
    unique_ptr<html_slicing> slicing;
    unique_ptr<html_ciphering> ciphering;
    unique_ptr<html_form_overwrite_action> overwrite;
    unique_ptr<html_options_read> read_default;
    unique_ptr<html_options_create> create_default;
    unique_ptr<html_options_test> test_default;
    unique_ptr<html_options_compare> compare_default;
    unique_ptr<html_options_isolate> isolate_default;
    unique_ptr<html_options_merge> merge_default;
    unique_ptr<html_options_repair> repair_default;
    unique_ptr<html_options_extract> extract_default;

    entrep.reset(new (nothrow) html_entrepot_landing());
    compr_params.reset(new (nothrow) html_compression_params(true, true, false));
    slicing.reset(new (nothrow) html_slicing());
    ciphering.reset(new (nothrow) html_ciphering());
    overwrite.reset(new (nothrow) html_form_overwrite_action(""));
    read_default.reset(new (nothrow) html_options_read());
    create_default.reset(new (nothrow) html_options_create());
    test_default.reset(new (nothrow) html_options_test());
    compare_default.reset(new (nothrow) html_options_compare());
    isolate_default.reset(new (nothrow) html_options_isolate());
    merge_default.reset(new (nothrow) html_options_merge());
    repair_default.reset(new (nothrow) html_options_repair());
    extract_default.reset(new (nothrow) html_options_extract());

    if(!entrep || ! compr_params || ! slicing || ! ciphering
       || ! overwrite || ! read_default || ! create_default
       || ! test_default || ! isolate_default || ! merge_default
       || ! repair_default || ! extract_default)
	throw exception_memory();

    if(!biblio)
	throw WEBDAR_BUG;

	// set biblio() methods must be invoked now for the object to be informed upon new configuration addition to the bibliotheque
    read_default->set_biblio(biblio);
    create_default->set_biblio(biblio);
    test_default->set_biblio(biblio);
    compare_default->set_biblio(biblio);
    isolate_default->set_biblio(biblio);
    merge_default->set_biblio(biblio);
    repair_default->set_biblio(biblio);
    extract_default->set_biblio(biblio);

	// adding no-compress filemask configuration
    set_default_configs_no_compress();

	// adding diff to full overwriting policy
    set_full_from_diff();

	// adding default entrepot
    if(!biblio->has_config(bibliotheque::repo, bibliotheque::default_config_name))
    {
	entrep->set_to_webdar_defaults();
	biblio->add_config(bibliotheque::repo,
			   bibliotheque::default_config_name,
			   entrep->save_json(),
			   bibliotheque::using_set()); // html_entrept not inheriting from bibliotheque_subconfig class
    }

	// adding default compression params
    if(!biblio->has_config(bibliotheque::compress, bibliotheque::default_config_name))
    {
	compr_params->set_to_webdar_defaults();
	biblio->add_config(bibliotheque::compress,
			   bibliotheque::default_config_name,
			   compr_params->save_json(),
			   bibliotheque::using_set()); // html_compression_params not inheriting from bibliotheque_subconfig class
    }

	// adding default slicing
    if(!biblio->has_config(bibliotheque::slicing, bibliotheque::default_config_name))
    {
	slicing->set_to_webdar_defaults();
	biblio->add_config(bibliotheque::slicing,
			   bibliotheque::default_config_name,
			   slicing->save_json(),
			   bibliotheque::using_set()); // html_slicing not inheriting from bibliotheque_subconfig class
    }

	// adding default ciphering
    if(!biblio->has_config(bibliotheque::ciphering, bibliotheque::default_config_name))
    {
	ciphering->set_to_webdar_defaults();
	biblio->add_config(bibliotheque::ciphering,
			   bibliotheque::default_config_name,
			   ciphering->save_json(),
			   bibliotheque::using_set()); // html_slicing not inheriting from bibliotheque_subconfig class
    }

	// adding default overwriting policy
    if(!biblio->has_config(bibliotheque::over_policy, bibliotheque::default_config_name))
    {
	overwrite->set_to_webdar_defaults();
	biblio->add_config(bibliotheque::over_policy,
			   bibliotheque::default_config_name,
			   overwrite->save_json(),
			   bibliotheque::using_set()); // html_form_overwrite_action not inheriting from bibliotheque_subconfig class
    }

	// adding default read options
    if(!biblio->has_config(bibliotheque::confread, bibliotheque::default_config_name))
    {
	read_default->set_to_webdar_defaults();
	biblio->add_config(bibliotheque::confread,
			   bibliotheque::default_config_name,
			   read_default->save_json(),
			   read_default->get_using_set());
    }

	// adding default create options
    if(!biblio->has_config(bibliotheque::confsave, bibliotheque::default_config_name))
    {
	create_default->set_to_webdar_defaults();
	biblio->add_config(bibliotheque::confsave,
			   bibliotheque::default_config_name,
			   create_default->save_json(),
			   create_default->get_using_set());
    }

	// adding default test options
    if(!biblio->has_config(bibliotheque::conftest, bibliotheque::default_config_name))
    {
	test_default->set_to_webdar_defaults();
	biblio->add_config(bibliotheque::conftest,
			   bibliotheque::default_config_name,
			   test_default->save_json(),
			   test_default->get_using_set());
    }

	// adding default compare options
    if(!biblio->has_config(bibliotheque::confdiff, bibliotheque::default_config_name))
    {
	compare_default->set_to_webdar_defaults();
	biblio->add_config(bibliotheque::confdiff,
			   bibliotheque::default_config_name,
			   compare_default->save_json(),
			   compare_default->get_using_set());
    }

	// adding default isolate options
    if(!biblio->has_config(bibliotheque::confisolate, bibliotheque::default_config_name))
    {
	isolate_default->set_to_webdar_defaults();
	biblio->add_config(bibliotheque::confisolate,
			   bibliotheque::default_config_name,
			   isolate_default->save_json(),
			   isolate_default->get_using_set());
    }

	// adding default merge options
    if(!biblio->has_config(bibliotheque::confmerge, bibliotheque::default_config_name))
    {
	merge_default->set_to_webdar_defaults();
	biblio->add_config(bibliotheque::confmerge,
			   bibliotheque::default_config_name,
			   merge_default->save_json(),
			   merge_default->get_using_set());
    }

	// adding default repair options
    if(!biblio->has_config(bibliotheque::confrepair, bibliotheque::default_config_name))
    {
	repair_default->set_to_webdar_defaults();
	biblio->add_config(bibliotheque::confrepair,
			   bibliotheque::default_config_name,
			   repair_default->save_json(),
			   repair_default->get_using_set());
    }

	// adding default extract options
    if(!biblio->has_config(bibliotheque::confrest, bibliotheque::default_config_name))
    {
	extract_default->set_to_webdar_defaults();
	biblio->add_config(bibliotheque::confrest,
			   bibliotheque::default_config_name,
			   extract_default->save_json(),
			   extract_default->get_using_set());
    }
}

void html_bibliotheque::set_default_configs_no_compress()
{
    static const char* subject = bibliotheque::default_no_compression;
    html_mask_form_filename no_compress(subject);
    bibliotheque::using_set depends; // empty set by default, as we don not have any dependency
    deque<string> labels = no_compress.get_labels();
    shared_ptr<body_builder> shptr;
    html_form_mask_expression* ptr;

    if(!biblio)
	throw WEBDAR_BUG;
    if(biblio->has_config(bibliotheque::filefilter, subject))
	return; // already created or creatged and modified...

    if(labels.empty())
	throw WEBDAR_BUG;
    if(labels[0].find(subject) == string::npos)
	throw WEBDAR_BUG;

    no_compress.manually_set_bool_mode(html_form_mask_bool::and_op);

    deque<string> no_compress_glob_expressions;

	// filling no_compress_glob_expressions from this
	/// included file, generated from ../data/darrc file
#include "no_compress_glob_expression_list.cpp"

    for(deque<string>::const_iterator it = no_compress_glob_expressions.begin();
	it != no_compress_glob_expressions.end();
	++it)
    {
	shptr = no_compress.manually_add_object(labels[0]);
	if(!shptr)
	    throw WEBDAR_BUG;
	ptr = dynamic_cast<html_form_mask_expression*>(shptr.get());
	if(ptr == nullptr)
	    throw WEBDAR_BUG;

	ptr->manually_set_mask_type(html_form_mask_expression::type_glob);
	ptr->manually_set_negate(true);
	ptr->manually_set_casesensitivity(false);
	ptr->manually_set_expression(*it);
    }

    biblio->add_config(bibliotheque::filefilter,
		       subject,
		       no_compress.save_json(),
		       depends);
}

void html_bibliotheque::set_full_from_diff()
{
    if(!biblio)
	throw WEBDAR_BUG;
    if(biblio->has_config(bibliotheque::over_policy, bibliotheque::default_full_from_diff))
	return; // already created or creatged and modified...

    html_form_overwrite_action full_from_diff("");

    full_from_diff.set_action_type(html_form_overwrite_action::action_type_chain);

    html_form_overwrite_chain_action & chain = full_from_diff.get_action_when_type_chain();

	////////
	// setting the first entry of the chain (for Data)
	//

    chain.add_new_entry();
    html_form_overwrite_action & entry1 = chain.get_last_added();

    entry1.set_action_type(html_form_overwrite_action::action_type_condition);
    html_form_overwrite_conditional_action & cond1 = entry1.get_action_when_type_condition();

    html_form_overwrite_combining_criterium & crit = cond1.get_access_to_condition();
    html_form_overwrite_action & if_true = cond1.get_access_to_action_true();
    html_form_overwrite_action & if_false = cond1.get_access_to_action_false();

    html_form_overwrite_base_criterium & base_crit = crit.add_base_criterium();
    base_crit.set_mode(html_form_overwrite_base_criterium::crit_in_place_is_inode);
    base_crit.set_invert(true);
    base_crit.set_negate(true);

    if_true.set_action_type(html_form_overwrite_action::action_type_const);
    html_form_overwrite_constant_action & if_true_act = if_true.get_action_when_type_const();
    if_true_act.set_data_action(html_form_overwrite_constant_action::data_remove);
    if_true_act.set_ea_action(html_form_overwrite_constant_action::ea_clear);

    if_false.set_action_type(html_form_overwrite_action::action_type_condition);
    html_form_overwrite_conditional_action & if_false_act = if_false.get_action_when_type_condition();

    html_form_overwrite_combining_criterium & sub_crit = if_false_act.get_access_to_condition();
    html_form_overwrite_action & sub_if_true = if_false_act.get_access_to_action_true();
    html_form_overwrite_action & sub_if_false = if_false_act.get_access_to_action_false();

    html_form_overwrite_base_criterium & sub_base_crit = sub_crit.add_base_criterium();
    sub_base_crit.set_mode(html_form_overwrite_base_criterium::crit_in_place_data_saved);
    sub_base_crit.set_invert(true);
    sub_base_crit.set_negate(false);

    sub_if_true.set_action_type(html_form_overwrite_action::action_type_const);
    html_form_overwrite_constant_action & sub_if_true_act = sub_if_true.get_action_when_type_const();
    sub_if_true_act.set_data_action(html_form_overwrite_constant_action::data_overwrite);
    sub_if_true_act.set_ea_action(html_form_overwrite_constant_action::ea_undefined);

    sub_if_false.set_action_type(html_form_overwrite_action::action_type_const);
    html_form_overwrite_constant_action & sub_if_false_act = sub_if_false.get_action_when_type_const();
    sub_if_false_act.set_data_action(html_form_overwrite_constant_action::data_preserve);
    sub_if_false_act.set_ea_action(html_form_overwrite_constant_action::ea_undefined);


	////////
        // setting the second entry of the chain (for EA)
    chain.add_new_entry();
    html_form_overwrite_action & entry2 = chain.get_last_added();

    entry2.set_action_type(html_form_overwrite_action::action_type_condition);
    html_form_overwrite_conditional_action & cond2 = entry2.get_action_when_type_condition();

    html_form_overwrite_combining_criterium & crit2 = cond2.get_access_to_condition();
    html_form_overwrite_action & if_true2 = cond2.get_access_to_action_true();
    html_form_overwrite_action & if_false2 = cond2.get_access_to_action_false();

    html_form_overwrite_base_criterium & base_crit2 = crit2.add_base_criterium();
    base_crit2.set_mode(html_form_overwrite_base_criterium::crit_in_place_EA_saved);
    base_crit2.set_invert(true);
    base_crit2.set_negate(false);

    if_true2.set_action_type(html_form_overwrite_action::action_type_const);
    html_form_overwrite_constant_action & if_true2_act = if_true2.get_action_when_type_const();
    if_true2_act.set_data_action(html_form_overwrite_constant_action::data_undefined);
    if_true2_act.set_ea_action(html_form_overwrite_constant_action::ea_overwrite);

    if_false2.set_action_type(html_form_overwrite_action::action_type_const);
    html_form_overwrite_constant_action & if_false2_act = if_false2.get_action_when_type_const();
    if_false2_act.set_data_action(html_form_overwrite_constant_action::data_undefined);
    if_false2_act.set_ea_action(html_form_overwrite_constant_action::ea_preserve);

    biblio->add_config(bibliotheque::over_policy,
		       bibliotheque::default_full_from_diff,
		       full_from_diff.save_json(),
		       bibliotheque::using_set());
}

