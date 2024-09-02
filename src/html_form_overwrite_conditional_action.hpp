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

    /// html component used for constant actions in overwriting policies

class html_form_overwrite_conditional_action: public html_overwrite_action, public actor
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

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:

	/// inherited from body_builder from html_overwrite_action
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    static constexpr const char* act_true_changed = "true changed";
    static constexpr const char* act_false_changed = "false changed";

    static constexpr const char* action_type_undefined = "undefined";
    static constexpr const char* action_type_const = "constant";
    static constexpr const char* action_type_condition = "condition";

    html_form_overwrite_combining_criterium condition;
    html_form_fieldset true_block;
    html_form_select action_type_true;
    std::unique_ptr<html_overwrite_action> act_if_true;
    std::unique_ptr<html_overwrite_action> old_true; ///< to keep act_if_true existing when changed from on_event()
    bool change_true;
    html_form_fieldset false_block;
    html_form_select action_type_false;
    std::unique_ptr<html_overwrite_action> act_if_false;

	/// \note old_true and old_false are necessary
	/// for the body_builder code to find the object
	/// until the get_body_part_from_all_children()
	/// exits as called from our inherited_get_body_part()
	/// only after that we will delete the objects. Though
	/// they are just foresaken() if the act_if_true
	/// or act_if_false had to change
    std::unique_ptr<html_overwrite_action> old_false;  ///< to keep act_if_false existing when changed from on_event()
    bool change_false;

};

#endif
