/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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

#ifndef HTML_OPTIONS_REPAIR_HPP
#define HTML_OPTIONS_REPAIR_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <memory>

    // webdar headers
#include "body_builder.hpp"
#include "actor.hpp"
#include "events.hpp"
#include "html_form_input.hpp"
#include "html_form_fieldset.hpp"
#include "html_form.hpp"
#include "html_hash_algo.hpp"
#include "html_derouleur.hpp"
#include "html_entrepot_landing.hpp"
#include "html_web_user_interaction.hpp"
#include "html_form_input_unit.hpp"
#include "html_slicing.hpp"
#include "html_ciphering.hpp"
#include "guichet.hpp"
#include "jsoner.hpp"
#include "bibliotheque_subconfig.hpp"

    /// html component used for the user to provide the parameters to repair an archive

class html_options_repair : public body_builder,
			    public actor,
			    public events,
			    public jsoner,
			    public bibliotheque_subconfig
{
public:
    static const std::string landing_path_changed; ///< landing path has changed
    static const std::string entrepot_changed;     ///< event triggered for entrepot only changes except landing_path
    static const std::string changed;              ///< event triggered for any change except landing_path

    html_options_repair();
    html_options_repair(const html_options_repair & ref) = delete;
    html_options_repair(html_options_repair && ref) noexcept = delete;
    html_options_repair & operator = (const html_options_repair & ref) = delete;
    html_options_repair & operator = (html_options_repair && ref) noexcept = delete;
    ~html_options_repair() = default;

	/// mandatory call to invoke ASAP after constructor
    void set_biblio(const std::shared_ptr<bibliotheque> & ptr);

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

	/// generate a corresponding libdar::archive_option_repair object
    libdar::archive_options_repair get_options(std::shared_ptr<html_web_user_interaction> & webui) const;

	/// optain the current entrepot object where is expected to be repaired the archive
    std::shared_ptr<libdar::entrepot> get_entrepot(std::shared_ptr<html_web_user_interaction> webui) const { return entrep->get_entrepot(webui); };

	/// obtain the entrepot landing path
    const std::string & get_landing_path() const { return entrep->get_landing_path(); };

	/// set the object to webdar defaults
    void set_to_webdar_defaults();

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    bool ignore_events;

    html_derouleur deroule;

	// entrepot

    guichet guichet_entrep;
    std::shared_ptr<html_entrepot_landing> entrep;


	// display settings

    html_form_input info_details;
    html_form_input display_treated;
    html_form_input display_only_dir;
    html_form_input display_skipped;
    html_form_input display_dirstats;
    html_form_fieldset display_fs;
    html_form display_form;

	// processing

    html_form_input allow_over;
    html_form_input warn_over;
    html_form_input pause;
    html_form_input execute;
    html_form_input dry_run;
    html_form_input multi_thread_compress;
    html_hash_algo hash_algo;
    html_form_fieldset processing_fs;
    html_form processing_form;

	// target

    html_form_input user_comment;
    html_form_fieldset target_fs;
    html_form target_form;

	// slicing

    guichet guichet_slicing;
    std::shared_ptr<html_slicing> slicing;

	// crypto

    guichet guichet_ciphering;
    std::shared_ptr<html_ciphering> ciphering;

    void init();
    void trigger_changed();

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_options_repair";

    static constexpr const char* jlabel_entrep = "entrepot";
    static constexpr const char* jlabel_info_details = "info-details";
    static constexpr const char* jlabel_display_treated = "display-treated";
    static constexpr const char* jlabel_display_only_dir = "display-only-dirs";
    static constexpr const char* jlabel_display_skipped = "display-skipped";
    static constexpr const char* jlabel_display_dirstats = "display-dirstats";
    static constexpr const char* jlabel_allow_over = "allow-over";
    static constexpr const char* jlabel_warn_over = "warn-over";
    static constexpr const char* jlabel_pause = "pause";
    static constexpr const char* jlabel_execute = "execute";
    static constexpr const char* jlabel_dry_run = "dry-run";
    static constexpr const char* jlabel_multi_thread_compress = "compress-threads";
    static constexpr const char* jlabel_hash_algo = "hash-algo";
    static constexpr const char* jlabel_user_comment = "user-comment";
    static constexpr const char* jlabel_slicing = "slicing";
    static constexpr const char* jlabel_ciphering = "ciphering";

};

#endif
