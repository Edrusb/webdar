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

#ifndef HTML_FORM_OVERWRITE_CONDITIONAL_ACTION_HPP
#define HTML_FORM_OVERWRITE_CONDITIONAL_ACTION_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_overwrite_action.hpp"
#include "html_form_overwrite_combining_criterium.hpp"
#include "body_builder.hpp"
#include "actor.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_overwrite_action.hpp"

    /// html component to be adopted by an html_form that implements conditional actions for overwriting policies

class html_form_overwrite_conditional_action: public html_overwrite_action
{
public:
    html_form_overwrite_conditional_action();
    html_form_overwrite_conditional_action(const html_form_overwrite_conditional_action & ref) = default;
    html_form_overwrite_conditional_action(html_form_overwrite_conditional_action && ref) noexcept = default;
    html_form_overwrite_conditional_action & operator = (const html_form_overwrite_conditional_action & ref) = default;
    html_form_overwrite_conditional_action & operator = (html_form_overwrite_conditional_action && ref) noexcept = default;
    ~html_form_overwrite_conditional_action() = default;

	/// obtain the crit_conditional_action object for libdar option
    virtual std::unique_ptr<libdar::crit_action> get_overwriting_action() const override;

protected:

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    static constexpr const char* act_true_changed = "true changed";
    static constexpr const char* act_false_changed = "false changed";

    static constexpr const char* action_type_undefined = "undefined";
    static constexpr const char* action_type_const = "constant";
    static constexpr const char* action_type_condition = "condition";

    static constexpr const char* css_cond = "html_form_overwrite_cond_act";

    html_form_overwrite_combining_criterium condition;
    html_form_overwrite_action when_true;
    html_form_overwrite_action when_false;
};

#endif
