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
#include "html_form_overwrite_constant_action.hpp"


    //
#include "html_form_overwrite_conditional_action.hpp"

using namespace std;

const string html_form_overwrite_conditional_action::changed = "hfo_cond_action_changed";

html_form_overwrite_conditional_action::html_form_overwrite_conditional_action():
    when_true("If condition is true"),
    when_false("If condition is false")
{

	// components setup

	// adoption tree

    adopt(&condition);
    adopt(&when_true);
    adopt(&when_false);

	// events
    condition.record_actor_on_event(this, html_form_overwrite_combining_criterium::changed);
    when_true.record_actor_on_event(this, html_form_overwrite_action::changed);
    when_false.record_actor_on_event(this, html_form_overwrite_action::changed);
    register_name(changed);

	// css
    when_true.add_css_class(css_cond);
    when_false.add_css_class(css_cond);
}

unique_ptr<libdar::crit_action> html_form_overwrite_conditional_action::get_overwriting_action() const
{
    unique_ptr<libdar::crit_action> ret;
    unique_ptr<libdar::criterium> crit;
    unique_ptr<libdar::crit_action> if_true, if_false;

    crit = condition.get_overwriting_criterium();
    if(!crit)
	throw WEBDAR_BUG;

    if_true = when_true.get_overwriting_action();
    if(!if_true)
	throw WEBDAR_BUG;

    if_false = when_false.get_overwriting_action();
    if(!if_false)
	throw WEBDAR_BUG;

    ret.reset(new (nothrow) libdar::testing(*crit, *if_true, *if_false));
    if(!ret)
	throw exception_memory();

    return ret;
}

void html_form_overwrite_conditional_action::load_json(const json & source)
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

	condition.load_json(config.at(jlabel_condition));
	when_true.load_json(config.at(jlabel_when_true));
	when_false.load_json(config.at(jlabel_when_false));
    }
    catch(json::exception & e)
    {
	throw exception_json(libdar::tools_printf("Error loading %s config", myclass_id), e);
    }
}

json html_form_overwrite_conditional_action::save_json() const
{
    json config;

    config[jlabel_condition] = condition.save_json();
    config[jlabel_when_true] = when_true.save_json();
    config[jlabel_when_false] = when_false.save_json();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					config);
}

void html_form_overwrite_conditional_action::clear_json()
{
    condition.clear_json();
    when_true.clear_json();
    when_false.clear_json();
}

void html_form_overwrite_conditional_action::on_event(const std::string & event_name)
{
    if(event_name == html_form_overwrite_combining_criterium::changed
       || event_name == html_form_overwrite_action::changed)
	act(changed);
    else
	throw WEBDAR_BUG;
}

void html_form_overwrite_conditional_action::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    if(! csslib->class_exists(css_cond))
    {
	css tmp;
	tmp.css_float(css::fl_left);
	tmp.css_width("50%", false);
	tmp.css_overflow_x(css::ov_auto);
	csslib->add(css_cond, tmp);
    }
}
