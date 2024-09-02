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

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_form_overwrite_constant_action.hpp"


    //
#include "html_form_overwrite_conditional_action.hpp"

using namespace std;

html_form_overwrite_conditional_action::html_form_overwrite_conditional_action():
    true_block("If condition is true"),
    action_type_true("Action type",  act_true_changed),
    false_block("If condition is false"),
    action_type_false("Action type", act_false_changed)
{

	// components setup
    action_type_true.add_choice(action_type_undefined, "--- select an action type ---");
    action_type_true.add_choice(action_type_const, "constant action");
    action_type_true.add_choice(action_type_condition, "conditional action");
    act_if_true.reset();

    action_type_false.add_choice(action_type_undefined, "--- select an action type ---");
    action_type_false.add_choice(action_type_const, "constant action");
    action_type_false.add_choice(action_type_condition, "conditional action");
    act_if_false.reset();

	// adoption tree

    adopt(&condition);

    true_block.adopt(&action_type_true);
    adopt(&true_block);

    false_block.adopt(&action_type_false);
    adopt(&false_block);

	// events

    action_type_true.record_actor_on_event(this, act_true_changed);
    action_type_false.record_actor_on_event(this, act_false_changed);


	// css
}

unique_ptr<libdar::crit_action> html_form_overwrite_conditional_action::get_overwriting_action() const
{
    unique_ptr<libdar::crit_action> ret;
    unique_ptr<libdar::criterium> crit;
    unique_ptr<libdar::crit_action> when_true, when_false;

    if(!act_if_true)
	throw exception_range("No action set when the overwriting policy returns true");
    if(!act_if_false)
	throw exception_range("No action set when the overwriting policy returns true");

    crit = condition.get_overwriting_criterium();
    if(!crit)
	throw WEBDAR_BUG;

    when_true = act_if_true->get_overwriting_action();
    if(!when_true)
	throw WEBDAR_BUG;

    when_false = act_if_false->get_overwriting_action();
    if(!when_false)
	throw WEBDAR_BUG;

    ret.reset(new (nothrow) libdar::testing(*crit, *when_true, *when_false));
    if(!ret)
	throw exception_memory();

    return ret;
}

void html_form_overwrite_conditional_action::on_event(const std::string & event_name)
{
    if(event_name == act_true_changed)
    {
	if(old_true)
	    throw WEBDAR_BUG;
	if(change_true)
	    throw WEBDAR_BUG;

	change_true = true;
	old_true = std::move(act_if_true);

	if(action_type_true.get_selected_id() == action_type_undefined)
	{
		// nothing to do
	}
	else if(action_type_true.get_selected_id() == action_type_const)
	{
	    act_if_true.reset(new (nothrow) html_form_overwrite_constant_action());
	    if(!act_if_true)
		throw exception_memory();
		// adoption of this new object will be done in inherited_get_body_part()
	}
	else if(action_type_true.get_selected_id() == action_type_condition)
	{
	    act_if_true.reset(new (nothrow) html_form_overwrite_conditional_action());
	    if(!act_if_true)
		throw exception_memory();
		// adoption of this new object will be done in inherited_get_body_part()
	}
	else
	    throw WEBDAR_BUG;
    }
    else if(event_name == act_false_changed)
    {
	if(old_false)
	    throw WEBDAR_BUG;
	if(change_false)
	    throw WEBDAR_BUG;

	change_false = true;
	old_false = std::move(act_if_false);

	if(action_type_false.get_selected_id() == action_type_undefined)
	{
		// nothing to do
	}
	else if(action_type_false.get_selected_id() == action_type_const)
	{
	    act_if_false.reset(new (nothrow) html_form_overwrite_constant_action());
	    if(!act_if_false)
		throw exception_memory();
		// adoption of this new object will be done in inherited_get_body_part()
	}
	else if(action_type_false.get_selected_id() == action_type_condition)
	{
	    act_if_false.reset(new (nothrow) html_form_overwrite_conditional_action());
	    if(!act_if_false)
		throw exception_memory();
		// adoption of this new object will be done in inherited_get_body_part()
	}
	else
	    throw WEBDAR_BUG;
    }
    else
	throw WEBDAR_BUG;
}

string html_form_overwrite_conditional_action::inherited_get_body_part(const chemin & path,
								       const request & req)
{
    string ret;

    change_true = false;
    change_false = false;

    ret = get_body_part_from_all_children(path, req);

    if(change_true)
    {
	old_true.reset();
	if(act_if_true)
	    true_block.adopt(&(*act_if_true));
    }

    if(change_false)
    {
	old_false.reset();
	if(act_if_false)
	    false_block.adopt(&(*act_if_false));
    }

    return ret;
}

