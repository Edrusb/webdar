#ifndef OPTIONS_CREATE_HPP
#define OPTIONS_CREATE_HPP

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
#include "archive_read.hpp"
#include "html_hash_algo.hpp"

class options_create : public body_builder , public actor
{
public:
    options_create();

    libdar::archive_options_create get_options() const;

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

	/// inherited from actor
    virtual void on_event(const std::string & event_name);

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
    archive_read reference;      // fs_archtype member
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
