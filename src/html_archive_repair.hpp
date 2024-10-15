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

#ifndef HTML_ARCHIVE_REPAIR_HPP
#define HTML_ARCHIVE_REPAIR_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "body_builder.hpp"
#include "html_form_input_file.hpp"
#include "html_form_fieldset.hpp"
#include "html_derouleur.hpp"
#include "html_options_repair.hpp"

    /// html component used to collect repairing operation parameters from the user

class html_archive_repair: public body_builder
{
public:
    html_archive_repair();
    html_archive_repair(const html_archive_repair & ref) = delete;
    html_archive_repair(html_archive_repair && ref) noexcept = delete;
    html_archive_repair & operator = (const html_archive_repair & ref) = delete;
    html_archive_repair & operator = (html_archive_repair && ref) noexcept = delete;
    ~html_archive_repair() = default;

    const std::string & get_archive_path() const { return repair_dest.get_value(); };
    const std::string & get_archive_basename() const { return basename.get_value(); };
    libdar::archive_options_repair get_options(std::shared_ptr<html_web_user_interaction> dialog) const { return opt_repair.get_options(dialog); };

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

    virtual void new_css_library_available() override;

private:
    html_derouleur deroule;
    html_form_input_file repair_dest;
    html_form_input basename;
    html_form_fieldset repair_fs;
    html_form repair_form;
    html_options_repair opt_repair;
};

#endif
