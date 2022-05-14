/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2022 Denis Corbin
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



    //
#include "html_archive_read.hpp"

using namespace std;

html_archive_read::html_archive_read(const string & archive_description):
    form("Update"),
    fs(archive_description),
    arch_path("Archive path",
	      html_form_input::text,
	      "",
	      50),
    archive("Archive basename",
	    html_form_input::text,
	    "",
	    30),
    show_read_options("Show archive reading options",
		      html_form_input::check,
		      "",
		      1)
{

	/// web components layout
    form.adopt(&arch_path);
    form.adopt(&archive);
    form.adopt(&show_read_options);
    fs.adopt(&form);
    fs.adopt(&opt_read);
    adopt(&fs);

	/// events
    show_read_options.record_actor_on_event(this, html_form_input::changed);
    on_event(html_form_input::changed); // set the object in a coherent status
}


string html_archive_read::get_body_part(const chemin & path,
				   const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_archive_read::on_event(const std::string & event_name)
{
    if(event_name != html_form_input::changed)
	throw WEBDAR_BUG;

    opt_read.set_visible(show_read_options.get_value_as_bool());
}
