/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2023 Denis Corbin
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
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "html_compression.hpp"
#include "html_crypto_algo.hpp"
#include "html_form_input.hpp"
#include "html_comparison_fields.hpp"
#include "html_size_unit.hpp"
#include "html_form_fieldset.hpp"
#include "html_datetime.hpp"
#include "html_form.hpp"
#include "html_archive_read.hpp"
#include "html_hash_algo.hpp"

class html_options_create : public body_builder , public actor
{
public:
    html_options_create();
    html_options_create(const html_options_create & ref) = default;
    html_options_create(html_options_create && ref) noexcept = default;
    html_options_create & operator = (const html_options_create & ref) = default;
    html_options_create & operator = (html_options_create && ref) noexcept = default;
    ~html_options_create() = default;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// informations available for libdar
	///
	/// \note if a reference is requested it implies openninng an archive
	/// and feeding the libdar::archive_options_create::set_reference(libdar::archive *)
	/// with the object built from this->get_reference().*
    libdar::archive_options_create get_options() const;
	/// whether an archive of reference has to be added to the libdar::archive_options_create object
    bool has_reference() const { return archtype.get_selected_num() == 1; };
	/// parameters required to build the archive of reference
    const html_archive_read & get_reference() const { return reference; };

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;


private:
    html_form form_archtype;
    html_form_fieldset fs_archtype;

    html_form form_archgen;
    html_form_fieldset fs_archgen;

    html_form form_shown;
    html_form_fieldset fs_shown;

    html_form form_perimeter;
    html_form_fieldset fs_perimeter;

    html_form form_reading;
    html_form_fieldset fs_reading;

    html_form form_compr;
    html_form_fieldset fs_compr;

    html_form form_slicing;
    html_form_fieldset fs_slicing;

    html_form form_crypto;
    html_form_fieldset fs_crypto;

    html_form_radio archtype;    // fs_archtype member
    html_archive_read reference;      // fs_archtype member
    html_form_input allow_over;
    html_form_input warn_over;
    html_form_input info_details;
    html_form_input pause;
    html_form_input empty_dir;
    html_compression compression;
    html_form_input compression_level;
    html_form_input slicing;
    html_form_input slice_size;
    html_size_unit slice_size_unit;
    html_form_input different_first_slice;
    html_form_input first_slice_size;
    html_size_unit first_slice_size_unit;
    html_form_input execute;
    html_crypto_algo crypto_algo;
    html_form_input crypto_pass1;
    html_form_input crypto_pass2;
    html_form_input crypto_size;
    html_form_input min_compr_size;
    html_size_unit min_compr_size_unit;
    html_form_input nodump;
    html_comparison_fields what_to_check;
    html_form_input hourshift;     // fs_archtype member
    html_form_input empty;
    html_form_fieldset fs_alter_atime;
    html_form_radio alter_atime;
    html_form_input furtive_read_mode;
    html_form_input same_fs;
    html_form_input cache_directory_tagging;
    html_form_input display_skipped;
    html_datetime fixed_date;      // fs_archtype member
    html_form_input slice_permission;
    html_form_input slice_user_ownership;
    html_form_input slice_group_ownership;
    html_form_input retry_on_change_times;
    html_form_input retry_on_change_overhead;
    html_size_unit retry_on_change_overhead_unit;
    html_form_input sequential_marks;
    html_form_input sparse_file_min_size;
    html_size_unit sparse_file_min_size_unit;
    html_form_input security_check;
    html_form_input user_comment;
    html_hash_algo hash_algo;
    html_form_input slice_min_digits;
    html_form_input ignore_unknown_inode_type;
};

#endif
