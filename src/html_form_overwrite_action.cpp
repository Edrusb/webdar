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
#include "my_config.h"
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_form_overwrite_conditional_action.hpp"

    //
#include "html_form_overwrite_action.hpp"

using namespace std;

html_form_overwrite_action::html_form_overwrite_action(const string & label):
    fs(label),
    chain_action("Chain of actions"),
    action_type("Action type",  act_changed),
    need_conditional_action(false)
{

	// components setup

	// conditional_action cannot be assigned right now
	// to avoid endless loop constructing an html_form_conditional_action
	// which in turn constructs two html_form_overwrite_action and so on.

    action_type.add_choice(action_type_const, "constant action");
    action_type.add_choice(action_type_condition, "conditional action");
    action_type.add_choice(action_type_chain, "chain action");
    action_type.set_selected_num(0);

    set_visible();

	// adoption tree
    fs.adopt(&action_type);
    fs.adopt(&constant_action);
    fs.adopt(&chain_action);
    adopt(&fs);

	// events
    action_type.record_actor_on_event(this, act_changed);

	// css
}

unique_ptr<libdar::crit_action> html_form_overwrite_action::get_overwriting_action() const
{
    unique_ptr<libdar::crit_action> ret;

    switch(action_type.get_selected_num())
    {
    case 0:
	ret = constant_action.get_overwriting_action();
	break;
    case 1:
	if(!conditional_action)
	    throw WEBDAR_BUG;
	ret = conditional_action->get_overwriting_action();
	break;
    case 2:
	ret = chain_action.get_overwriting_action();
	break;
    default:
	throw WEBDAR_BUG;
    }

    if(!ret)
	throw WEBDAR_BUG;

    return ret;
}

void html_form_overwrite_action::on_event(const std::string & event_name)
{
    if(event_name == act_changed)
    {
	if(action_type.get_selected_num() == 1)
	{
	    if(!conditional_action)
		need_conditional_action = true;
	}

	set_visible();
    }
    else // unexpected event name
	throw WEBDAR_BUG;
}

string html_form_overwrite_action::inherited_get_body_part(const chemin & path,
							   const request & req)
{
    string ret;

    ret = html_div::inherited_get_body_part(path, req);

    if(need_conditional_action)
    {
	make_conditional_action();
	need_conditional_action = false;
	my_body_part_has_changed();
    }

    return ret;
}

void html_form_overwrite_action::make_conditional_action()
{
    if(!conditional_action)
    {
	conditional_action.reset(new (nothrow) html_form_overwrite_conditional_action());
	if(!conditional_action)
	    throw exception_memory();

	fs.adopt(conditional_action.get());
    }
}

void html_form_overwrite_action::set_visible()
{
    switch(action_type.get_selected_num())
    {
    case 0: // constant action
	constant_action.set_visible(true);
	if(conditional_action)
	    conditional_action->set_visible(false);
	chain_action.set_visible(false);
	break;
    case 1: // conditional action
	constant_action.set_visible(false);
	if(conditional_action)
	    conditional_action->set_visible(true);
	else
	{
	    if(!need_conditional_action)
		throw WEBDAR_BUG;
	}
	chain_action.set_visible(false);
	break;
    case 2: // chain action
	constant_action.set_visible(false);
	if(conditional_action)
	    conditional_action->set_visible(false);
	chain_action.set_visible(true);
	break;
    default:
	throw WEBDAR_BUG;
    }
}

