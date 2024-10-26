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

#ifndef HTML_OPTIONS_ISOLATE_HPP
#define HTML_OPTIONS_ISOLATE_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "html_form.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_input.hpp"
#include "html_hash_algo.hpp"
#include "html_crypto_algo.hpp"
#include "html_derouleur.hpp"
#include "html_entrepot.hpp"
#include "html_form_gnupg_list.hpp"
#include "html_form_select.hpp"
#include "html_mask_form_filename.hpp"
#include "html_form_sig_block_size.hpp"
#include "html_compression_params.hpp"

    /// html components for the user to provide parameters of a isolation operation

class html_options_isolate : public body_builder, public actor, public events
{
public:
    static const std::string entrepot_changed;

    html_options_isolate();
    html_options_isolate(const html_options_isolate & ref) = delete;
    html_options_isolate(html_options_isolate && ref) noexcept = delete;
    html_options_isolate & operator = (const html_options_isolate & ref) = delete;
    html_options_isolate & operator = (html_options_isolate && ref) noexcept = delete;
    ~html_options_isolate() = default;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;


	/// informations available for libdar

    libdar::archive_options_isolate get_options(std::shared_ptr<html_web_user_interaction> & webui) const;

	/// optain the current entrepot object where is expected to be create the archive
    std::shared_ptr<libdar::entrepot> get_entrepot(std::shared_ptr<html_web_user_interaction> webui) const { return entrep.get_entrepot(webui); };

protected:

	/// inherited from bdy_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

    virtual void new_css_library_available() override;

private:

	// specific events
    static constexpr const char* kdf_algo_changed = "kdf_algo_changed";

    html_derouleur deroule;

    html_form form_delta_sig;
    html_form_fieldset delta_fs;
    html_form_input delta_sig;
    html_form_input delta_transfer_mode;
    html_form_input_unit delta_sig_min_size;
    html_form_sig_block_size sig_block_size;
    html_mask_form_filename delta_mask;

    html_entrepot entrep;
    html_form form_archgen;
    html_form_fieldset fs_archgen;
    html_form_input allow_over;
    html_form_input warn_over;
    html_form_input pause;
    html_form_input slice_permission;
    html_form_input slice_user_ownership;
    html_form_input slice_group_ownership;
    html_form_input sequential_marks;
    html_form_input user_comment;
    html_form_input slice_min_digits;
    html_hash_algo hash_algo;
    html_form_input execute;
    html_form_input empty;


    html_form form_shown;
    html_form_fieldset fs_shown;
    html_form_input info_details;

    html_compression_params compr_params;

    html_form form_slicing;
    html_form_fieldset fs_slicing;
    html_form_input slicing;
    html_form_input_unit slice_size;
    html_form_input different_first_slice;
    html_form_input_unit first_slice_size;

    html_form form_crypto;
    html_form_fieldset fs_crypto;
    html_form_select crypto_type;
    html_crypto_algo crypto_algo;
    html_form_input crypto_pass1;
    html_form_input crypto_pass2;
    html_form_input_unit crypto_size;
    html_form_gnupg_list gnupg;
    html_form_input crypto_threads;
    html_form_fieldset crypto_fs_kdf_hash;
    html_form_radio crypto_kdf_hash;
    html_form_input iteration_count;
};

#endif
