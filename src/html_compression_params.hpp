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


    /// html component used for the user to define a has algorithm

class html_compression_params : public body_builder, public actor, public events
{
public:
	// change event
    static const std::string changed;

    html_compression_params(bool show_resave,
			    bool show_min_size);
    html_compression_params(const html_compression_params & ref) = delete;
    html_compression_params(html_compression_params && ref) noexcept = delete;
    html_compression_params & operator = (const html_compression_params & ref) = delete;
    html_compression_params & operator = (html_compression_params && ref) noexcept = delete;
    ~html_compression_params() = default;

    void set_compression_algo(libdar::compression val) { compression.set_value(val); };
    void set_compression_level(int val) { compression_level.set_value_as_int(val); };
    void set_min_compression_size(const libdar::infinint & val) { min_compr_size.set_value_as_infinint(val); };
    void set_compression_block(const libdar::infinint & val) { compression_block.set_value_as_infinint(val); };

    libdar::compression get_compression_algo() const { return compression.get_value(); };
    libdar::U_I get_compression_level() const { return compression_level.get_value_as_int(); };
    libdar::infinint get_min_compression_size() const { return min_compr_size.get_value_as_infinint(); };
    libdar::infinint get_compression_block() const { return compression_block.get_value_as_infinint(); };
    bool get_resave_uncompressed() const { return never_resave_uncompressed.get_value_as_bool(); };
    libdar::U_I get_num_threads() const { return compr_threads.get_value_as_int(); };

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;


private:
    html_form form_compr;
    html_form_fieldset compr_fs;
    html_compression compression;
    html_form_input compression_level;
    html_form_input_unit min_compr_size;
    html_form_input_unit compression_block;
    html_form_input never_resave_uncompressed;
    html_form_input compr_threads;

    bool x_show_resave;
    bool x_show_min_size;
};

#endif
