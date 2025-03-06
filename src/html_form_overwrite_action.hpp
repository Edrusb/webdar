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

#ifndef HTML_FORM_OVERWRITE_ACTION_HPP
#define HTML_FORM_OVERWRITE_ACTION_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_overwrite_action.hpp"
#include "html_div.hpp"
#include "html_form_select.hpp"
#include "html_form_fieldset.hpp"
#include "actor.hpp"
#include "html_form_overwrite_constant_action.hpp"
#include "html_form_overwrite_chain_action.hpp"
#include "jsoner.hpp"
#include "events.hpp"

    // cannot include header file as this one includes us, making a dependency loop
class html_form_overwrite_conditional_action;

    /// html component to be adopted by an html_form class

    /// \note this class implements the html_overwrite_action and proposes to
    /// the user a switch between html_form_overwrite_constant_action,
    /// html_form_overwrite_conditional_action and html_form_overwrite_chain_action
    /// which are more specific implementations of html_overwrite_action ancestor
    /// class that should also be adopted by an html_form (directly or not).


class html_form_overwrite_action: public html_overwrite_action,
				  public actor,
				  public jsoner,
				  public events
{
public:

	// events

    static const std::string changed;

	// actions type

    static constexpr const char* action_type_const = "constant";
    static constexpr const char* action_type_condition = "condition";
    static constexpr const char* action_type_chain = "chain";

    html_form_overwrite_action(const std::string & label);
    html_form_overwrite_action(const html_form_overwrite_action & ref) = delete;
    html_form_overwrite_action(html_form_overwrite_action && ref) noexcept = delete;
    html_form_overwrite_action & operator = (const html_form_overwrite_action & ref) = delete;
    html_form_overwrite_action & operator = (html_form_overwrite_action && ref) noexcept = delete;
    ~html_form_overwrite_action() = default;

	/// programmatically define the action type

	/// \note this call is to programmatically setup a component, normal way to setup is from Web user interaction
    void set_action_type(const std::string & acttype) { action_type.set_selected_id(acttype); };

	/// get access to object storing constant action
    html_form_overwrite_constant_action & get_action_when_type_const() { return constant_action; };

	/// get access to object storing chain action
    html_form_overwrite_chain_action & get_action_when_type_chain() { return chain_action; };

	/// get access to object storing conditional action
    html_form_overwrite_conditional_action & get_action_when_type_condition();

	/// obtain the crit_action object for libdar option
    virtual std::unique_ptr<libdar::crit_action> get_overwriting_action() const override;

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

protected:

	/// inherited from body_builder from html_overwrite_action
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    static constexpr const char* act_changed = "action changed";

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_form_overwrite_action";

    static constexpr const char* jlabel_type = "type";
    static constexpr const char* jlabel_value = "value";

    bool need_conditional_action;

	// we use pointers for here
	// to avoid cyclic dependency between html_form_overwrite_action
	// and html_form_conditional_action
    std::unique_ptr<html_overwrite_action> conditional_action;

    html_form_fieldset fs;
    html_form_select action_type;
    html_form_overwrite_constant_action constant_action;
    html_form_overwrite_chain_action chain_action;

    void make_conditional_action();
    void set_visible();
};

#endif
