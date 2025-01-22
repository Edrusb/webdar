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
#include "webdar_css_style.hpp"



    //
#include "html_form_mask_expression.hpp"

using namespace std;

html_form_mask_expression::html_form_mask_expression(const string & subject):
    sujet(subject),
    fs(""),
    mask_type("Mask Type"), // see clear()
    negate("Negate",
	   html_form_input::check,
	   "", // unckecked (see clear())
	   "1"),
    casesensitivity("Case Sensitive",
		    html_form_input::check,
		    "1", // checked (see clear())
		    "1"),
    mask_expression("Mask Expression",
		    html_form_input::text,
		    "",   // (see clear())
		    "80%")
{

	// component configuration

    mask_type.add_choice("0", "Glob Expression");
    mask_type.add_choice("1", "Regular Expression");

    init();
}

html_form_mask_expression::html_form_mask_expression(const html_form_mask_expression & ref):
    html_mask(ref), // parent class
    sujet(ref.sujet),
    fs(ref.fs),
    mask_type(ref.mask_type),
    negate(ref.negate),
    casesensitivity(ref.casesensitivity),
    mask_expression(ref.mask_expression)
{
    init();
}

void html_form_mask_expression::clear()
{
    mask_type.set_selected(0);
    negate.set_value_as_bool(false);
    casesensitivity.set_value_as_bool(true);
    mask_expression.set_value("");
}

unique_ptr<libdar::mask> html_form_mask_expression::get_mask() const
{
    bool neg = negate.get_value_as_bool();
    bool casesensit = casesensitivity.get_value_as_bool();
    unique_ptr<libdar::mask> ret;

    switch(mask_type.get_selected_num())
    {
    case 0: // Glob expr.
	ret.reset(new (nothrow) libdar::simple_mask(mask_expression.get_value(),
						    casesensit));
	break;
    case 1: // Regex
	ret.reset(new (nothrow) libdar::regular_mask(mask_expression.get_value(),
						     casesensit));
	break;
    default:
	throw WEBDAR_BUG;
    }

    if(!ret)
	throw exception_memory();

    if(neg)
    {
	unique_ptr<libdar::mask> tmp(new (nothrow) libdar::not_mask(*ret));
	if(tmp)
	{
	    ret.swap(tmp);
	    tmp.reset();
	}
	else
	    throw exception_memory();
    }

    return ret;
}

void html_form_mask_expression::on_event(const std::string & event_name)
{
    if(event_name == html_form_select::changed
       || event_name == html_form_input::changed)
	fs.change_label(tell_action());
    else
	throw WEBDAR_BUG;
}

void html_form_mask_expression::load_json(const json & source)
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

	mask_type.set_selected_id(config.at(jlabel_mask_type));
	negate.set_value_as_bool(config.at(jlabel_negate));
	casesensitivity.set_value_as_bool(config.at(jlabel_casesensit));
	mask_expression.set_value(config.at(jlabel_expression));
    }
    catch(json::exception & e)
    {
	throw exception_json(libdar::tools_printf("Error loading %s config",
						  myclass_id),
			     e);
    }
}


json html_form_mask_expression::save_json() const
{
    json ret;

    ret[jlabel_mask_type] = mask_type.get_selected_id();
    ret[jlabel_negate] = negate.get_value_as_bool();
    ret[jlabel_casesensit] = casesensitivity.get_value_as_bool();
    ret[jlabel_expression] = mask_expression.get_value();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					ret);
}

string html_form_mask_expression::inherited_get_body_part(const chemin & path,
						   const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_form_mask_expression::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}


void html_form_mask_expression::init()
{
    fs.change_label(tell_action());

	// adoption tree

    fs.adopt(&mask_type);
    fs.adopt(&negate);
    fs.adopt(&casesensitivity);
    fs.adopt(&mask_expression);
    adopt(&fs);


	// events
    mask_type.record_actor_on_event(this, html_form_select::changed);
    negate.record_actor_on_event(this, html_form_input::changed);
    casesensitivity.record_actor_on_event(this, html_form_input::changed);
    mask_expression.record_actor_on_event(this, html_form_input::changed);

	// visibity

	// css stuff
    fs.add_label_css_class(webdar_css_style::wcs_bold_text);

    clear();
}

string html_form_mask_expression::tell_action() const
{
    string ret;

    ret = sujet + " ";

    if(negate.get_value_as_bool())
	ret += "does not match ";
    else
	ret += "matches ";

    switch(mask_type.get_selected_num())
    {
    case 0:
	ret += "the glob expression ";
	break;
    case 1:
	ret += "the regular expression ";
	break;
    default:
	throw WEBDAR_BUG;
    }

    ret += "\"" + mask_expression.get_value() + "\" ";

    if(casesensitivity.get_value_as_bool())
	ret += "(case-sensitive)";
    else
	ret += "(case-INsensitive)";

    return ret;
}
