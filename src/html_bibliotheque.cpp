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

    // webdar headers
#include "environment.hpp"

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
    save("Save", event_save),
    load("Load", event_load),
    bot_fs(""),
    upload_form("Upload"),
    upload_file("Webdar configuration to upload", html_form_input::file, "", "50"),
    down_fs(""),
    download("Downlaod", event_download),

    expect_upload(false)
{
    unique_ptr<html_entrepot> tmp;

    if(!ptr)
	throw WEBDAR_BUG;

    biblio = ptr;
    if(! biblio)
	throw WEBDAR_BUG;

    upload_form.set_enctype("multipart/form-data");
    ok_message.add_text(0, "Configuration uploaded successfully!");

	// main tab

    top_fs.adopt(&form);
    form.adopt(&filename_fs);
    filename_fs.adopt(&filename);
    top_fs.adopt(&save);
    top_fs.adopt(&load);

    upload_form.adopt(&upload_file);
    bot_fs.adopt(&upload_form);
    bot_fs.adopt(&ok_message);

    down_fs.adopt(&download);

	// entrepot tab

    tmp.reset(new (nothrow) html_entrepot());
    if(!tmp)
	throw exception_memory();

    ab_entrepot.reset(new (nothrow) arriere_boutique<html_entrepot>(ptr,
								    bibliotheque::repo,
								    tmp));
    if(!ab_entrepot)
	throw exception_memory();


    	// global component setups
    tabs.add_tab(tab_main, tab_main);
    tabs.add_tab(tab_repo, tab_repo);


	//  global adoption tree

    tabs.adopt_in_section(tab_main, &top_fs);
    tabs.adopt_in_section(tab_main, &bot_fs);
    tabs.adopt_in_section(tab_main, &down_fs);

    tabs.adopt_in_section(tab_repo, ab_entrepot.get());
    adopt(&tabs);

	// actors and events

    register_name(event_download);

    save.record_actor_on_event(this, event_save);
    load.record_actor_on_event(this, event_load);
    download.record_actor_on_event(this, event_download);
    upload_form.record_actor_on_event(this, html_form::changed);

	// visibility
    ok_message.set_visible(false);

	// css
    webdar_css_style::normal_button(load);
    webdar_css_style::normal_button(save);

    load.add_css_class(css_float);
    save.add_css_class(css_float);
}

void html_bibliotheque::on_event(const std::string & event_name)
{
    if(!biblio)
	throw WEBDAR_BUG;

    if(event_name == event_save)
    {
	ok_message.set_visible(false);
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
    }
    else if(event_name == event_load)
    {
	ok_message.set_visible(false);
	ifstream input(filename.get_value());
	if(input)
	{
	    json config = json::parse(input);
	    if(!input)
		throw exception_system(libdar::tools_printf("Failed reading configuration from %s", filename.get_value().c_str()), errno);
	    input.close();
	    biblio->load_json(config);
	    if(!ab_entrepot)
		throw WEBDAR_BUG;
	    ab_entrepot->refresh();
	}
	else
	    throw exception_system(libdar::tools_printf("Failed openning %s", filename.get_value().c_str()), errno);
    }
    else if(event_name == html_form::changed)
    {
	ok_message.set_visible(false);
	expect_upload = true;
    }
    else if(event_name == event_download)
    {
	act(event_download);
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
		    ab_entrepot->refresh();
		    ok_message.set_visible(true);
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


    webdar_css_style::update_library(*csslib);
}
