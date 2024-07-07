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
#include "html_derouleur.hpp"
#include "html_mask_form_filename.hpp"
#include "html_mask_form_path.hpp"

    /// html components used for the user to provide parameters of libdar archive restoration operation

class html_options_extract : public body_builder
{
public:
    html_options_extract();
    html_options_extract(const html_options_extract & ref) = delete;
    html_options_extract(html_options_extract && ref) noexcept = delete;
    html_options_extract & operator = (const html_options_extract & ref) = delete;
    html_options_extract & operator = (html_options_extract && ref) noexcept = delete;
    ~html_options_extract() = default;

	/// needed for path based filtering to filter accordingly to the current root_fs
    void set_fs_root(const std::string & prefix) { path_mask.set_fs_root(prefix); };

    libdar::archive_options_extract get_options() const;

protected:

	// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    html_derouleur deroule;
    html_form form;
    html_form_fieldset fs;
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
    html_mask_form_filename filename_mask;
    html_mask_form_path path_mask;
};

#endif
