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
#include "html_slicing.hpp"

using namespace std;

const string html_slicing::changed = "html_slicing_changed";

html_slicing::html_slicing():
    form_slicing("Update"),
    slicing_fs(""),
    slicing("Sliced archive", html_form_input::check, "", "1"), // unchecked
    slice_size("Slice size", 0, "10"),
    different_first_slice("Specific size for first slice", html_form_input::check, "", "1"), // unchecked
    first_slice_size("Slice size", 0, "10"),
    slice_permission("Slice permission (octal)", html_form_input::text, "", "10"),
    slice_user_ownership("Slice user ownership", html_form_input::text, "", "80%"),
    slice_group_ownership("slice group ownership", html_form_input::text, "", "80%"),
    slice_min_digits("Minimum digits in slice file names", html_form_input::number, "0", "3")
{

	// component setup
    slice_size.set_min_only(60);
    first_slice_size.set_min_only(60);
    slice_min_digits.set_min_only(0);

	// adoption tree
    slicing_fs.adopt(&slice_permission);
    slicing_fs.adopt(&slice_user_ownership);
    slicing_fs.adopt(&slice_group_ownership);
    slicing_fs.adopt(&slice_min_digits);
    slicing_fs.adopt(&slicing);
    slicing_fs.adopt(&slice_size);
    slicing_fs.adopt(&different_first_slice);
    slicing_fs.adopt(&first_slice_size);
    form_slicing.adopt(&slicing_fs);
    adopt(&form_slicing);

	// events
    slicing.record_actor_on_event(this, html_form_input::changed);
    slice_size.record_actor_on_event(this, html_form_input_unit::changed);
    different_first_slice.record_actor_on_event(this, html_form_input::changed);
    first_slice_size.record_actor_on_event(this, html_form_input_unit::changed);
    slice_permission.record_actor_on_event(this, html_form_input::changed);
    slice_user_ownership.record_actor_on_event(this, html_form_input::changed);
    slice_group_ownership.record_actor_on_event(this, html_form_input::changed);
    slice_min_digits.record_actor_on_event(this, html_form_input::changed);
    register_name(changed);

	// css

	// visibility
    on_event(html_form_input::changed);
}


void html_slicing::set_slicing(const libdar::infinint & x_slice_size,
			       const libdar::infinint & x_first_slice_size)
{
    if(x_slice_size.is_zero())
	slicing.set_value_as_bool(false);
    else
    {
	slicing.set_value_as_bool(true);
	slice_size.set_value_as_infinint(x_slice_size);
	if(x_first_slice_size.is_zero())
	    different_first_slice.set_value_as_bool(false);
	else
	{
	    different_first_slice.set_value_as_bool(true);
	    first_slice_size.set_value_as_infinint(x_first_slice_size);
	}
    }
}

void html_slicing::set_permission(const std::string & perm)
{
    slice_permission.set_value(perm);
}

void html_slicing::set_user_ownership(const std::string & username)
{
    slice_user_ownership.set_value(username);
}

void html_slicing::set_group_ownership(const std::string & groupname)
{
    slice_group_ownership.set_value(groupname);
}

void html_slicing::set_min_digits(const libdar::infinint & val)
{
    slice_min_digits.set_value(libdar::deci(val).human());
}

void html_slicing::get_slicing(libdar::infinint & x_slice_size,
			       libdar::infinint & x_first_slice_size) const
{
    if(slicing.get_value_as_bool())
    {
	x_slice_size = slice_size.get_value_as_infinint();

	if(different_first_slice.get_value_as_bool())
	    x_first_slice_size = first_slice_size.get_value_as_infinint();
	else
	    x_first_slice_size = 0; // first slice has the same size as other slice size
    }
    else
    {
	    // no slicing (single file archive)
	x_slice_size = 0;
	x_first_slice_size = 0;
    }
}

libdar::infinint html_slicing::get_min_digits() const
{
    libdar::infinint ret;

    try
    {
	ret = libdar::deci(slice_min_digits.get_value()).computer();
    }
    catch(libdar::Edeci & e)
    {
	e.prepend_message("Error met while reading min digits value");
	throw exception_libcall(e);
    }

    return ret;
}

std::string html_slicing::get_permission() const
{
    return slice_permission.get_value();
}

std::string html_slicing::get_user_ownership() const
{
    return slice_user_ownership.get_value();
}

std::string html_slicing::get_group_ownership() const
{
    return slice_group_ownership.get_value();
}

void html_slicing::load_json(const json & source)
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
	    throw exception_range(libdar::tools_printf("Json format version too hight for %s, upgrade your webdar software", myclass_id));

	slicing.set_value_as_bool(config.at(jlabel_slicing));
	different_first_slice.set_value_as_bool(config.at(jlabel_diff_first));
	slice_size.set_value_as_infinint(libdar::deci(config.at(jlabel_slice_size)).computer());
	first_slice_size.set_value_as_infinint(libdar::deci(config.at(jlabel_first_slice_size)).computer());
	slice_permission.set_value(config.at(jlabel_slice_permission));
	slice_user_ownership.set_value(config.at(jlabel_user_owner));
	slice_group_ownership.set_value(config.at(jlabel_group_owner));
	slice_min_digits.set_value_as_int(config.at(jlabel_min_digits));

	on_event(html_form_input::changed);
    }
    catch(json::exception & e)
    {
	throw exception_json(libdar::tools_printf("Error loading %s config", myclass_id), e);
    }
}

json html_slicing::save_json() const
{
    json ret;

    ret[jlabel_slicing] = slicing.get_value_as_bool();
    ret[jlabel_diff_first] = different_first_slice.get_value_as_bool();
    ret[jlabel_slice_size] = libdar::deci(slice_size.get_value_as_infinint()).human();
    ret[jlabel_first_slice_size] = libdar::deci(first_slice_size.get_value_as_infinint()).human();
    ret[jlabel_slice_permission] = slice_permission.get_value();
    ret[jlabel_user_owner] = slice_user_ownership.get_value();
    ret[jlabel_group_owner] = slice_group_ownership.get_value();
    ret[jlabel_min_digits] = slice_min_digits.get_value_as_int();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					ret);
}

void html_slicing::clear_json()
{
    slicing.set_value_as_bool(false);
    slice_size.set_value_as_infinint(0);
    different_first_slice.set_value_as_bool(false);
    first_slice_size.set_value_as_infinint(0);
    slice_permission.set_value("");
    slice_user_ownership.set_value("");
    slice_group_ownership.set_value("");
    slice_min_digits.set_value_as_int(3);

    on_event(html_form_input::changed);
}


void html_slicing::on_event(const std::string & event_name)
{
    if(event_name == html_form_input::changed
       || event_name == html_form_input_unit::changed)
    {
	if(slicing.get_value_as_bool())
	{
	    slice_size.set_visible(true);
	    different_first_slice.set_visible(true);
	    if(different_first_slice.get_value_as_bool())
		first_slice_size.set_visible(true);
	    else
		first_slice_size.set_visible(false);
	}
	else // no slicing requested
	{
	    slice_size.set_visible(false);
	    different_first_slice.set_visible(false);
	    first_slice_size.set_visible(false);
	}

	act(changed);
    }
    else
	throw WEBDAR_BUG;
}


string html_slicing::inherited_get_body_part(const chemin & path,
					     const request & req)
{
    return get_body_part_from_all_children(path, req);
}
