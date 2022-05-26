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
#include "html_archive_merge.hpp"

using namespace std;

html_archive_merge::html_archive_merge():
    form("Update"),
    fs("Resulting archive"),
    sauv_path("Where to create the archive", html_form_input::text, "", 20),
    basename("Archive basename", html_form_input::text, "", 10),
    show_options("Show merging options", html_form_input::check, "", 1),
    reference("Archive of reference")
{
    fs.adopt(&sauv_path);
    fs.adopt(&basename);
    fs.adopt(&show_options);
    form.adopt(&fs);
    adopt(&form);
    adopt(&reference);
    adopt(&options);

    show_options.record_actor_on_event(this, html_form_input::changed);

    on_event("");
}

string html_archive_merge::get_body_part(const chemin & path,
				     const request & req)
{
    return get_body_part_from_all_children(path, req);
}


void html_archive_merge::on_event(const std::string & event_name)
{
    options.set_visible(show_options.get_value_as_bool());
}
