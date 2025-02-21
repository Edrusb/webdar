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

#ifndef HTML_CIPHERING_HPP
#define HTML_CIPHERING_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <vector>
#include <string>

    // webdar headers
#include "body_builder.hpp"
#include "actor.hpp"
#include "html_form.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_select.hpp"
#include "html_crypto_algo.hpp"
#include "html_form_input.hpp"
#include "html_form_input_unit.hpp"
#include "html_form_gnupg_list.hpp"
#include "html_form_radio.hpp"
#include "jsoner.hpp"

    /// html component used for the user to define a has algorithm

class html_ciphering : public body_builder,
		       public actor,
		       public jsoner,
		       public events
{
public:
    static const std::string changed;

    enum crypto_type_t
	{
	    sym = 0,
	    asym = 1
    };

    html_ciphering();
    html_ciphering(const html_ciphering & ref) = delete;
    html_ciphering(html_ciphering && ref) noexcept = delete;
    html_ciphering & operator = (const html_ciphering & ref) = delete;
    html_ciphering & operator = (html_ciphering && ref) noexcept = delete;
    ~html_ciphering() = default;

    void set_crypto_size_range(const libdar::infinint & min,
			       const libdar::infinint & max);

    libdar::U_I get_multi_threaded_crypto() const;
    libdar::crypto_algo get_crypto_algo() const;
    crypto_type_t get_crypto_type() const;
    libdar::secu_string get_crypto_pass() const;
    libdar::infinint get_iteration_count() const;
    libdar::hash_algo get_kdf_hash() const;
    std::vector<std::string> get_gnupg_recipients() const;
    std::vector<std::string> get_gnupg_signatories() const;
    libdar::U_32 get_crypto_size() const;

	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override;

	/// actor indirect inheritance
    virtual void on_event(const std::string & event_name) override;

	/// set the object to webdar defaults
    void set_to_webdar_defaults();

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void css_classes_have_changed() override;

private:
	// specific events
    static constexpr const char* kdf_algo_changed = "kdf_algo_changed";

    html_form form_crypto;
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
    bool ignore_events;

    void set_kdf_hash(libdar::hash_algo hash);

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_ciphering";

    static constexpr const char* jlabel_type = "type";
    static constexpr const char* jlabel_algo = "algo";
    static constexpr const char* jlabel_pass = "key";
    static constexpr const char* jlabel_size = "block-size";
    static constexpr const char* jlabel_threads = "threads";
    static constexpr const char* jlabel_gnupg = "gnupg";
    static constexpr const char* jlabel_kdf_hash = "hash";
    static constexpr const char* jlabel_kdf_iter = "iterations";

};

#endif
