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
#ifndef HTML_ARCHIVE_EXTRACT_HPP
#define HTML_ARCHIVE_EXTRACT_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "actor.hpp"
#include "html_derouleur.hpp"
#include "html_form_input_file.hpp"
#include "html_options_extract.hpp"
#include "guichet.hpp"

    /// html component used to let the user provide parameters for an isolation operation

class html_archive_extract: public body_builder, public actor
{
public:
    html_archive_extract();
    html_archive_extract(const html_archive_extract & ref) = delete;
    html_archive_extract(html_archive_extract && ref) noexcept = delete;
    html_archive_extract & operator = (const html_archive_extract & ref) = delete;
    html_archive_extract & operator = (html_archive_extract && ref) noexcept = delete;
    ~html_archive_extract() = default;

	/// mandatory call to invoke ASAP after constructor
    void set_biblio(const std::shared_ptr<bibliotheque> & ptr);

    const std::string & get_fs_root() const { return extract_fs_root.get_value(); };

    libdar::archive_options_extract get_options() const { return opt_extract->get_options(); };

            /// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:

        /// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
                                                const request & req) override;

        /// inherited from body_builder
    virtual void new_css_library_available() override;


private:
    html_derouleur extract_params;
    html_form_fieldset extract_fs_root_fs;
    html_form_input_file extract_fs_root;
    html_form extract_fs_root_form;

    guichet guichet_opt_extract;
    std::shared_ptr<html_options_extract> opt_extract;

    static const std::string extract_root_changed;

};

#endif
