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
#include "html_form_overwrite_base_criterium.hpp"


    //
#include "html_form_overwrite_combining_criterium.hpp"

using namespace std;

const string html_form_overwrite_combining_criterium::changed = "hfo_comb_crit_changed";

html_form_overwrite_combining_criterium::html_form_overwrite_combining_criterium(const string & initial_mode):
    fs(""),
    crit_type("Combining with", html_form_select::changed),
    table(true, true, "Add a new mask", " --- select a criterium --- ")
{

	// components configuration

    crit_type.add_choice(and_op, "AND");
    crit_type.add_choice(or_op, "OR");
    if(crit_type.num_choices() != 2)
	throw WEBDAR_BUG;
    crit_type.set_selected_id(initial_mode);
    current_bool_mode = crit_type.get_selected_id();

    table.set_obj_type_provider(this);
    table.add_obj_type("atomic criterium");   // this is index 0 in provide_object_of_type()
    table.add_obj_type("composit criterium"); // this is index 1 in provide_object_of_type()

	// adoption tree

    fs.adopt(&crit_type);
    fs.adopt(&table);
    adopt(&fs);

	// events
    crit_type.record_actor_on_event(this, html_form_select::changed);
    table.record_actor_on_event(this, html_form_dynamic_table::changed);
    register_name(changed);

	// visibility

	// css stuff
    table.set_css_class_first_column(css_class_bool_text);

	// final setup
    update_table_content_logic(true); // true = update unconditionally
}


unique_ptr<libdar::criterium> html_form_overwrite_combining_criterium::get_overwriting_criterium() const
{
    unique_ptr<libdar::crit_and> ret;

    if(current_bool_mode == and_op)
	ret.reset(new (nothrow) libdar::crit_and());
    else if(current_bool_mode == or_op)
	ret.reset(new (nothrow) libdar::crit_or());
    else
	throw WEBDAR_BUG;

    if(!ret)
	throw exception_memory();
    else
    {
	html_form_dynamic_table::iterator it = table.begin();

	while(it != table.end())
	{
	    std::shared_ptr<body_builder> tmp = it.get_object();
	    if(!tmp)
		throw WEBDAR_BUG;
	    html_overwrite_criterium* sub = dynamic_cast<html_overwrite_criterium*>(tmp.get());
	    if(sub == nullptr)
		throw WEBDAR_BUG;
	    unique_ptr<libdar::criterium> sub_crit = sub->get_overwriting_criterium();
	    if(!sub_crit)
		throw WEBDAR_BUG;

	    ret->add_crit(*sub_crit);

	    ++it;
	}
    }

    return ret;
}

void html_form_overwrite_combining_criterium::load_json(const json & source)
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

	crit_type.set_selected_id_with_warning(config.at(jlabel_logic), jlabel_logic);
	table.load_json(config.at(jlabel_contents));
    }
    catch(json::exception & e)
    {
	throw exception_json(libdar::tools_printf("Error loading %s config", myclass_id), e);
    }
}

json html_form_overwrite_combining_criterium::save_json() const
{
    json config;

    config[jlabel_logic] = crit_type.get_selected_id();
    config[jlabel_contents] = table.save_json();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					config);
}

void html_form_overwrite_combining_criterium::clear_json()
{
    crit_type.set_selected_num(0);
    table.clear_json();
}

void html_form_overwrite_combining_criterium::on_event(const std::string & event_name)
{
    if(event_name == html_form_select::changed)
	update_table_content_logic(false);
    else if (event_name == html_form_dynamic_table::changed)
	update_table_content_logic(true);
    else
	throw WEBDAR_BUG;
}

unique_ptr<body_builder> html_form_overwrite_combining_criterium::provide_object_of_type(unsigned int num,
											 const string & context,
											 string & changed_event) const
{
    unique_ptr<body_builder> ret;
    unique_ptr<html_form_overwrite_combining_criterium> tmp;

    switch(num)
    {
    case 0:
	ret.reset(new (nothrow) html_form_overwrite_base_criterium());
	changed_event = html_form_overwrite_base_criterium::changed;
	break;
    case 1:
	tmp.reset(new (nothrow) html_form_overwrite_combining_criterium(invert_logic(context)));
	if(!tmp)
	    throw exception_memory();
	changed_event = html_form_overwrite_combining_criterium::changed;
	if(current_bool_mode == and_op)
	    tmp->current_bool_mode = or_op;
	else
	    tmp->current_bool_mode = and_op;
	ret = std::move(tmp);
	break;
    default:
	throw WEBDAR_BUG;
    }

    if(!ret)
	throw exception_memory();

    return ret;
}

string html_form_overwrite_combining_criterium::inherited_get_body_part(const chemin & path,
									const request & req)
{
    return get_body_part_from_all_children(path, req);
}


void html_form_overwrite_combining_criterium::new_css_library_available()
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
}

string html_form_overwrite_combining_criterium::bool_op_to_name(const std::string & op)
{
    if(op == and_op)
	return "and";
    else if(op == or_op)
	return "or";
    else
	throw WEBDAR_BUG;
}

void html_form_overwrite_combining_criterium::update_table_content_logic(bool unconditionally)
{
    string target_bool_mode = crit_type.get_selected_id();

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
	act(changed);
    }
}


string html_form_overwrite_combining_criterium::invert_logic(const std::string & logic)
{
    if(logic == and_op)
	return or_op;
    else if(logic == or_op)
	return and_op;
    else
	throw WEBDAR_BUG;
}

