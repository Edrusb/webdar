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

#ifndef HTML_OPTIONS_EXTRACT_HPP
#define HTML_OPTIONS_EXTRACT_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "html_form_select.hpp"
#include "html_form_input.hpp"
#include "html_form.hpp"
#include "html_form_fieldset.hpp"

class html_options_extract : public body_builder
{
public:
    html_options_extract();
    html_options_extract(const html_options_extract & ref) = default;
    html_options_extract(html_options_extract && ref) noexcept = default;
    html_options_extract & operator = (const html_options_extract & ref) = default;
    html_options_extract & operator = (html_options_extract && ref) noexcept = default;
    ~html_options_extract() = default;

    libdar::archive_options_extract get_options() const;

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req) override;
private:
    html_form form;
    html_form_fieldset fs;
	// selection (filter)
	// subtree (filter)
    html_form_input warn_over;
    html_form_input info_details;
	// ea_mask (filter)
    html_form_input flat;
    html_form_select what_to_check;
    html_form_input warn_remove_no_match;
    html_form_input empty;
    html_form_input display_skipped;
    html_form_input empty_dir;
    html_form_select dirty_behavior;
	// overwriting_rules (policy)
    html_form_input only_deleted;
    html_form_input ignore_deleted;
};

#endif
