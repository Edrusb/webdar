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
#include "jsoner.hpp"
#include "events.hpp"
#include "actor.hpp"

    /// html component to be adopted by an html_form that implements conditional actions for overwriting policies

class html_form_overwrite_conditional_action: public html_overwrite_action,
					      public jsoner,
					      public actor,
					      public events
{
public:
    static const std::string changed;

    html_form_overwrite_conditional_action();
    html_form_overwrite_conditional_action(const html_form_overwrite_conditional_action & ref) = default;
    html_form_overwrite_conditional_action(html_form_overwrite_conditional_action && ref) noexcept = default;
    html_form_overwrite_conditional_action & operator = (const html_form_overwrite_conditional_action & ref) = default;
    html_form_overwrite_conditional_action & operator = (html_form_overwrite_conditional_action && ref) noexcept = default;
    ~html_form_overwrite_conditional_action() = default;

	/// provide access to the condition object

	/// \note this call is to programmatically setup a component, normal way to setup is from Web user interaction
    html_form_overwrite_combining_criterium & get_access_to_condition() { return condition; };

	/// provide access to the action used when condition is false

	/// \note this call is to programmatically setup a component, normal way to setup is from Web user interaction
    html_form_overwrite_action & get_access_to_action_true() { return when_true; };

	/// provide access to the action used when condition is false

	/// \note this call is to programmatically setup a component, normal way to setup is from Web user interaction
    html_form_overwrite_action & get_access_to_action_false() { return when_false; };

	/// obtain the crit_conditional_action object for libdar option
    virtual std::unique_ptr<libdar::crit_action> get_overwriting_action() const override;

	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    html_form_overwrite_combining_criterium condition;
    html_form_overwrite_action when_true;
    html_form_overwrite_action when_false;

    static constexpr const char* act_true_changed = "true changed";
    static constexpr const char* act_false_changed = "false changed";

    static constexpr const char* action_type_undefined = "undefined";
    static constexpr const char* action_type_const = "constant";
    static constexpr const char* action_type_condition = "condition";

    static constexpr const char* css_cond = "html_form_overwrite_cond_act";

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_form_overwrite_conditional_action";

    static constexpr const char* jlabel_condition = "condition";
    static constexpr const char* jlabel_when_true = "when-true";
    static constexpr const char* jlabel_when_false = "when-false";

};

#endif
