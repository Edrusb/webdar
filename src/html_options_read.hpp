/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2022 Denis Corbin
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

#ifndef HTML_OPTIONS_READ_HPP
#define HTML_OPTIONS_READ_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "html_div.hpp"
#include "html_form.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_select.hpp"
#include "html_form_input.hpp"
#include "html_crypto_algo.hpp"
#include "events.hpp"

class html_options_read : public body_builder, public actor
{
public:
    html_options_read();
    html_options_read(const html_options_read & ref) = default;
    html_options_read(html_options_read && ref) noexcept = default;
    html_options_read & operator = (const html_options_read & ref) = default;
    html_options_read & operator = (html_options_read && ref) noexcept = default;
    ~html_options_read() = default;

    const libdar::archive_options_read & get_options() const;

	/// inherited from actor
    void on_event(const std::string & event_name);

	/// inherited from body_builder
    std::string get_body_part(const chemin & path,
			      const request & req);

private:
	// construction fields
    html_div div;
    html_form form_src;
    html_form_fieldset fs_src;
    html_form_fieldset fs_ref;
	// archive_options_read fields
    html_crypto_algo src_crypto_algo;
    html_form_input src_crypto_pass;
    html_form_input src_crypto_size;
    html_form_input src_execute;
    html_form_input src_slice_min_digits;
    html_form_input info_details;
    html_form_input lax;
    html_form_input sequential_read;
	// html_select src_entrepot;
    html_form_input src_use_external_catalogue;
    html_form form_ref;
    html_form_input ref_path;
    html_form_input ref_basename;
    html_crypto_algo ref_crypto_algo;
    html_form_input ref_crypto_pass;
    html_form_input ref_crypto_size;
    html_form_input ref_execute;
    html_form_input ref_slice_min_digits;
	// html_select ref_entrepot;

    libdar::archive_options_read opts;
};

#endif
