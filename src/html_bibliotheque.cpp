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

html_bibliotheque::html_bibliotheque(std::shared_ptr<bibliotheque> & ptr,
				     const string & bib_path):
    top_fs(""),
    form("Update"),
    filename_fs(""),
    filename("Filename", bib_path, "50", "Select file used to store configurations"),
    save("Save", event_save),
    load("Load", event_load),
    bot_fs(""),
    download("Download", event_down),
    upload("Upload", event_up)
{
    unique_ptr<html_entrepot> tmp;

    if(!ptr)
	throw WEBDAR_BUG;

    biblio = ptr;
    if(! biblio)
	throw WEBDAR_BUG;

	// main tab

    top_fs.adopt(&form);
    form.adopt(&filename_fs);
    filename_fs.adopt(&filename);
    top_fs.adopt(&save);
    top_fs.adopt(&load);

    bot_fs.adopt(&download);
    bot_fs.adopt(&upload);


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

    tabs.adopt_in_section(tab_repo, ab_entrepot.get());
    adopt(&tabs);

	// actors and events

    save.record_actor_on_event(this, event_save);
    load.record_actor_on_event(this, event_load);
    download.record_actor_on_event(this, event_down);
    upload.record_actor_on_event(this, event_up);

	// css
    webdar_css_style::normal_button(load);
    webdar_css_style::normal_button(save);
    webdar_css_style::normal_button(download);
    webdar_css_style::normal_button(upload);

    load.add_css_class(css_float);
    save.add_css_class(css_float);
    download.add_css_class(css_float);
    upload.add_css_class(css_float);
}

void html_bibliotheque::on_event(const std::string & event_name)
{
    if(!biblio)
	throw WEBDAR_BUG;

    if(event_name == event_save)
    {
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
    else if(event_name == event_down)
    {

    }
    else if(event_name == event_up)
    {

    }
    else
	throw WEBDAR_BUG;
}

string html_bibliotheque::inherited_get_body_part(const chemin & path,
						  const request & req)
{
    return get_body_part_from_all_children(path, req);
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
