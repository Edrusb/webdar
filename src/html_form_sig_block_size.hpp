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

#ifndef HTML_FORM_SIG_BLOCK_SIZE_HPP
#define HTML_FORM_SIG_BLOCK_SIZE_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <list>
#include <deque>
#include <map>

    // webdar headers
#include "html_div.hpp"
#include "actor.hpp"
#include "events.hpp"
#include "body_builder.hpp"
#include "html_table.hpp"
#include "html_text.hpp"
#include "html_form_radio.hpp"
#include "html_form_input.hpp"
#include "html_form_input_unit.hpp"
#include "html_form_fieldset.hpp"

    /// html component used to define how/when delta signature are calculated

class html_form_sig_block_size: public html_div,
				public actor,
				public events
{
public:
    static const std::string changed;

    html_form_sig_block_size();
    html_form_sig_block_size(const html_form_sig_block_size & ref) = default;
    html_form_sig_block_size(html_form_sig_block_size && ref) noexcept = default;
    html_form_sig_block_size & operator = (const html_form_sig_block_size & ref) = default;
    html_form_sig_block_size & operator = (html_form_sig_block_size && ref) noexcept = default;
    ~html_form_sig_block_size() = default;

	/// define default/initial file size value below which to never try performing binary delta
    void set_delta_sig_min_size(const libdar::infinint & val) { delta_sig_min_size.set_value_as_infinint(val); };

	/// read the currently file size value set below which to never try perfoming binary delta
    libdar::infinint get_delta_sig_min_size() const { return delta_sig_min_size.get_value_as_infinint(); }

	/// return the corresponding value for libdar
    libdar::delta_sig_block_size get_value() const;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    html_form_input_unit delta_sig_min_size;
    html_text summary_f; // formula
    html_text summary_l; // limits
    html_form_fieldset fs_function;
    html_form_radio function;
    html_form_input multiply;
    html_form_input divisor;
    html_form_input_unit min_size;
    html_form_input_unit max_size;

    void make_summary();
};

#endif
