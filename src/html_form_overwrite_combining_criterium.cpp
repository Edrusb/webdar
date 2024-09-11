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
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_form_overwrite_base_criterium.hpp"


    //
#include "html_form_overwrite_combining_criterium.hpp"

using namespace std;

html_form_overwrite_combining_criterium::html_form_overwrite_combining_criterium():
    fs(""),
    crit_type("Combining with", html_form_select::changed),
    table(true, false, "Add a new mask", " --- select a criterium --- "),
    self_added(false)
{

	// components configuration

    crit_type.add_choice(and_op, "AND");
    crit_type.add_choice(or_op, "OR");
    if(crit_type.num_choices() != 2)
	throw WEBDAR_BUG;
    crit_type.set_selected(0);
    current_bool_mode = crit_type.get_selected_id();

    table.add_obj_type("atomic criterium", html_form_overwrite_base_criterium());


	// adoption tree

    fs.adopt(&crit_type);
    fs.adopt(&table);
    adopt(&fs);

	// events
    crit_type.record_actor_on_event(this, html_form_select::changed);
    table.record_actor_on_event(this, html_form_dynamic_table::changed);

	// visibility

	// css stuff
    table.set_css_class_first_column(css_class_bool_text);
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

void html_form_overwrite_combining_criterium::on_event(const std::string & event_name)
{
    if(event_name == html_form_select::changed)
	update_table_content_logic(false);
    else if (event_name == html_form_dynamic_table::changed)
	update_table_content_logic(true);
    else
	throw WEBDAR_BUG;
}

string html_form_overwrite_combining_criterium::inherited_get_body_part(const chemin & path,
									const request & req)
{
    if(!self_added)
    {
	table.add_obj_type("composit criterium", *this);
	self_added = true;
    }

    return  get_body_part_from_all_children(path, req);
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
    }
}
