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
#include <fstream>
#include <dar/tools.hpp>

    // webdar headers
#include "environment.hpp"
#include "tokens.hpp"

    //
#include "html_bibliotheque.hpp"

using namespace std;

const string html_bibliotheque::event_download = "html_bibl_download";


html_bibliotheque::html_bibliotheque(std::shared_ptr<bibliotheque> & ptr,
				     const string & bib_path):
    top_fs(""),
    form("Update"),
    filename_fs(""),
    filename("Filename", bib_path, "50", "Select file used to store configurations"),
    autosave("Auto-saving to file", html_form_input::check, "x", "1"),
    save("Save", event_save),
    load("Load", event_load),
    bot_fs(""),
    upload_form("Upload"),
    upload_file("Webdar configuration to upload", html_form_input::file, "", "50"),
    down_fs(""),
    download("Download", event_download),
    clear_fs(""),
    clear_conf("Clear all configurations", event_clear),
    expect_upload(false)
{
    unique_ptr<html_entrepot> tmp_e;
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
    intro.add_text(0, "download and upload the whole configurations defined in all the following tabs.");
    intro.add_paragraph();
    intro.add_text(0, "Note: If saving to file or loading from file should usually be fine in term of security (this is saved locally, on the host where webdar is running),");
    intro.add_text(0, "this is not true while downloading or uploading over a non-secured session (not secured for http sessions, but OK for https session):");
    intro.add_text(0, "All configuration information, including login and passwords used to cipher archive and backups or to connect to remote repositories, if you have enterred such type of information");
    intro.add_text(0, "in the followin tabs, would be transmitted in clear and accessible to anyone");
    intro.add_text(0, "having access to the underneath network used between WebDar and your browser.");
    intro.add_paragraph();
    intro.add_text(0, "This security warning also applies when using untrusted SSL certificates for WebDar (risk of man-in-the-middle attack).");

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
    clear_fs.adopt(&ok_cleared);

	// entrepot tab

    tmp_e.reset(new (nothrow) html_entrepot());
    if(! tmp_e)
	throw exception_memory();

    ab_entrepot.reset(new (nothrow) arriere_boutique<html_entrepot>(ptr,
								    bibliotheque::repo,
								    tmp_e,
								    html_entrepot::changed,
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

    	// global component setups

    tabs.add_tab("Main", tab_main);
    tabs.add_tab("Repos", tab_repo);
    tabs.add_tab("Filename Masks", tab_filemask);
    tabs.add_tab("Path Masks", tab_pathmask);
    tabs.add_tab("Compression", tab_compression);
    tabs.add_tab("Slicing", tab_slicing);
    tabs.add_tab("Ciphering", tab_ciphering);
    tabs.add_tab("Delta signatures", tab_delta_sig);
    tabs.add_tab("Overwriting Policy", tab_over_policy);
    tabs.add_tab("Testing Options", tab_options_test);
    tabs.add_tab("Comparison Options", tab_options_compare);
    tabs.add_tab("Reading Options", tab_options_read);

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

    adopt(&tabs);

	// actors and events

    register_name(event_download);

    save.record_actor_on_event(this, event_save);
    load.record_actor_on_event(this, event_load);
    autosave.record_actor_on_event(this, html_form_input::changed);
    download.record_actor_on_event(this, event_download);
    upload_form.record_actor_on_event(this, html_form::changed);
    clear_conf.record_actor_on_event(this, event_clear);

	// visibility
    clear_ok_messages();
    set_saved_status();

	// css
    webdar_css_style::normal_button(load);
    webdar_css_style::normal_button(save);
    webdar_css_style::normal_button(download);
    webdar_css_style::normal_button(clear_conf);

    load.add_css_class(css_float);
    save.add_css_class(css_float);
    download.add_css_class(css_float);
    clear_conf.add_css_class(css_float);

    upload_form.add_button_css_class(webdar_css_style::wcs_btn_off);
    upload_form.add_button_css_class(webdar_css_style::wcs_url_normal);
    upload_form.add_button_css_class(webdar_css_style::wcs_8em_width);
    upload_form.add_button_css_class(css_float);

    ok_loaded.add_css_class(css_green_text);
    ok_saved.add_css_class(css_green_text);
    ok_uploaded.add_css_class(css_green_text);
    ok_cleared.add_css_class(css_green_text);
    nok_message.add_css_class(css_red_text);
    saved_status.add_css_class(css_green_text);
    unsaved_status.add_css_class(css_red_text);

    statusbar.add_css_class(css_statusbar);

	// loading the default configuration if present

    try
    {
	if(webdar_tools_exists_and_is_dir(bib_path, true))
	    on_event(event_load);
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
}

void html_bibliotheque::on_event(const std::string & event_name)
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
	clear_ok_messages();
	if(! biblio)
	    throw WEBDAR_BUG;
	else
	{
	    biblio->clear();
	    autosave.set_value_as_bool(biblio->get_autosave_status());
	}
	ok_cleared.set_visible(true);
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
    std::unique_ptr<css_library> & csslib = lookup_css_library();

    if(!csslib)
	throw WEBDAR_BUG;

    if(!csslib->class_exists(css_float))
    {
	tmp.clear();
	tmp.css_float(css::fl_right);
	tmp.css_margin_left("1em", true);
	csslib->add(css_float, tmp);
    }

    if(!csslib->class_exists(css_red_text))
    {
	tmp.clear();
	tmp.css_color(RED);
	tmp.css_font_weight_bold();
	csslib->add(css_red_text, tmp);
    }

    if(!csslib->class_exists(css_green_text))
    {
	tmp.clear();
	tmp.css_color(GREEN);
	tmp.css_font_style_italic();
	csslib->add(css_green_text, tmp);
    }

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
