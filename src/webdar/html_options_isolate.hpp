/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013 Denis Corbin
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
#include "html_compression.hpp"
#include "html_size_unit.hpp"
#include "html_hash_algo.hpp"
#include "html_crypto_algo.hpp"

class html_options_isolate : public body_builder, public actor
{
public:
    html_options_isolate();

	/// inherited from bdy_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

	/// inherited from actor
    virtual void on_event(const std::string & event_name);

    libdar::archive_options_isolate get_options() const;

private:
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

    html_form form_compr;
    html_form_fieldset fs_compr;
    html_compression compression;
    html_form_input compression_level;


    html_form form_slicing;
    html_form_fieldset fs_slicing;
    html_form_input slicing;
    html_form_input slice_size;
    html_size_unit slice_size_unit;
    html_form_input different_first_slice;
    html_form_input first_slice_size;
    html_size_unit first_slice_size_unit;


    html_form form_crypto;
    html_form_fieldset fs_crypto;
    html_crypto_algo crypto_algo;
    html_form_input crypto_pass1;
    html_form_input crypto_pass2;
    html_form_input crypto_size;
};

#endif
