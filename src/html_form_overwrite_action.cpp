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
#include "html_form_overwrite_conditional_action.hpp"
#include "html_form_overwrite_chain_action.hpp"

    //
#include "html_form_overwrite_action.hpp"

using namespace std;

html_form_overwrite_action::html_form_overwrite_action(const string & label):
    fs(label),
    action_type("Action type",  act_changed)
{

	// components setup
    action_type.add_choice(action_type_const, "constant action");
    action_type.add_choice(action_type_condition, "conditional action");
    action_type.add_choice(action_type_chain, "chain action");
    action_type.set_selected(0);
    action.reset(new (nothrow) html_form_overwrite_constant_action());
    if(!action)
	throw exception_memory();
    old_action.reset();

	// adoption tree
    fs.adopt(&action_type);
    fs.adopt(&(*action));
    adopt(&fs);

	// events
    action_type.record_actor_on_event(this, act_changed);

	// css
}

unique_ptr<libdar::crit_action> html_form_overwrite_action::get_overwriting_action() const
{
    unique_ptr<libdar::crit_action> ret;

    if(!action)
	throw WEBDAR_BUG;

    ret = action->get_overwriting_action();
    if(!ret)
	throw WEBDAR_BUG;

    return ret;
}

void html_form_overwrite_action::on_event(const std::string & event_name)
{
    if(event_name == act_changed)
    {
	if(old_action)
	    throw WEBDAR_BUG;
	if(changed)
	    throw WEBDAR_BUG;

	changed = true;
	old_action = std::move(action);

	if(action_type.get_selected_id() == action_type_const)
	{
	    action.reset(new (nothrow) html_form_overwrite_constant_action());
	    if(!action)
		throw exception_memory();
		// adoption of this new object will be done in inherited_get_body_part()
	}
	else if(action_type.get_selected_id() == action_type_condition)
	{
	    action.reset(new (nothrow) html_form_overwrite_conditional_action());
	    if(!action)
		throw exception_memory();
		// adoption of this new object will be done in inherited_get_body_part()
	}
	else if(action_type.get_selected_id() == action_type_chain)
	{
	    action.reset(new (nothrow) html_form_overwrite_chain_action("Chain of actions"));
	    if(!action)
		throw exception_memory();
	}
	else // unknown action type
	    throw WEBDAR_BUG;
    }
    else // unexpected event name
	throw WEBDAR_BUG;
}

unique_ptr<body_builder> html_form_overwrite_action::make_brother() const
{
    std::unique_ptr<body_builder> ret(new (std::nothrow) html_form_overwrite_action(fs.get_label()));

    if(!ret)
	throw exception_memory();
    return ret;
}

string html_form_overwrite_action::inherited_get_body_part(const chemin & path,
							   const request & req)
{
    string ret;

    changed = false;

    ret = get_body_part_from_all_children(path, req);

    if(changed)
    {
	old_action.reset();
	if(action)
	    fs.adopt(&(*action));
	else
	    throw WEBDAR_BUG;
    }

    return ret;
}
