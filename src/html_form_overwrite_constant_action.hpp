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

#ifndef HTML_FORM_OVERWRITE_CONSTANT_ACTION_HPP
#define HTML_FORM_OVERWRITE_CONSTANT_ACTION_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_overwrite_action.hpp"
#include "body_builder.hpp"
#include "html_form_select.hpp"
#include "html_form_fieldset.hpp"
#include "jsoner.hpp"
#include "events.hpp"
#include "actor.hpp"

    /// html component to be adopted by an html_form that implements constant actions for overwriting policies

class html_form_overwrite_constant_action: public html_overwrite_action,
					   public jsoner,
					   public events,
					   public actor
{
public:
    static const std::string changed;

    html_form_overwrite_constant_action();
    html_form_overwrite_constant_action(const html_form_overwrite_constant_action & ref) = default;
    html_form_overwrite_constant_action(html_form_overwrite_constant_action && ref) noexcept = default;
    html_form_overwrite_constant_action & operator = (const html_form_overwrite_constant_action & ref) = default;
    html_form_overwrite_constant_action & operator = (html_form_overwrite_constant_action && ref) noexcept = default;
    ~html_form_overwrite_constant_action() = default;

	/// obtain the crit_constant_action object for libdar option
    virtual std::unique_ptr<libdar::crit_action> get_overwriting_action() const override;

	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

private:
    html_form_fieldset action_fs;
    html_form_select data_action;
    html_form_select ea_action;

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_form_overwrite_constant_action";

    static constexpr const char* jlabel_data = "data";
    static constexpr const char* jlabel_ea = "ea";
};

#endif
