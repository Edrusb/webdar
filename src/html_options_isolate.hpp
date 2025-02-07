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
#include "html_slicing.hpp"
#include "html_ciphering.hpp"
#include "guichet.hpp"
#include "jsoner.hpp"

    /// html components for the user to provide parameters of a isolation operation

class html_options_isolate : public body_builder,
			     public actor,
			     public events,
			     public jsoner
{
public:
    static const std::string entrepot_changed; ///< event only triggered when entrepot related params changed
    static const std::string changed;  ///< triggered when the component changed, including entrepot params changed

    html_options_isolate();
    html_options_isolate(const html_options_isolate & ref) = delete;
    html_options_isolate(html_options_isolate && ref) noexcept = delete;
    html_options_isolate & operator = (const html_options_isolate & ref) = delete;
    html_options_isolate & operator = (html_options_isolate && ref) noexcept = delete;
    ~html_options_isolate() = default;

	/// mandatory call to invoke ASAP after constructor
    void set_biblio(const std::shared_ptr<bibliotheque> & ptr);

	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;


	/// informations available for libdar

    libdar::archive_options_isolate get_options(std::shared_ptr<html_web_user_interaction> & webui) const;

	/// optain the current entrepot object where is expected to be create the archive
    std::shared_ptr<libdar::entrepot> get_entrepot(std::shared_ptr<html_web_user_interaction> webui) const { return entrep->get_entrepot(webui); };

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

    html_text delta_filter_params;

    guichet guichet_sig_block_size;
    std::shared_ptr<html_form_sig_block_size> sig_block_size;

    html_text delta_filter_title;

    guichet guichet_delta_mask;
    std::shared_ptr<html_mask_form_filename> delta_mask;

    guichet guichet_entrep;
    std::shared_ptr<html_entrepot> entrep;

    html_form form_archgen;
    html_form_fieldset fs_archgen;
    html_form_input allow_over;
    html_form_input warn_over;
    html_form_input pause;
    html_form_input sequential_marks;
    html_form_input user_comment;
    html_hash_algo hash_algo;
    html_form_input execute;
    html_form_input empty;


    html_form form_shown;
    html_form_fieldset fs_shown;
    html_form_input info_details;

    guichet guichet_compr_params;
    std::shared_ptr<html_compression_params> compr_params;

    guichet guichet_slicing;
    std::shared_ptr<html_slicing> slicing;

    guichet guichet_ciphering;
    std::shared_ptr<html_ciphering> ciphering;

    bool ignore_events;

    void init();  ///< set parameters to default values
    void trigger_changed();

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_options_isolate";

    static constexpr const char* jlabel_delta_sig = "delta-sig";
    static constexpr const char* jlabel_delta_xfer = "delta-sig-xfer";
    static constexpr const char* jlabel_sig_block_size = "sig-block-size";
    static constexpr const char* jlabel_delta_mask = "delta-mask";
    static constexpr const char* jlabel_entrep = "entrepot";
    static constexpr const char* jlabel_allow_over = "allow-over";
    static constexpr const char* jlabel_warn_over = "warn-over";
    static constexpr const char* jlabel_seq_masks = "seq-masks";
    static constexpr const char* jlabel_user_comment = "user-comment";
    static constexpr const char* jlabel_hash_algo = "hash-algo";
    static constexpr const char* jlabel_execute = "execute";
    static constexpr const char* jlabel_empty = "dry-run";
    static constexpr const char* jlabel_info_details = "info-details";
    static constexpr const char* jlabel_compr_params = "compr-params";
    static constexpr const char* jlabel_slicing = "slicing";
    static constexpr const char* jlabel_ciphering = "ciphering";

};

#endif
