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
#include "chemin.hpp"


    //
#include "html_archive_read.hpp"

using namespace std;

html_archive_read::html_archive_read(const string & archive_description):
    form("Update"),
    fs(archive_description),
    arch_path("Archive path",
	      "/",
	      50,
	      "Select the backup to read...")
{
    webui.reset(new (nothrow) html_web_user_interaction(10));
    if(!webui)
	throw exception_memory();
    webui->set_visible(false);
    opt_read.set_webui(webui);

	// web components layout
    adopt(webui.get());
    fs.adopt(&arch_path);
    form.adopt(&fs);
    adopt(&form);
    adopt(&opt_read);

	// events and actor
    opt_read.record_actor_on_event(this, html_options_read::entrepot_has_changed);

	// initial values
    arch_path.set_select_mode(html_form_input_file::select_slice);
}

void html_archive_read::on_event(const std::string & event_name)
{
    if(event_name == html_options_read::entrepot_has_changed)
    {
	webui->clear();
	webui->auto_hide(true, true);
	arch_path.set_entrepot(opt_read.get_entrepot(webui));
    }
}

string html_archive_read::inherited_get_body_part(const chemin & path,
						  const request & req)
{
    return get_body_part_from_all_children(path, req);
}


string html_archive_read::get_archive_path() const
{
    chemin chem(arch_path.get_value());

    if(chem.size() >= 1)
    {
	chem.pop_back();
	return chem.display();
    }
    else
	return "";
}

string html_archive_read::get_archive_basename() const
{
    string ret;
    chemin chem(arch_path.get_value());

    if(chem.size() > 0)
	ret = chem.back();
    else
	ret = arch_path.get_value();

    return ret;
}
