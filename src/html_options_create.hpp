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

#ifndef HTML_OPTIONS_CREATE_HPP
#define HTML_OPTIONS_CREATE_HPP

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
#include "html_compression.hpp"
#include "html_crypto_algo.hpp"
#include "html_form_input.hpp"
#include "html_comparison_fields.hpp"
#include "html_form_fieldset.hpp"
#include "html_datetime.hpp"
#include "html_form.hpp"
#include "html_archive_read.hpp"
#include "html_hash_algo.hpp"
#include "html_derouleur.hpp"
#include "html_text.hpp"
#include "html_entrepot.hpp"
#include "html_web_user_interaction.hpp"
#include "html_mask_form_filename.hpp"
#include "html_mask_form_path.hpp"
#include "html_form_select.hpp"
#include "html_form_gnupg_list.hpp"
#include "html_form_same_fs.hpp"
#include "html_form_ignore_as_symlink.hpp"
#include "html_form_sig_block_size.hpp"
#include "html_form_input_unit.hpp"

    /// html component used for the user to provide the parameters to create a new archive

class html_options_create : public body_builder, public actor, public events
{
public:
    static const std::string entrepot_changed;

    html_options_create();
    html_options_create(const html_options_create & ref) = delete;
    html_options_create(html_options_create && ref) noexcept = delete;
    html_options_create & operator = (const html_options_create & ref) = delete;
    html_options_create & operator = (html_options_create && ref) noexcept = delete;
    ~html_options_create() = default;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// informations available for libdar
	///
	/// \note if a reference is requested it implies openninng an archive
	/// and feeding the libdar::archive_options_create::set_reference(libdar::archive *)
	/// with the object built from this->get_reference().*
    libdar::archive_options_create get_options(std::shared_ptr<html_web_user_interaction> & webui) const;

	/// optain the current entrepot object where is expected to be create the archive
    std::shared_ptr<libdar::entrepot> get_entrepot(std::shared_ptr<html_web_user_interaction> webui) const { return entrep.get_entrepot(webui); };

	/// needed for path based filtering to filter accordingly to the current root_fs
    void set_fs_root(const std::string & prefix) { path_mask.set_fs_root(prefix); };

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
	// specific events
    static constexpr const char* kdf_algo_changed = "kdf_algo_changed";


    html_derouleur deroule;

    html_entrepot entrep;

    html_form form_archtype;

    html_form form_delta_sig;

    html_form form_reading;

    html_form form_archgen;

    html_form form_shown;

    html_form form_perimeter;

    html_mask_form_filename filename_mask;

    html_mask_form_path path_mask;

    html_mask_form_filename ea_mask;

    html_form form_same_fs;

    html_form form_compr;

    html_form form_slicing;

    html_form form_crypto;

    html_form_fieldset archtype_fs;
    html_form_radio archtype;      // fs_archtype member
    html_datetime fixed_date;      // fs_archtype member
    html_archive_read reference;   // fs_archtype member
    html_form_fieldset wrap_ref;   // help make distinction at display between archive or ref options and archive options
    html_text binary_delta_note;   // note about binary delta requirement
    html_text ref_placeholder;     // shows when archtype is not differential/incremental

    html_form_fieldset delta_fs;
    html_form_input delta_sig;
    html_form_input_unit delta_sig_min_size;
    html_form_sig_block_size sig_block_size;
    html_mask_form_filename delta_mask;

    html_form_fieldset fs_alter_atime;
    html_form_radio alter_atime;
    html_form_input furtive_read_mode;
    html_form_input zeroing_neg_date;
    html_form_fieldset fs_mod_data_detect;
    html_form_radio mod_data_detect;
    html_form_ignore_as_symlink follow_symlinks;

    html_form_fieldset archgen_fs;
    html_form_input allow_over;
    html_form_input warn_over;
    html_form_input pause;
    html_form_input slice_permission;
    html_form_input slice_user_ownership;
    html_form_input slice_group_ownership;
    html_form_input retry_on_change_times;
    html_form_input_unit retry_on_change_overhead;
    html_form_input sequential_marks;
    html_form_input_unit sparse_file_min_size;
    html_form_input user_comment;
    html_form_input slice_min_digits;
    html_hash_algo hash_algo;
    html_form_input execute;
    html_comparison_fields what_to_check;
    html_form_input hourshift;     // fs_archtype member
    html_form_input empty;

    html_form_fieldset shown_fs;
    html_form_input info_details;
    html_form_input display_treated;
    html_form_input display_treated_only_dir;
    html_form_input display_skipped;
    html_form_input display_dir_summary;
    html_form_input security_check;
    html_form_input dont_ignore_unknown_inode_type;

    html_form_fieldset perimeter_fs;
    html_form_input empty_dir;
    html_form_input cache_directory_tagging;
    html_form_input nodump;
    html_form_fieldset exclu_by_ea_fs;
    html_form_input exclude_by_ea;
    html_form_input default_ea;
    html_form_input exclude_by_ea_name;

    html_form_same_fs same_fs;
    html_form_fieldset same_fs_fs;

    html_form_fieldset compr_fs;
    html_compression compression;
    html_form_input compression_level;
    html_form_input_unit min_compr_size;
    html_form_input_unit compression_block;
    html_form_input never_resave_uncompressed;
    html_form_input compr_threads;
    html_mask_form_filename compr_mask;

    html_form_fieldset slicing_fs;
    html_form_input slicing;
    html_form_input_unit slice_size;
    html_form_input different_first_slice;
    html_form_input_unit first_slice_size;

    html_form_fieldset crypto_fs;
    html_form_select crypto_type;
    html_crypto_algo crypto_algo;
    html_form_input crypto_pass1;
    html_form_input crypto_pass2;
    html_form_input_unit crypto_size;
    html_form_input crypto_threads;
    html_form_gnupg_list gnupg;
    html_form_fieldset crypto_fs_kdf_hash;
    html_form_radio crypto_kdf_hash;
    html_form_input iteration_count;
};

#endif
