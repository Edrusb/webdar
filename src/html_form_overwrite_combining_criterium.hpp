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

#ifndef HTML_FORM_OVERWRITE_COMBINING_CRITERIUM_HPP
#define HTML_FORM_OVERWRITE_COMBINING_CRITERIUM_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <list>
#include <deque>
#include <map>

    // webdar headers
#include "html_overwrite_criterium.hpp"
#include "actor.hpp"
#include "body_builder.hpp"
#include "html_table.hpp"
#include "html_text.hpp"
#include "html_form_input.hpp"
#include "html_form_select.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_dynamic_table.hpp"

    /// html component used for to logically combine (and / or) criteria to setup an overwriting policies

class html_form_overwrite_combining_criterium: public html_overwrite_criterium,
					       public actor,
					       public html_form_dynamic_table_object_provider
{
public:
    html_form_overwrite_combining_criterium(const std::string & initial_mode = and_op);
    html_form_overwrite_combining_criterium(const html_form_overwrite_combining_criterium & ref) = default;
    html_form_overwrite_combining_criterium(html_form_overwrite_combining_criterium && ref) noexcept = default;
    html_form_overwrite_combining_criterium & operator = (const html_form_overwrite_combining_criterium & ref) = default;
    html_form_overwrite_combining_criterium & operator = (html_form_overwrite_combining_criterium && ref) noexcept = default;
    ~html_form_overwrite_combining_criterium() = default;

	/// obtain the crit_combining_criterium object for libdar option
    virtual std::unique_ptr<libdar::criterium> get_overwriting_criterium() const override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;


	/// inherited from html_form_dynamic_table_object_provider
    virtual std::unique_ptr<body_builder> provide_object_of_type(unsigned int num,
								 const std::string & context) const override;

	/// inherited from body_builder
    MAKE_BROTHER_MACRO;

	// mask_type values
    static constexpr const char* and_op = "and_op";
    static constexpr const char* or_op = "or_op";

protected:

	/// inherited from body_builder from html_overwrite_criterium
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

    	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
	// events
    static constexpr const char* bool_changed_event = "bool_changed";
    static constexpr const char* new_crit_to_add = "new_mask";

	// adder values
    static constexpr const char* base_crit = "base_crit";
    static constexpr const char* combining_crit = "combining_crit";

	// css class
    static constexpr const char* css_class_bool_text = "html_form_over_comb_crit";


    html_form_fieldset fs;           ///< wrapps all html form components of this class
    html_form_select crit_type;      ///< either "and" or "or" combination
    html_form_dynamic_table table;   ///< adopts all member we are combining
    std::string current_bool_mode;   ///< current value combination mode

    std::string bool_op_to_name(const std::string & op);
    void update_table_content_logic(bool unconditionally);

    static std::string invert_logic(const std::string & logic);

};

#endif
