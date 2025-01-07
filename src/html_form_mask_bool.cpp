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
#include "html_form_mask_bool.hpp"

using namespace std;

html_form_mask_bool::html_form_mask_bool(const string & initial_mode):
    fs(""),
    mask_type("Combining with", bool_changed_event),
    table(true, true, "Add a new mask", "--- select a mask type ---"),
    root_prefix(libdar::FAKE_ROOT)
{

	// components configuration

    mask_type.add_choice(and_op, "AND");
    mask_type.add_choice(or_op, "OR");
    if(mask_type.num_choices() != 2)
	throw WEBDAR_BUG;
    mask_type.set_selected(initial_mode);
    current_bool_mode = mask_type.get_selected_id();
    fs.change_label(tell_action());

	// adoption tree

    fs.adopt(&mask_type);
    fs.adopt(&table);
    adopt(&fs);

	// events
    mask_type.record_actor_on_event(this, bool_changed_event);
    table.record_actor_on_event(this, html_form_dynamic_table::changed);

	// visibility


	// css stuff
    table.set_css_class_first_column(css_class_bool_text);
    fs.add_label_css_class(webdar_css_style::wcs_bold_text);

	// final table update
    update_table_content_logic(true); // true = update unconditionally
}

void html_form_mask_bool::set_root_prefix(const libdar::path & x_prefix)
{
    root_prefix = x_prefix; // record the value for use with future sub-mask
    propagate_root_prefix(); // propagate the new prefix to child masks
}

unique_ptr<libdar::mask> html_form_mask_bool::get_mask() const
{
    unique_ptr<libdar::mask> ret;
    bool and_mode;

    if(mask_type.get_selected_id() == and_op)
	and_mode = true;
    else if(mask_type.get_selected_id() == or_op)
	and_mode = false;
    else
	throw WEBDAR_BUG;

    if(table.empty())
	ret.reset(new (nothrow) libdar::bool_mask(true));
    else
    {
	html_form_dynamic_table::iterator it = table.begin();
	shared_ptr<body_builder> obj;
	html_mask* ptr = nullptr;
	unique_ptr<libdar::mask> toadd;
	unique_ptr<libdar::et_mask> tmp;


	if(and_mode)
	    tmp.reset(new (nothrow) libdar::et_mask());
	else
	    tmp.reset(new (nothrow) libdar::ou_mask());

	if(!tmp)
	    throw exception_memory();

	    // adding member to the mask

	while(it != table.end())
	{
	    obj = it.get_object();
	    if(!obj)
		throw WEBDAR_BUG;

	    ptr = dynamic_cast<html_mask*>(obj.get());
	    if(ptr == nullptr)
		throw WEBDAR_BUG;

	    toadd = ptr->get_mask();
	    if(!toadd)
		throw WEBDAR_BUG;

	    tmp->add_mask(*toadd);

	    ++it;
	}

	ret.reset(tmp.release());
    }

    if(!ret)
	throw exception_memory();

    return ret;
}

void html_form_mask_bool::on_event(const string & event_name)
{
    if(event_name == bool_changed_event)
	update_table_content_logic(false);
    else if(event_name == html_form_dynamic_table::changed)
    {
	update_table_content_logic(true);
	propagate_root_prefix();
    }
    else
	throw WEBDAR_BUG;

    fs.change_label(tell_action());
}

string html_form_mask_bool::invert_logic(const std::string & logic)
{
    if(logic == and_op)
	return or_op;
    else if(logic == or_op)
	return and_op;
    else
	throw WEBDAR_BUG;
}

void html_form_mask_bool::load_json(const json & source)
{
    try
    {
	unsigned int version;
	string class_id;
	json config = unwrap_config_from_json_header(source,
						     version,
						     class_id);

	if(class_id != "html_form_mask_bool")
	    throw exception_range(libdar::tools_printf("Unexpected class_id in json data, found %s while expecting html_form_mask_bool",
						       class_id.c_str()));

	if(version > format_version)
	    throw exception_range("Json format version too hight for html_form_mask_bool, upgrade your webdar software");

	    // setting back the bool mode and mask_type selected value

	current_bool_mode = config.at(jlabel_logic);
	mask_type.set_selected(current_bool_mode);

	    // setting back prefix

	root_prefix = libdar::path(config.at(jlabel_prefix));

	    // filling the table for each component found

	html_form_dynamic_table::iterator dynptr;
	jsoner* intable = nullptr;
	json components = config.at(jlabel_components);

	table.clear();

	if(! components.is_array() && ! components.is_null())
	    throw exception_range(libdar::tools_printf("Expecting table of components for label %s in %s json configuration",
						       jlabel_components,
						       class_id.c_str()));

	for(json::iterator it = components.begin();
	    it != components.end();
	    ++it)
	{
		// the following leads the object provider to create
		// a new line with the correct object type
	    table.add_line(it->at(jlabel_compo_type));

		// now we get acces to the just create object of the expected type
	    dynptr = table.last();
	    if(dynptr == table.end())
		throw WEBDAR_BUG;

	    intable = dynamic_cast<jsoner*>(dynptr.get_object().get());
	    if(intable == nullptr)
		throw WEBDAR_BUG;

		// restoring its configuration based on the json info found for it
	    intable->load_json(it->at(jlabel_compo_conf));
	}
    }
    catch(json::exception & e)
    {
	throw exception_json("Error loading html_form_mask_bool config", e);
    }
}

