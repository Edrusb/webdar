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

#ifndef HTML_FORM_OVERWRITE_CHAIN_ACTION_HPP
#define HTML_FORM_OVERWRITE_CHAIN_ACTION_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "actor.hpp"
#include "html_overwrite_action.hpp"
#include "html_form_fieldset.hpp"
#include "html_table.hpp"
#include "html_button.hpp"
#include "html_form_input.hpp"
#include "html_hr.hpp"
#include "html_form_dynamic_table.hpp"
#include "jsoner.hpp"

    /// html component to be adopted by an html_form that implements a chain of actions for overwriting policies

class html_form_overwrite_chain_action: public html_overwrite_action,
					public html_form_dynamic_table_object_provider,
					public jsoner

{
public:
    html_form_overwrite_chain_action(const std::string & label);
    html_form_overwrite_chain_action(const html_form_overwrite_chain_action & ref) = default;
    html_form_overwrite_chain_action(html_form_overwrite_chain_action && ref) noexcept = default;
    html_form_overwrite_chain_action & operator = (const html_form_overwrite_chain_action & ref) = default;
    html_form_overwrite_chain_action & operator = (html_form_overwrite_chain_action && ref) noexcept = default;
    ~html_form_overwrite_chain_action() = default;

	/// obtain the crit_chain_action object for libdar option
    virtual std::unique_ptr<libdar::crit_action> get_overwriting_action() const override;

	/// inherited from html_form_dynamic_table_object_provider
    virtual std::unique_ptr<body_builder> provide_object_of_type(unsigned int num, const std::string & context) const override;

	/// inherited from jsoner
    virtual void load_json(const json & source) override { table.load_json(source); };

	/// inherited from jsoner
    virtual json save_json() const override { return table.save_json(); };

	/// inherited from jsoner
    virtual void clear_json() override { table.clear_json(); };

private:
    html_form_fieldset fs;
    html_form_dynamic_table table;

};

#endif
