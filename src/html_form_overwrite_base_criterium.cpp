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

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/tools.hpp>

    // webdar headers



    //
#include "html_form_overwrite_base_criterium.hpp"

using namespace std;

const string html_form_overwrite_base_criterium::changed = "hfobc_changed";

html_form_overwrite_base_criterium::html_form_overwrite_base_criterium():
    crit_fs(""),
    negate("Not", html_form_input::check, "", "", ""),
    invert("Invert roles", html_form_input::check, "", "", ""),
    base("Criterium"),
    date("Date"),
    hourshift("Hourshift", html_form_input::number, "0", "5", ""),
    trigger_change(false)

{

	// components setup
    base.add_choice(crit_in_place_is_inode, crit_in_place_is_inode);
    base.add_choice(crit_in_place_is_dir, crit_in_place_is_dir);
    base.add_choice(crit_in_place_is_file, crit_in_place_is_file);
    base.add_choice(crit_in_place_is_hardlinked_inode, crit_in_place_is_hardlinked_inode);
    base.add_choice(crit_in_place_is_new_hardlinked_inode, crit_in_place_is_new_hardlinked_inode);
    base.add_choice(crit_in_place_data_more_recent, crit_in_place_data_more_recent);
    base.add_choice(crit_in_place_data_more_recent_or_equal_to, crit_in_place_data_more_recent_or_equal_to);
    base.add_choice(crit_in_place_data_bigger, crit_in_place_data_bigger);
    base.add_choice(crit_in_place_data_saved, crit_in_place_data_saved);
    base.add_choice(crit_in_place_data_dirty, crit_in_place_data_dirty);
    base.add_choice(crit_in_place_data_sparse, crit_in_place_data_sparse);
    base.add_choice(crit_in_place_has_delta_sig, crit_in_place_has_delta_sig);
    base.add_choice(crit_same_inode_data, crit_same_inode_data);
    base.add_choice(crit_in_place_EA_present, crit_in_place_EA_present);
    base.add_choice(crit_in_place_EA_more_recent, crit_in_place_EA_more_recent);
    base.add_choice(crit_in_place_EA_more_recent_or_equal_to, crit_in_place_EA_more_recent_or_equal_to);
    base.add_choice(crit_in_place_more_EA, crit_in_place_more_EA);
    base.add_choice(crit_in_place_EA_bigger, crit_in_place_EA_bigger);
    base.add_choice(crit_in_place_EA_saved, crit_in_place_EA_saved);
    base.add_choice(crit_same_type, crit_same_type);

    hourshift.set_range(0, 24);

	// adoption tree
    crit_fs.adopt(&negate);
    crit_fs.adopt(&invert);
    crit_fs.adopt(&base);
    crit_fs.adopt(&date);
    crit_fs.adopt(&hourshift);
    adopt(&crit_fs);

	// events
    register_name(changed);
    negate.record_actor_on_event(this, html_form_input::changed);
    invert.record_actor_on_event(this, html_form_input::changed);
    base.record_actor_on_event(this, html_form_select::changed);
    date.record_actor_on_event(this, html_datetime::changed);
    hourshift.record_actor_on_event(this, html_form_input::changed);

    	// set default visibility
    on_event(html_form_select::changed);

	// css

	// tooltips
    negate.set_tooltip("Take the opposite result of the selected criterium, for example if the selected criterium is \"in place is an inode\" negating it will result in \"in place is *not* an inode\"");
    invert.set_tooltip("Invert the roles of the \"in-place\" entry and the \"to be added\" one, for example if the selected criterium is \"in-place is an inode\" inverting will result in \"*to be added* is an inode\"");
    base.set_tooltip("Define what action to do when the same entry is found in the \"in-place\" archive and in the \"to be added\" one");
}