json html_form_mask_bool::save_json() const
{
    json ret;
    json tmp;
    jsoner* itjson = nullptr;

    ret[jlabel_logic] = current_bool_mode;
    ret[jlabel_prefix] = root_prefix.display();

    for(html_form_dynamic_table::iterator it = table.begin();
	it != table.end();
	++it)
    {
	tmp.clear();
	    // tmp will contain
	    // - type of compoent : value
	    // - config : configuration

	tmp[jlabel_compo_type] = it.get_object_type();

	itjson = dynamic_cast<jsoner*>(it.get_object().get());
	if(itjson == nullptr)
	    throw WEBDAR_BUG;
	tmp[jlabel_compo_conf] = itjson->save_json();

	    // jlabel_components is a list of components
	    // each with two fields : type and config
	ret[jlabel_components].push_back(tmp);
    }

    return wrap_config_with_json_header(format_version,
					"html_form_mask_bool",
					ret);
}

void html_form_mask_bool::clear_json()
{
    table.clear();
}

bibliotheque::using_set html_form_mask_bool::get_using_set() const
{
    bibliotheque::using_set ret;
    bibliotheque_subconfig* subconf = nullptr;

    for(html_form_dynamic_table::iterator it = table.begin();
	it != table.end();
	++it)
    {
	subconf = dynamic_cast<bibliotheque_subconfig*>(it.get_object().get());

	if(subconf != nullptr)
	    subconfig_add_to(ret, subconf->get_using_set());
    }

    return ret;
}

string html_form_mask_bool::inherited_get_body_part(const chemin & path,
						    const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_form_mask_bool::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    if(!csslib->class_exists(css_class_bool_text))
    {
	css tmp;

	tmp.css_text_h_align(css::al_right);
	tmp.css_text_v_align(css::al_top);
	tmp.css_font_weight_bold();

	csslib->add(css_class_bool_text, tmp);
    }

    webdar_css_style::update_library(*csslib);
}

string html_form_mask_bool::bool_op_to_name(const string & op)
{
    if(op == and_op)
	return "and";
    else if(op == or_op)
	return "or";
    else
	throw WEBDAR_BUG;
}

void html_form_mask_bool::update_table_content_logic(bool unconditionally)
{
    string target_bool_mode = mask_type.get_selected_id();

    if((target_bool_mode != current_bool_mode) || unconditionally)
    {
	string logic_text = bool_op_to_name(target_bool_mode);
	html_form_dynamic_table::iterator it = table.begin();

	    // first line is empty
	if(it != table.end())
	{
	    shared_ptr<html_text> label = it.get_left_label();
	    if(!label)
		throw WEBDAR_BUG;
	    label->clear();
	    ++it;
	}

	while(it != table.end())
	{
	    shared_ptr<html_text> label = it.get_left_label();
	    if(!label)
		throw WEBDAR_BUG;
	    label->clear();
	    label->add_text(0, logic_text);
	    ++it;
	}

	current_bool_mode = target_bool_mode;
	table.set_obj_type_context(current_bool_mode);
    }
}

void html_form_mask_bool::propagate_root_prefix()
{
    html_form_dynamic_table::iterator it = table.begin();
    shared_ptr<body_builder> obj;
    html_mask* ptr = nullptr;

    while(it != table.end())
    {
	obj = it.get_object();
	if(!obj)
	    throw WEBDAR_BUG;

	ptr = dynamic_cast<html_mask*>(obj.get());
	if(ptr == nullptr)
	    throw WEBDAR_BUG;

	ptr->set_root_prefix(root_prefix);

	++it;
    }
}

string html_form_mask_bool::tell_action() const
{
    string ret;

    if(table.empty())
	ret += "match everything";
    else
	ret += "invovled if...";

    return ret;
}
