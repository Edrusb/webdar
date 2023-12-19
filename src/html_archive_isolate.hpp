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
#ifndef HTML_ARCHIVE_ISOLATE_HPP
#define HTML_ARCHIVE_ISOLATE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_form_input_file.hpp"
#include "html_options_isolate.hpp"
#include "body_builder.hpp"
#include "events.hpp"
#include "html_archive_read.hpp"
#include "html_derouleur.hpp"

class html_archive_isolate: public body_builder, public actor
{
public:
    html_archive_isolate();
    html_archive_isolate(const html_archive_isolate & ref) = delete;
    html_archive_isolate(html_archive_isolate && ref) noexcept = delete;
    html_archive_isolate & operator = (const html_archive_isolate & ref) = delete;
    html_archive_isolate & operator = (html_archive_isolate && ref) noexcept = delete;
    ~html_archive_isolate() = default;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

    const std::string & get_archive_path() const { return sauv_path.get_value(); };
    const std::string & get_archive_basename() const { return basename.get_value(); };
    const html_options_isolate & get_options_isolate() const { return options; };
    const html_archive_read & get_reference() const { return ref; };

protected:

    	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

    virtual void new_css_library_available() override;

private:
    html_derouleur deroule;
    html_form form;
    html_form_fieldset fs;
    html_form_input_file sauv_path;
    html_form_input basename;
    html_archive_read ref;
    html_form_input show_options;
    html_options_isolate options;
};

#endif
