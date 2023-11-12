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
	      "",
	      50,
	      "Select the backup to read..."),
    show_read_options("Show archive reading options",
		      html_form_input::check,
		      "",
		      1)
{

	// web components layout
    adopt(&form);
    form.adopt(&fs);
    fs.adopt(&arch_path);
    fs.adopt(&show_read_options);

    adopt(&opt_read);

	// events
    show_read_options.record_actor_on_event(this, html_form_input::changed);
    on_event(html_form_input::changed); // set the object in a coherent status

	// initial values
    arch_path.set_value("/");
}


string html_archive_read::inherited_get_body_part(const chemin & path,
						  const request & req)
{
    return get_body_part_from_all_children(path, req);
}


string html_archive_read::get_archive_path() const
{
    chemin chem(arch_path.get_value());

    if(chem.size() > 1)
    {
	chem.pop_back();
	return chem.display();
    }
    else
	return "";
}


string html_archive_read::get_archive_basename() const
{
    chemin chem(arch_path.get_value());

    if(chem.size() > 0)
	return chem.back();
    else
	return arch_path.get_value();;
}

void html_archive_read::on_event(const std::string & event_name)
{
    if(event_name != html_form_input::changed)
	throw WEBDAR_BUG;

    opt_read.set_visible(show_read_options.get_value_as_bool());
}
