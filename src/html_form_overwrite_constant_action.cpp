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
#include <dar/tools.hpp>

    // webdar headers



    //
#include "html_form_overwrite_constant_action.hpp"

using namespace std;

html_form_overwrite_constant_action::html_form_overwrite_constant_action():
    action_fs(""),
    data_action("Policy for Data"),
    ea_action("Policy for EA and FSA")
{

	// components setup

    data_action.add_choice("0", "Preserve");
    data_action.add_choice("1", "Overwrite");
    data_action.add_choice("2", "Preserve and mark already saved");
    data_action.add_choice("3", "Overwrite and mark already saved");
    data_action.add_choice("4", "Remove");
    data_action.add_choice("5", "Keep undefined if not already defined");
    data_action.add_choice("6", "Ask user what to do for each file in conflict");

    ea_action.add_choice("0", "Preserve");
    ea_action.add_choice("1", "Overwrite");
    ea_action.add_choice("2", "Clear all EA and FSA");
    ea_action.add_choice("3", "Preserve and mark already saved");
    ea_action.add_choice("4", "Overwrite and mark already saved");
    ea_action.add_choice("5", "Merge EA and preserve");
    ea_action.add_choice("6", "Merge EA and overwrite");
    ea_action.add_choice("7", "Keep undefined if not already defined");
    ea_action.add_choice("8", "Ask user what to do for each file in conflict");

	// adoption tree

    adopt(&data_action);
    adopt(&ea_action);

	// events

	// css

}


unique_ptr<libdar::crit_action> html_form_overwrite_constant_action::get_overwriting_action() const
{
    unique_ptr<libdar::crit_action> ret;
    libdar::over_action_data act_data;
    libdar::over_action_ea act_ea;

    switch(data_action.get_selected_num())
    {
    case 0:
	act_data = libdar::data_preserve;
	break;
    case 1:
	act_data = libdar::data_overwrite;
	break;
    case 2:
	act_data = libdar::data_preserve_mark_already_saved;
	break;
    case 3:
	act_data = libdar::data_overwrite_mark_already_saved;
	break;
    case 4:
	act_data = libdar::data_remove;
	break;
    case 5:
	act_data = libdar::data_undefined;
	break;
    case 6:
	act_data = libdar::data_ask;
	break;
    default:
	throw WEBDAR_BUG;
    }

    switch(ea_action.get_selected_num())
    {
    case 0:
	act_ea = libdar::EA_preserve;
	break;
    case 1:
	act_ea = libdar::EA_overwrite;
	break;
    case 2:
	act_ea = libdar::EA_clear;
	break;
    case 3:
	act_ea = libdar::EA_preserve_mark_already_saved;
	break;
    case 4:
	act_ea = libdar::EA_overwrite_mark_already_saved;
	break;
    case 5:
	act_ea = libdar::EA_merge_preserve;
	break;
    case 6:
	act_ea = libdar::EA_merge_overwrite;
	break;
    case 7:
	act_ea = libdar::EA_undefined;
	break;
    case 8:
	act_ea = libdar::EA_ask;
	break;
    default:
	throw WEBDAR_BUG;
    }

    ret.reset(new (nothrow) libdar::crit_constant_action(act_data, act_ea));
    if(!ret)
	throw exception_memory();
    return ret;
}

void html_form_overwrite_constant_action::load_json(const json & source)
{
    try
    {
	unsigned int version;
	string class_id;
	json config = unwrap_config_from_json_header(source,
						     version,
						     class_id);

	if(class_id != myclass_id)
	    throw exception_range(libdar::tools_printf("Unexpected class_id in json data, found %s while expecting %s",
						       class_id.c_str(),
						       myclass_id));

	if(version > format_version)
	    throw exception_range(libdar::tools_printf("Json format version too hight for %s, upgrade your webdar software",
						       myclass_id));

	data_action.set_selected_id(config.at(jlabel_data));
	ea_action.set_selected_id(config.at(jlabel_ea));
    }
    catch(json::exception & e)
    {
	throw exception_json(libdar::tools_printf("Error loading %s config", myclass_id), e);
    }
}

json html_form_overwrite_constant_action::save_json() const
{
    json config;

    config[jlabel_data] = data_action.get_selected_id();
    config[jlabel_ea] = ea_action.get_selected_id();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					config);
}

void html_form_overwrite_constant_action::clear_json()
{
    data_action.set_selected_num(0);
    ea_action.set_selected_num(0);
}
