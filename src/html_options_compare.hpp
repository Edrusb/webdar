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

#ifndef HTML_OPTIONS_COMPARE_HPP
#define HTML_OPTIONS_COMPARE_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"
#include "actor.hpp"
#include "html_form_input.hpp"
#include "html_form_fieldset.hpp"
#include "html_comparison_fields.hpp"
#include "html_form.hpp"
#include "html_derouleur.hpp"
#include "html_mask_form_filename.hpp"
#include "html_mask_form_path.hpp"
#include "html_fsa_scope.hpp"
#include "guichet.hpp"

    /// html component providing a way for the user to set the parameter of libdar comparison operation

class html_options_compare : public body_builder, public actor
{
public:
    html_options_compare();
    html_options_compare(const html_options_compare & ref) = delete;
    html_options_compare(html_options_compare && ref) noexcept = delete;
    html_options_compare & operator = (const html_options_compare & ref) = delete;
    html_options_compare & operator = (html_options_compare && ref) noexcept = delete;
    ~html_options_compare() = default;

	/// mandatory call to invoke ASAP after constructor
    void set_biblio(const std::shared_ptr<bibliotheque> & ptr);

	/// needed for path based filtering to filter accordingly to the current root_fs
    void set_fs_root(const std::string & prefix) { path_mask->set_fs_root(prefix); };

    libdar::archive_options_diff get_options() const;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    static constexpr const char* treated_none = "none";
    static constexpr const char* treated_dir = "dir";
    static constexpr const char* treated_all = "all";

    html_derouleur deroule;

    html_form form_reading;
    html_form_fieldset fs_alter_atime;
    html_form_radio alter_atime;
    html_form_input furtive_read_mode;
    html_form_input zeroing_neg_date;
    html_form_input hourshift;

    html_form form;
    html_form_fieldset fs;
    html_form_input in_place;
    html_comparison_fields what_to_check;
    html_form_input compare_symlink_date;

    html_form form_show;
    html_form_fieldset fs_show;
    html_form_input info_details;
    html_form_input display_treated;
    html_form_input display_treated_only_dir;
    html_form_input display_skipped;

    guichet guichet_filename_mask;
    std::shared_ptr<html_mask_form_filename> filename_mask;

    guichet guichet_path_mask;
    std::shared_ptr<html_mask_form_path> path_mask;

    guichet guichet_ea_mask;
    std::shared_ptr<html_mask_form_filename> ea_mask;

    html_fsa_scope fsa_scope;

};


#endif
