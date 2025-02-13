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

#ifndef HTML_OPTIONS_MERGE_HPP
#define HTML_OPTIONS_MERGE_HPP

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
#include "events.hpp"
#include "html_form.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_input.hpp"
#include "html_compression.hpp"
#include "html_hash_algo.hpp"
#include "html_crypto_algo.hpp"
#include "html_archive_read.hpp"
#include "html_derouleur.hpp"
#include "html_entrepot.hpp"
#include "html_mask_form_filename.hpp"
#include "html_mask_form_path.hpp"
#include "html_form_overwrite_action.hpp"
#include "html_form_input_unit.hpp"
#include "html_compression_params.hpp"
#include "html_slicing.hpp"
#include "html_ciphering.hpp"
#include "html_form_sig_block_size.hpp"
#include "html_form_select.hpp"
#include "html_fsa_scope.hpp"
#include "guichet.hpp"
#include "jsoner.hpp"
#include "bibliotheque_subconfig.hpp"

    /// html component for the user to provided parameters of a libdar merging operation

class html_options_merge : public body_builder,
			   public actor,
			   public events,
			   public jsoner,
			   public bibliotheque_subconfig
{
public:
    static const std::string landing_path_changed; ///< landing path has changed
    static const std::string entrepot_changed; ///< event triggered only for entrepot related changes except landing_path
    static const std::string changed;   ///< event for any change in this component, including entrpot changes except landing_path

    html_options_merge();
    html_options_merge(const html_options_merge & ref) = delete;
    html_options_merge(html_options_merge && ref) noexcept = delete;
    html_options_merge & operator = (const html_options_merge & ref) = delete;
    html_options_merge & operator = (html_options_merge && ref) noexcept = delete;
    ~html_options_merge() = default;

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

	/// informations available for libdar
	///
	/// \note if a reference is requested it implies openninng an archive
	/// and feeding the libdar::archive_options_merge::set_auxiliary_ref(libdar::archive *)
	/// with the object built from this->get_auxiliary()
    libdar::archive_options_merge get_options(std::shared_ptr<html_web_user_interaction> & webui) const;

	/// optain the current entrepot object where is expected to be create the archive
    std::shared_ptr<libdar::entrepot> get_entrepot(std::shared_ptr<html_web_user_interaction> webui) const { return entrep->get_entrepot(webui); };

	/// obtain the entrepot landing path
    const std::string & get_landing_path() const { return entrep->get_landing_path(); };

protected:

	/// inherited from bdy_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

    virtual void new_css_library_available() override;


private:
    html_derouleur deroule;

    html_form form_archgen;

    guichet guichet_entrep;
    std::shared_ptr<html_entrepot> entrep;

    html_form_fieldset fs_archgen;
    html_form_input allow_over;
    html_form_input warn_over;
    html_form_input pause;
    html_form_input sequential_marks;
    html_form_input_unit sparse_file_min_size;
    html_form_input user_comment;
    html_hash_algo hash_algo;
    html_form_input execute;
    html_form_input empty;

    html_form_fieldset has_aux_fs;
    html_form_input has_aux;
    html_form_input decremental;
    html_form aux_form;

    html_form_fieldset aux_block;
    html_archive_read auxiliary;

    html_form form_delta_sig;
    html_form_fieldset delta_fs;
    html_form_select delta_sig;

    html_text delta_filter_params;

    guichet guichet_sig_block_size;
    std::shared_ptr<html_form_sig_block_size> sig_block_size;

    html_text delta_filter_title;

    guichet guichet_delta_mask;
    std::shared_ptr<html_mask_form_filename> delta_mask;

    html_form form_shown;
    html_form_fieldset fs_shown;
    html_form_input info_details;
    html_form_input display_treated;
    html_form_input display_treated_only_dir;
    html_form_input display_skipped;

    html_form form_perimeter;
    html_form_fieldset fs_perimeter;
    html_form_input empty_dir;

    guichet guichet_filename_mask;
    std::shared_ptr<html_mask_form_filename> filename_mask;

    guichet guichet_path_mask;
    std::shared_ptr<html_mask_form_path> path_mask;

    guichet guichet_ea_mask;
    std::shared_ptr<html_mask_form_filename> ea_mask;

    html_fsa_scope fsa_scope;

    guichet guichet_overwriting_policy;
    std::shared_ptr<html_form_overwrite_action> overwriting_policy;

    guichet guichet_compr_params;
    std::shared_ptr<html_compression_params> compr_params;
    html_text compr_filter_title;

    guichet guichet_compr_mask;
    std::shared_ptr<html_mask_form_filename> compr_mask;

    guichet guichet_slicing;
    std::shared_ptr<html_slicing> slicing;

    guichet guichet_ciphering;
    std::shared_ptr<html_ciphering> ciphering;

    bool ignore_events;
    void init(); ///< set default values to parameters
    void trigger_changed();

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_options_merge";

    static constexpr const char* jlabel_entrep = "entrepot";
    static constexpr const char* jlabel_allow_over = "allow-over";
    static constexpr const char* jlabel_warn_over = "warn-over";
    static constexpr const char* jlabel_pause = "pause";
    static constexpr const char* jlabel_seq_marks = "sequential-marks";
    static constexpr const char* jlabel_sparse_min_size = "sparse-file-min-size";
    static constexpr const char* jlabel_user_comment = "user-comments";
    static constexpr const char* jlabel_hash_algo = "hash-algo";
    static constexpr const char* jlabel_execute = "execute";
    static constexpr const char* jlabel_empty = "dry-run";
    static constexpr const char* jlabel_has_aux = "has-auxiliary";
    static constexpr const char* jlabel_decremental = "decremental";
    static constexpr const char* jlabel_auxiliary = "auxiliary";
    static constexpr const char* jlabel_delta_sig = "delta-sig";
    static constexpr const char* jlabel_sig_block_size = "delta-sig-block-size";
    static constexpr const char* jlabel_delta_mask = "delta-mask";
    static constexpr const char* jlabel_info_details = "info-details";
    static constexpr const char* jlabel_display_treated = "disp-treated";
    static constexpr const char* jlabel_display_only_dir = "disp-only-dirs";
    static constexpr const char* jlabel_display_skipped = "disp-skipped";
    static constexpr const char* jlabel_empty_dir = "empty-dirs";
    static constexpr const char* jlabel_filename_mask = "filename-mask";
    static constexpr const char* jlabel_path_mask = "path-mask";
    static constexpr const char* jlabel_ea_mask = "ea-mask";
    static constexpr const char* jlabel_fsa_scope = "fsa-scope";
    static constexpr const char* jlabel_overwrite_policy = "overwriting-policy";
    static constexpr const char* jlabel_compr_params = "compression-params";
    static constexpr const char* jlabel_compr_mask = "compression-mask";
    static constexpr const char* jlabel_slicing = "slicing";
    static constexpr const char* jlabel_ciphering = "ciphering";


};

#endif
