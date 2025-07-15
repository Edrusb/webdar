/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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

#ifndef HTML_COMPRESSION_PARAMS_HPP
#define HTML_COMPRESSION_PARAMS_HPP

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
#include "html_form_select.hpp"
#include "html_compression.hpp"
#include "html_form_input.hpp"
#include "html_form_input_unit.hpp"
#include "webdar_tools.hpp"
#include "jsoner.hpp"

    /// html component used for the user to define a compression algorithm

class html_compression_params : public body_builder,
				public actor,
				public events,
				public jsoner

{
public:
	// change event
    static const std::string changed;

    html_compression_params(bool show_resave,
			    bool show_min_size,
			    bool show_keep_compressed);
    html_compression_params(const html_compression_params & ref) = delete;
    html_compression_params(html_compression_params && ref) noexcept = delete;
    html_compression_params & operator = (const html_compression_params & ref) = delete;
    html_compression_params & operator = (html_compression_params && ref) noexcept = delete;
    ~html_compression_params() = default;

    void set_compression_algo(libdar::compression val) { compression.set_value(val); };
    void set_compression_level(int val) { compression_level.set_value_as_int(val); };
    void set_min_compression_size(const libdar::infinint & val) { min_compr_size.set_value_as_infinint(val); };
    void set_compression_block(const libdar::infinint & val) { compression_block.set_value_as_infinint(val); };
    void set_keep_compressed(bool val) { keep_compressed.set_value_as_bool(val); };

    libdar::compression get_compression_algo() const { return compression.get_value(); };
    libdar::U_I get_compression_level() const { return compression_level.get_value_as_int(); };
    libdar::infinint get_min_compression_size() const { return min_compr_size.get_value_as_infinint(); };
    libdar::infinint get_compression_block() const { return compression_block.get_value_as_infinint(); };
    bool get_resave_uncompressed() const { return never_resave_uncompressed.get_value_as_bool(); };
    libdar::U_I get_num_threads() const { return compr_threads.get_value_as_int(); };
    bool get_keep_compressed() const { return keep_compressed.get_value_as_bool(); };

	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// set the object to webdar defaults
    void set_to_webdar_defaults();

	/// inherited from body_builder
    virtual void bind_to_anchor(const std::string & val) override { form_compr.bind_to_anchor(val); };

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void css_classes_have_changed() override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    bool x_show_resave;
    bool x_show_min_size;
    bool ignore_events;

    html_form form_compr;
    html_form_fieldset compr_fs;
    html_compression compression;
    html_form_input compression_level;
    html_form_input_unit min_compr_size;
    html_form_input_unit compression_block;
    html_form_input never_resave_uncompressed;
    html_form_input compr_threads;
    html_form_input keep_compressed;

    static constexpr const unsigned int default_level = 9;
    static constexpr const unsigned int default_min_compr_size = 100;
    static constexpr const unsigned int default_compression_block = 240*1024;
    static constexpr const bool default_never_resave_uncompressed = false;
    static constexpr const unsigned int default_compr_threads = 4;
    static constexpr const bool default_keep_compressed = false;

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_compression_params";

    static constexpr const char* jlabel_algo = "compr-algo";
    static constexpr const char* jlabel_level = "compr-level";
    static constexpr const char* jlabel_min_compr_sz = "min-compr-size";
    static constexpr const char* jlabel_compr_block = "compr-block";
    static constexpr const char* jlabel_never_resave_uncompr = "never-resave-uncompr";
    static constexpr const char* jlabel_compr_threads = "compr-threads";
    static constexpr const char* jlabel_keep_compr = "keep-compressed";
};

#endif
