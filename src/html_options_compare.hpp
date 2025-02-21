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
#include "jsoner.hpp"
#include "events.hpp"

    /// html component providing a way for the user to set the parameter of libdar comparison operation

class html_options_compare : public body_builder,
			     public actor,
			     public jsoner,
			     public events,
			     public bibliotheque_subconfig
{
public:
    static const std::string changed;

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

	/// gathering informations for libdar
    libdar::archive_options_diff get_options() const;

	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override;

	/// inherited from bibliotheque_subconfig
    virtual bibliotheque::using_set get_using_set() const override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// set the object to webdar defaults
    void set_to_webdar_defaults();

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

    bool ignore_events;
    void trigger_change();

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_options_compare";

    static constexpr const char* jlabel_alter_atime = "alter-atime";
    static constexpr const char* jlabel_furtive_read = "furtive-read";
    static constexpr const char* jlabel_zeroing_neg_dates = "zeroing-negative-dates";
    static constexpr const char* jlabel_hourshift = "hourshift";
    static constexpr const char* jlabel_in_place = "use-in-place-root";
    static constexpr const char* jlabel_what_to_check = "what-to-check-for";
    static constexpr const char* jlabel_symlink_date = "compare-symlink-mtime";
    static constexpr const char* jlabel_info_details = "info-details";
    static constexpr const char* jlabel_disp_treated = "display-treated";
    static constexpr const char* jlabel_disp_only_dir = "display-only-dirs";
    static constexpr const char* jlabel_disp_skipped = "display-skipped";
    static constexpr const char* jlabel_file_mask = "file-mask";
    static constexpr const char* jlabel_path_mask = "path-mask";
    static constexpr const char* jlabel_ea_mask = "ea-mask";
    static constexpr const char* jlabel_fsa_scope = "fsa-scope";
};


#endif
