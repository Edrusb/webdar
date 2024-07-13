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

#ifndef HTML_OPTIONS_REPAIR_HPP
#define HTML_OPTIONS_REPAIR_HPP

    // C system header files
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
#include "html_crypto_algo.hpp"
#include "html_form_input.hpp"
#include "html_size_unit.hpp"
#include "html_form_fieldset.hpp"
#include "html_form.hpp"
#include "html_hash_algo.hpp"
#include "html_derouleur.hpp"
#include "html_entrepot.hpp"
#include "html_web_user_interaction.hpp"

    /// html component used for the user to provide the parameters to repair an archive

class html_options_repair : public body_builder, public actor, public events
{
public:
    static const std::string entrepot_changed;

    html_options_repair();
    html_options_repair(const html_options_repair & ref) = delete;
    html_options_repair(html_options_repair && ref) noexcept = delete;
    html_options_repair & operator = (const html_options_repair & ref) = delete;
    html_options_repair & operator = (html_options_repair && ref) noexcept = delete;
    ~html_options_repair() = default;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// generate a corresponding libdar::archive_option_repair object
    libdar::archive_options_repair get_options(std::shared_ptr<html_web_user_interaction> & webui) const;

	/// optain the current entrepot object where is expected to be repaired the archive
    std::shared_ptr<libdar::entrepot> get_entrepot(std::shared_ptr<html_web_user_interaction> webui) const { return entrep.get_entrepot(webui); };

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    html_derouleur deroule;

	// entrepot

    html_entrepot entrep;


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
    html_form_input multi_thread_crypto;
    html_form_input multi_thread_compress;
    html_hash_algo hash_algo;
    html_form_fieldset processing_fs;
    html_form processing_form;

	// target

    html_form_input slice_min_digits;
    html_form_input slice_permission;
    html_form_input slice_user_ownership;
    html_form_input slice_group_ownership;
    html_form_input user_comment;
    html_form_fieldset target_fs;
    html_form target_form;

	// slicing

    html_form_input slicing;
    html_form_input slice_size;
    html_size_unit slice_size_unit;
    html_form_input different_first_slice;
    html_form_input first_slice_size;
    html_size_unit first_slice_size_unit;
    html_form_fieldset slicing_fs;
    html_form slicing_form;

	// crypto

    html_crypto_algo crypto_algo;
    html_form_input crypto_pass1;
    html_form_input crypto_pass2;
    html_form_input crypto_size;
    html_form_fieldset crypto_fs;
    html_form crypto_form;


    static const std::string css_indent;
};

#endif