unique_ptr<libdar::criterium> html_form_overwrite_base_criterium::get_overwriting_criterium() const
{
    unique_ptr<libdar::criterium> ret;

    if(base.get_selected_id() == crit_in_place_is_inode)
	ret.reset(new (nothrow) libdar::crit_in_place_is_inode());
    else if(base.get_selected_id() == crit_in_place_is_dir)
	ret.reset(new (nothrow) libdar::crit_in_place_is_dir());
    else if(base.get_selected_id() == crit_in_place_is_file)
	ret.reset(new (nothrow) libdar::crit_in_place_is_file());
    else if(base.get_selected_id() == crit_in_place_is_hardlinked_inode)
	ret.reset(new (nothrow) libdar::crit_in_place_is_hardlinked_inode());
    else if(base.get_selected_id() == crit_in_place_is_new_hardlinked_inode)
	ret.reset(new (nothrow) libdar::crit_in_place_is_new_hardlinked_inode());
    else if(base.get_selected_id() == crit_in_place_data_more_recent)
	ret.reset(new (nothrow) libdar::crit_in_place_data_more_recent(libdar::deci(hourshift.get_value()).computer()));
    else if(base.get_selected_id() == crit_in_place_data_more_recent_or_equal_to)
	ret.reset(new (nothrow) libdar::crit_in_place_data_more_recent_or_equal_to(date.get_value(),
										   libdar::deci(hourshift.get_value()).computer()));
    else if(base.get_selected_id() == crit_in_place_data_bigger)
	ret.reset(new (nothrow) libdar::crit_in_place_data_bigger());
    else if(base.get_selected_id() == crit_in_place_data_saved)
	ret.reset(new (nothrow) libdar::crit_in_place_data_saved());
    else if(base.get_selected_id() == crit_in_place_data_dirty)
	ret.reset(new (nothrow) libdar::crit_in_place_data_dirty());
    else if(base.get_selected_id() == crit_in_place_data_sparse)
	ret.reset(new (nothrow) libdar::crit_in_place_data_sparse());
    else if(base.get_selected_id() == crit_in_place_has_delta_sig)
	ret.reset(new (nothrow) libdar::crit_in_place_has_delta_sig());
    else if(base.get_selected_id() == crit_same_inode_data)
	ret.reset(new (nothrow) libdar::crit_same_inode_data());
    else if(base.get_selected_id() == crit_in_place_EA_present)
	ret.reset(new (nothrow) libdar::crit_in_place_EA_present());
    else if(base.get_selected_id() == crit_in_place_EA_more_recent)
	ret.reset(new (nothrow) libdar::crit_in_place_EA_more_recent(libdar::deci(hourshift.get_value()).computer()));
    else if(base.get_selected_id() == crit_in_place_EA_more_recent_or_equal_to)
	ret.reset(new (nothrow) libdar::crit_in_place_EA_more_recent_or_equal_to(date.get_value(),
										 libdar::deci(hourshift.get_value()).computer()));
    else if(base.get_selected_id() == crit_in_place_more_EA)
	ret.reset(new (nothrow) libdar::crit_in_place_more_EA());
    else if(base.get_selected_id() == crit_in_place_EA_bigger)
	ret.reset(new (nothrow) libdar::crit_in_place_EA_bigger());
    else if(base.get_selected_id() == crit_in_place_EA_saved)
	ret.reset(new (nothrow) libdar::crit_in_place_EA_saved());
    else if(base.get_selected_id() == crit_same_type)
	ret.reset(new (nothrow) libdar::crit_same_type());
    else
	throw WEBDAR_BUG;

    if(!ret)
	throw exception_memory();

    if(invert.get_value_as_bool()) // "Invert role" checkbox is checked
    {
	ret.reset(new (nothrow) libdar::crit_invert(*ret));
	if(!ret)
	    throw exception_memory();
    }

    if(negate.get_value_as_bool()) // "Not" checkbox is checked
    {
	ret.reset(new (nothrow) libdar::crit_not(*ret));
	if(!ret)
	    throw exception_memory();
    }

    return ret;
}

void html_form_overwrite_base_criterium::load_json(const json & source)
{
    trigger_change = false;

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

	negate.set_value_as_bool(config.at(jlabel_negate));
	invert.set_value_as_bool(config.at(jlabel_invert));
	base.set_selected_id_with_warning(config.at(jlabel_base), jlabel_base);
	date.set_value(libdar::deci(config.at(jlabel_date)).computer());
	hourshift.set_value_as_int(config.at(jlabel_hourshift));
    }
    catch(json::exception & e)
    {
	throw exception_json(libdar::tools_printf("Error loading %s config", myclass_id), e);
    }

    if(trigger_change)
	act(changed);
}

json html_form_overwrite_base_criterium::save_json() const
{
    json config;

    config[jlabel_negate] = negate.get_value_as_bool();
    config[jlabel_invert] = invert.get_value_as_bool();
    config[jlabel_base] = base.get_selected_id();
    config[jlabel_date] = libdar::deci(date.get_value()).human();
    config[jlabel_hourshift] = hourshift.get_value_as_int();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					config);
}

void html_form_overwrite_base_criterium::clear_json()
{
    trigger_change = false;

    negate.set_value_as_bool(false);
    invert.set_value_as_bool(false);
    base.set_selected_num(0);
    date.set_value(0);
    hourshift.set_value_as_int(0);

    if(trigger_change)
	act(changed);
}

void html_form_overwrite_base_criterium::on_event(const string & event_name)
{
    if(event_name == html_form_select::changed)
    {
	if(base.get_selected_id() == "crit_in_place_data_more_recent_or_equal_to" ||
	   base.get_selected_id() == "crit_in_place_EA_more_recent_or_equal_to")
	{
	    date.set_visible(true);
	    hourshift.set_visible(true);
	}
	else
	{
	    date.set_visible(false);
	    hourshift.set_visible(false);
	}
	trigger_change = true;
    }
    else if(event_name == html_form_input::changed
	    || event_name == html_datetime::changed)
    {
	trigger_change = true;
    }
    else
	throw WEBDAR_BUG;
}

string html_form_overwrite_base_criterium::inherited_get_body_part(const chemin & path,
								   const request & req)
{
    string ret;

    trigger_change = false;

    ret = get_body_part_from_all_children(path, req);

    if(trigger_change)
	act(changed);

    return ret;
}

