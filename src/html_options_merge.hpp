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

    /// html component for the user to provided parameters of a libdar merging operation

class html_options_merge : public body_builder, public actor, public events
{
public:
    static const std::string entrepot_changed;

    html_options_merge();
    html_options_merge(const html_options_merge & ref) = delete;
    html_options_merge(html_options_merge && ref) noexcept = delete;
    html_options_merge & operator = (const html_options_merge & ref) = delete;
    html_options_merge & operator = (html_options_merge && ref) noexcept = delete;
    ~html_options_merge() = default;

	/// mandatory call to invoke ASAP after constructor
    void set_biblio(const std::shared_ptr<bibliotheque> & ptr);

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
    html_form_input_unit delta_sig_min_size;
    html_form_sig_block_size sig_block_size;
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

    html_form_overwrite_action overwriting_policy;
    html_form form_overwriting;

    guichet guichet_compr_params;
    std::shared_ptr<html_compression_params> compr_params;
    html_text compr_filter_title;

    guichet guichet_compr_mask;
    std::shared_ptr<html_mask_form_filename> compr_mask;

    guichet guichet_slicing;
    std::shared_ptr<html_slicing> slicing;

    guichet guichet_ciphering;
    std::shared_ptr<html_ciphering> ciphering;

};

#endif
