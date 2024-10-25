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
#include "webdar_tools.hpp"
#include "exceptions.hpp"


    //
#include "html_form_input_unit.hpp"

using namespace std;

const string html_form_input_unit::changed = "html_form_input_unit_changed";

html_form_input_unit::html_form_input_unit(const string & label,
				 const string & initial_value,
				 unsigned int size):
    ignore_input_event(false),
    field(label,
	  html_form_input::number,
	  initial_value,
	  size),
    val(0),
    min(0),
    max(0),
    modif_change("")
{

	// components configuration
    field.set_no_CR();

	// adoption tree
    adopt(&field);
    adopt(&unit_box);

	// events

    register_name(changed);
    field.record_actor_on_event(this, html_form_input::changed);
    unit_box.record_actor_on_event(this, html_size_unit::changed);
}

void html_form_input_unit::set_range(const libdar::infinint & x_min,
				     const libdar::infinint & x_max)
{
    if(x_max.is_zero())
	throw WEBDAR_BUG;
    if(x_max < x_min)
	throw WEBDAR_BUG;

    min = x_min;
    max = x_max;
    if(val < min)
    {
	val = min;
	set_field_val();
    }
    if(val > max)
    {
	val = max;
	set_field_val();
    }

    set_field_min_max();
}

void html_form_input_unit::set_min_only(const libdar::infinint & x_min)
{
    min = x_min;
    max = 0;
    if(val < min)
    {
	val = min;
	set_field_val();
    }
    set_field_min_max();
}

void html_form_input_unit::set_max_only(const libdar::infinint & x_max)
{
    min = 0;
    max = x_max;
    if(val > max)
    {
	val = max;
	set_field_val();
    }
    set_field_min_max();
}

void html_form_input_unit::set_value_as_infinint(const libdar::infinint & x_val)
{
    libdar::infinint tmp = x_val;
    val = x_val;

    tmp /= unit_box.get_value();
    field.set_value(libdar::deci(tmp).human());
	// this triggers the html_form_input::change event
	// which in turns will update the value of "val"
	// and trigger our own event
}

void html_form_input_unit::on_event(const std::string & event_name)
{
    if(event_name == html_form_input::changed)
    {
	val = libdar::infinint(field.get_value_as_int()) * unit_box.get_value();
	if(! ignore_input_event)
	    my_act();
    }
    else if(event_name == html_size_unit::changed)
    {
	    // reduce html_form_input value (and ignore the change event)
	ignore_input_event = true;
	try
	{
	    set_field_min_max();
	    val = libdar::infinint(field.get_value_as_int()) * unit_box.get_value();
	    if(val < min)
	    {
		val = min;
		set_field_val();
	    }
	    if(max != 0 && val > max)
	    {
		val = max;
		set_field_val();
	    }
	}
	catch(...)
	{
	    ignore_input_event = false;
	    throw;
	}
	ignore_input_event = false;

	my_act(); // acting only once even if we modified or not the "field" field
    }
}

string html_form_input_unit::inherited_get_body_part(const chemin & path,
						     const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_form_input_unit::css_classes_have_changed()
{
    css_class_group applied_to_me = get_css_class_group();

	// propagating our css class only to the "field" field
    field.clear_css_classes();
    field.add_css_class(applied_to_me);
}

void html_form_input_unit::set_change_event_name(const string & name)
{
    if(name == "")
	throw WEBDAR_BUG;
	// we expect non-empty string as valid event name

    if(modif_change.empty())
	modif_change = changed;

	// leveraging new featur from events class
	// all registered actors are kept:
    rename_name(modif_change, name);
    modif_change = name;

}

void html_form_input_unit::set_field_min_max()
{
    libdar::infinint unit_val = unit_box.get_value();

    if(max.is_zero()) // only min is set
	field.set_min_only(reduce_to_unit_above(min, unit_val));
    else
	field.set_range(reduce_to_unit_above(min, unit_val),
			reduce_to_unit_above(max, unit_val));
	// we use a set_range and not a set_max_only()
	// because we must forbid negative values, always.
}

void html_form_input_unit::set_field_val()
{
    field.set_value_as_int(reduce_to_unit(val, unit_box.get_value()));
}

int html_form_input_unit::reduce_to_unit(const libdar::infinint & val, const libdar::infinint & unit)
{
    libdar::infinint target = val / unit;
    int ret = 0;

    target.unstack(ret);
    if(! target.is_zero()) // too large for integer type, falling back to zero
	ret = 0;

    return ret;
}

int html_form_input_unit::reduce_to_unit_above(const libdar::infinint & val, const libdar::infinint & unit)
{
    libdar::infinint target = val / unit;
    int ret = 0;

    while(target * unit < val)
	++target;

    target.unstack(ret);
    if(! target.is_zero()) // too large for integer type, falling back to zero
	ret = 0;

    return ret;
}
