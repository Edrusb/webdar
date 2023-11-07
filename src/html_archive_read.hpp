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

#ifndef HTML_ARCHIVE_READ_HPP
#define HTML_ARCHIVE_READ_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"
#include "html_form.hpp"
#include "html_form_input.hpp"
#include "html_form_fieldset.hpp"
#include "html_options_read.hpp"
#include "events.hpp"

    /// class html_archive_read let user define the archive path, basename and option to read

    /// it is mainly used in class saisie

class html_archive_read: public body_builder, public actor
{
public:
    html_archive_read(const std::string & archive_description);
    html_archive_read(const html_archive_read & ref) = delete;
    html_archive_read(html_archive_read && ref) noexcept = delete;
    html_archive_read & operator = (const html_archive_read & ref) = delete;
    html_archive_read & operator = (html_archive_read && ref) noexcept = delete;
    ~html_archive_read() = default;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	// available fields for libdar

    const std::string & get_archive_path() const { return arch_path.get_value(); };
    const std::string & get_archive_basename() const { return archive.get_value(); };
    const libdar::archive_options_read & get_read_options() const { return opt_read.get_options(); };

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;


private:
    html_form form;
    html_form_fieldset fs;
    html_form_input arch_path;
    html_form_input archive;
    html_form_input show_read_options;
    html_options_read opt_read;
};

#endif
