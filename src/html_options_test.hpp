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

#ifndef HTML_OPTIONS_TEST_HPP
#define HTML_OPTIONS_TEST_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "actor.hpp"
#include "html_form.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_input.hpp"
#include "html_derouleur.hpp"
#include "html_mask_form_filename.hpp"
#include "html_mask_form_path.hpp"
#include "guichet.hpp"
#include "jsoner.hpp"
#include "bibliotheque_subconfig.hpp"
#include "events.hpp"

    /// html component for the user to provide parameters of libdar archive testing operation

class html_options_test : public body_builder,
			  public actor,
			  public jsoner,
			  public bibliotheque_subconfig,
			  public events
{
public:
    static const std::string changed;

    html_options_test();
    html_options_test(const html_options_test & ref) = delete;
    html_options_test(html_options_test && ref) noexcept = delete;
    html_options_test & operator = (const html_options_test & ref) = delete;
    html_options_test & operator = (html_options_test && ref) noexcept = delete;
    ~html_options_test() = default;

	/// mandatory call to invoke ASAP after constructor
    void set_biblio(const std::shared_ptr<bibliotheque> & ptr);

	/// extract component info as an option set for libdar
    libdar::archive_options_test get_options() const;

	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override;

	/// inherited from biblioteque_subconfig
    virtual bibliotheque::using_set get_using_set() const override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// set the object to webdar defaults
    void set_to_webdar_defaults();

protected:

	// inherited from bdy_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	// inherited from bdy_builder
    virtual void new_css_library_available() override;

private:
    html_derouleur deroule;

    html_form form;
    html_form_fieldset fs;
    html_form_input empty;

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

    bool ignore_events;

    void reset_non_pointer_fields();
    void trigger_change();

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_options_test";

    static constexpr const char* jlabel_dry_run = "dry-run";
    static constexpr const char* jlabel_info_details = "info-details";
    static constexpr const char* jlabel_disp_treated = "display-treated";
    static constexpr const char* jlabel_disp_only_dir = "display-only-dirs";
    static constexpr const char* jlabel_disp_skipped = "display-skipped";
    static constexpr const char* jlabel_file_mask = "file-mask";
    static constexpr const char* jlabel_path_mask = "path-mask";

};

#endif
