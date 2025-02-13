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
#include "html_form_same_fs.hpp"
#include "html_form_ignore_as_symlink.hpp"
#include "html_form_sig_block_size.hpp"
#include "html_form_input_unit.hpp"
#include "html_compression_params.hpp"
#include "html_slicing.hpp"
#include "html_ciphering.hpp"
#include "html_fsa_scope.hpp"
#include "guichet.hpp"
#include "jsoner.hpp"
#include "bibliotheque_subconfig.hpp"

    /// html component used for the user to provide the parameters to create a new archive

class html_options_create : public body_builder,
			    public actor,
			    public events,
			    public jsoner,
			    public bibliotheque_subconfig
{
public:
    static const std::string landing_path_changed; ///< only change is related to entrepot's landing_path
    static const std::string entrepot_changed; ///< only changes are related to entrepot (excluding landing_path)
    static const std::string changed; ///< any parameter change (including entrepot_changed, but excluding landing_path)

    html_options_create();
    html_options_create(const html_options_create & ref) = delete;
    html_options_create(html_options_create && ref) noexcept = delete;
    html_options_create & operator = (const html_options_create & ref) = delete;
    html_options_create & operator = (html_options_create && ref) noexcept = delete;
    ~html_options_create() = default;

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
	/// and feeding the libdar::archive_options_create::set_reference(libdar::archive *)
	/// with the object built from this->get_reference().*
    libdar::archive_options_create get_options(std::shared_ptr<html_web_user_interaction> & webui) const;

	/// obtain the current entrepot object where is expected to be create the archive
    std::shared_ptr<libdar::entrepot> get_entrepot(std::shared_ptr<html_web_user_interaction> webui) const { return entrep->get_entrepot(webui); };

	/// obtain the current landing path
    const std::string & get_landing_path() const { return entrep->get_landing_path(); };

	/// needed for path based filtering to filter accordingly to the current root_fs
    void set_fs_root(const std::string & prefix) { path_mask->set_fs_root(prefix); };

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:

    html_derouleur deroule;

    guichet guichet_entrep;
    std::shared_ptr<html_entrepot> entrep;

    html_form form_archtype;

    html_form form_delta_sig;

    html_form form_reading;

    html_form form_archgen;

    html_form form_shown;

    html_form form_perimeter;

    guichet guichet_filename_mask;
    std::shared_ptr<html_mask_form_filename> filename_mask;

    guichet guichet_path_mask;
    std::shared_ptr<html_mask_form_path> path_mask;

    guichet guichet_ea_mask;
    std::shared_ptr<html_mask_form_filename> ea_mask;

    html_form form_same_fs;

    html_form_fieldset archtype_fs;
    html_form_radio archtype;      // fs_archtype member
    html_datetime fixed_date;      // fs_archtype member
    html_archive_read reference;   // fs_archtype member
    html_form_fieldset wrap_ref;   // help make distinction at display between archive or ref options and archive options
    html_text binary_delta_note;   // note about binary delta requirement
    html_text ref_placeholder;     // shows when archtype is not differential/incremental

    html_form_fieldset delta_fs;
    html_form_input delta_sig;

    html_text delta_filter_params;

    guichet guichet_sig_block_size;
    std::shared_ptr<html_form_sig_block_size> sig_block_size;

    html_text delta_filter_title;

    guichet guichet_delta_mask;
    std::shared_ptr<html_mask_form_path> delta_mask;

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
    html_form_input retry_on_change_times;
    html_form_input_unit retry_on_change_overhead;
    html_form_input sequential_marks;
    html_form_input_unit sparse_file_min_size;
    html_form_input user_comment;
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

    guichet guichet_compr_params;
    std::shared_ptr<html_compression_params> compr_params;

    html_text compr_filter_title;
    guichet guichet_compr_mask;
    std::shared_ptr<html_mask_form_filename> compr_mask;

    html_fsa_scope fsa_scope;

    guichet guichet_slicing;
    std::shared_ptr<html_slicing> slicing;

    guichet guichet_ciphering;
    std::shared_ptr<html_ciphering> ciphering;

    bool ignore_events;

    void init();
    void trigger_changed();

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_form_options_create";

    static constexpr const char* jlabel_entrep = "entrepot";
    static constexpr const char* jlabel_file_mask = "file-mask";
    static constexpr const char* jlabel_path_mask = "path-mask";
    static constexpr const char* jlabel_ea_mask = "ea-mask";
    static constexpr const char* jlabel_archtype = "archive-type";
    static constexpr const char* jlabel_fixed_date = "fixed-date";
    static constexpr const char* jlabel_reference = "reference";
    static constexpr const char* jlabel_delta_sig = "delta-sig";
    static constexpr const char* jlabel_sig_block_size = "delta-sig-block-size";
    static constexpr const char* jlabel_delta_mask = "delta-mask";
    static constexpr const char* jlabel_alter_atime = "alter-atime";
    static constexpr const char* jlabel_furtive_read = "furtive-read";
    static constexpr const char* jlabel_zeroing = "zeroing-neg-dates";
    static constexpr const char* jlabel_mod_data = "modified-data-detection";
    static constexpr const char* jlabel_follow_sym = "follow-symlinks";
    static constexpr const char* jlabel_allow_over = "allow-overwrite";
    static constexpr const char* jlabel_warn_over = "warn-overwrite";
    static constexpr const char* jlabel_pause = "pause";
    static constexpr const char* jlabel_retry_times = "retry-times";
    static constexpr const char* jlabel_retry_overhead = "retry-overhead";
    static constexpr const char* jlabel_sequential_marks = "tape-marks";
    static constexpr const char* jlabel_sparse_min_size = "sparse-file-min-size";
    static constexpr const char* jlabel_user_comment = "user-comment";
    static constexpr const char* jlabel_hash_algo = "hash-algo";
    static constexpr const char* jlabel_execute = "execute";
    static constexpr const char* jlabel_what_to_check = "what-to-check";
    static constexpr const char* jlabel_hourshift = "hourshift";
    static constexpr const char* jlabel_empty = "dry-run";
    static constexpr const char* jlabel_info_details = "info-details";
    static constexpr const char* jlabel_display_treated = "display-treated";
    static constexpr const char* jlabel_display_only_dir = "display-on-dirs";
    static constexpr const char* jlabel_display_skipped = "display-skipped";
    static constexpr const char* jlabel_display_dir_summ = "display-dir-summary";
    static constexpr const char* jlabel_secu_check = "security-check";
    static constexpr const char* jlabel_dont_ignore_unknown = "dont-ignore-unknown-inode-types";
    static constexpr const char* jlabel_empty_dir = "empty-dirs";
    static constexpr const char* jlabel_cache_dir_tag = "cache-dir-tagging";
    static constexpr const char* jlabel_nodump = "nodump";
    static constexpr const char* jlabel_exclude_by_ea = "exclude-by-ea";
    static constexpr const char* jlabel_default_ea = "exclude-by-ea-default-name";
    static constexpr const char* jlabel_exclude_by_ea_name = "exclude-by-ea-named";
    static constexpr const char* jlabel_same_fs = "same-fs";
    static constexpr const char* jlabel_compr_params = "compress-params";
    static constexpr const char* jlabel_compr_mask = "compress-mask";
    static constexpr const char* jlabel_fsa_scope = "fsa-scope";
    static constexpr const char* jlabel_slicing = "slicing";
    static constexpr const char* jlabel_ciphering = "ciphering";


};

#endif
