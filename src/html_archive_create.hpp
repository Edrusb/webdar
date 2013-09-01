/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013 Denis Corbin
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

#ifndef HTML_ARCHIVE_CREATE_HPP
#define HTML_ARCHIVE_CREATE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_form_input.hpp"
#include "html_form_fieldset.hpp"
#include "html_options_create.hpp"
#include "body_builder.hpp"
#include "events.hpp"

class html_archive_create: public body_builder, public actor
{
public:
    html_archive_create();
    html_archive_create(const html_archive_create & ref);
    const html_archive_create & operator = (const html_archive_create & ref) { throw WEBDAR_BUG; };

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

	/// inherited from on_event
    virtual void on_event(const std::string & event_name);

    const std::string & get_archive_path() const { return sauv_path.get_value(); };
    const std::string & get_archive_basename() const { return basename.get_value(); };
    const std::string & get_fs_root() const { return fs_root.get_value(); };
    const html_options_create & get_options_create() const { return options; };

private:
    html_form form;
    html_form_fieldset fs;
    html_form_input fs_root;
    html_form_input sauv_path;
    html_form_input basename;
    html_form_input show_options;
    html_options_create options;

};

#endif
