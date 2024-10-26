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
					   const libdar::infinint & initial_value,
					   unsigned int size):
    ignore_events(false),
    no_act(false),
    field(label,
	  html_form_input::number,
	  "", // we will set the field value below
	  size),
    val(initial_value),
    min(0),
    max(0),
    modif_change("")
{

	// components configuration
    field.set_no_CR();
    set_field_val();

	// adoption tree
    adopt(&field);
    adopt(&unit_box);

	// events
    register_name(changed);
    field.record_actor_on_event(this, html_form_input::changed);
    unit_box.record_actor_on_event(this, html_size_unit::changed);

	// updating the value representation
    set_value_to_largest_unit();
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
	set_value_as_infinint(min);
	set_field_val();
    }
    if(val > max)
    {
	set_value_as_infinint(max);
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
	set_value_as_infinint(min);
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
	set_value_as_infinint(max);
	set_field_val();
    }
    set_field_min_max();
}

void html_form_input_unit::set_value_as_infinint(const libdar::infinint & x_val)
{
    val = x_val;
    set_value_to_largest_unit();
}

void html_form_input_unit::on_event(const std::string & event_name)
{
    if(ignore_events)
	return;

    if(event_name == html_form_input::changed)
    {
	val = libdar::infinint(field.get_value_as_int()) * unit_box.get_value();
	my_act();
    }
    else if(event_name == html_size_unit::changed)
    {
	    // reduce html_form_input value (and ignore the change event)
	no_act = true;
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
	    no_act = false;
	    throw;
	}
	no_act = false;

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
    field.set_value(libdar::deci(reduce_to_unit(val, unit_box.get_value())).human());
}

void html_form_input_unit::set_value_to_largest_unit()
{
    unsigned int best_base_index = unit_box.get_current_base_index();
    unsigned int best_power_index = unit_box.get_current_power_for_base_unit();
    libdar::infinint reduced_val = val;

    unsigned int base_index = unit_box.get_max_base_unit_index();
    do
    {
	unsigned int max_power = unit_box.get_max_power_for_base_unit(base_index);
	libdar::infinint base_unit = unit_box.get_base_unit_value(base_index);

	bool stop = false;
	unsigned int power = 1;
	libdar::infinint factor = base_unit;
	    // to avoid recomputing power of base_unit at reach round in the
	    // while loop that follows:

	while(!stop && val > factor && power <= max_power)
	{
	    if((val % factor).is_zero()) // val is an integer number of factor
	    {
		libdar::infinint newval = val / factor;

		if(newval < reduced_val) // this is the best unit reduction so far
		{
		    reduced_val = newval;
		    best_power_index = power;
		    best_base_index = base_index;
		}
	    }
	    else
		stop = true;
		// val is not divisible by factor (a power of base unit),
		// thus is is not divisible by further powers of the base unit.

	    if(!stop)
	    {
		++power;
		factor *= base_unit;
	    }
	}

	--base_index;
    }
    while(base_index > 0);

	// we dont change the value of the field, just its representation
	// thus we must not trigger any change event, though this does hopefully
	// not provent the body_builder change to be propagated from the adopted
	// objects for the web output to be refreshed accordingly to the new
	// representation:
    ignore_events = true;
    try
    {
	unit_box.set_unit_and_ratio_indexes(best_base_index, best_power_index);
    }
    catch(...)
    {
	ignore_events = false;
	throw;
    }
    ignore_events = false;

	// we remind the current val value
    reduced_val = val;

	// this will recompute val according to the new unit/base
	// because of the event generated by field, which we now
	// take care of:
    set_field_val();

	// checking whether the resulting val stays the same as it ought to be
    if(val != reduced_val)
	throw WEBDAR_BUG;
}

libdar::infinint html_form_input_unit::reduce_to_unit(const libdar::infinint & val, const libdar::infinint & unit)
{
    return val / unit;
}

libdar::infinint html_form_input_unit::reduce_to_unit_above(const libdar::infinint & val, const libdar::infinint & unit)
{
    libdar::infinint target = val / unit;

    while(target * unit < val)
	++target;

    return target;
}
