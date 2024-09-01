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
    event_del_count(0),
    fs(""),
    crit_type("Combining with", bool_changed_event),
    table(3),
    adder("Add a new mask", new_crit_to_add)
{

	// components configuration

    crit_type.add_choice(and_op, "AND");
    crit_type.add_choice(or_op, "OR");
    if(crit_type.num_choices() != 2)
	throw WEBDAR_BUG;
    crit_type.set_selected(0);
    current_bool_mode = crit_type.get_selected_id();
    current_table_size = table_content.size();

    adder.add_choice("", " --- select a criterium ---");
    adder.add_choice(base_crit, "atomic criterium");
    adder.add_choice(combining_crit, "composit criterium");
    adder.set_selected(0);

	// adoption tree

    table.adopt(&crit_type);
    table.adopt_static_html(""); // second column, first line
    table.adopt_static_html(""); // thrid column, first line

    fs.adopt(&table);
    fs.adopt(&adder);
    adopt(&fs);

	// events
    adder.record_actor_on_event(this, new_crit_to_add);

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
	list<entry>::const_iterator it = table_content.begin();

	while(it != table_content.end())
	{
	    if(!(it->crit))
		throw WEBDAR_BUG;
	    unique_ptr<libdar::criterium> sub((it->crit)->get_overwriting_criterium());
	    if(!sub)
		throw WEBDAR_BUG;
	    ret->add_crit(*sub);

	    ++it;
	}
    }

    return ret;
}

void html_form_overwrite_combining_criterium::on_event(const std::string & event_name)
{
    if(event_name == new_crit_to_add)
    {
	add_crit(adder.get_selected_id());
	adder.set_selected(0); // resetting 'adder'
    }
    else
	del_crit(event_name);
}

string html_form_overwrite_combining_criterium::inherited_get_body_part(const chemin & path,
									const request & req)
{
    string ret = get_body_part_from_all_children(path, req);
    purge_to_delete();
    return ret;
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

void html_form_overwrite_combining_criterium::add_crit(const std::string crit_type)
{
    entry new_crit;

    if(crit_type == "")
	return; // crit_type was set to undefined type

    if(crit_type == base_crit)
	new_crit.crit.reset(new (nothrow) html_form_overwrite_base_criterium());
    else if(crit_type == combining_crit)
	new_crit.crit.reset(new (nothrow) html_form_overwrite_combining_criterium());
    else
	throw WEBDAR_BUG;

    if(!new_crit.crit)
	throw exception_memory();

    new_crit.del.reset(new (nothrow) html_form_input("delete",
						     html_form_input::check,
						     "",
						     1));
    if(!new_crit.del)
	throw exception_memory();
    string event_name = webdar_tools_convert_to_string(event_del_count++);
    new_crit.del->set_change_event_name(event_name);
    new_crit.del->record_actor_on_event(this, event_name);

    new_crit.logic.reset(new (nothrow) html_text(0, ""));
    if(! new_crit.logic)
	throw exception_memory();

    table.adopt(&(*new_crit.logic));
    table.adopt(&(*new_crit.crit));
    table.adopt(&(*new_crit.del));
    list<entry>::iterator pos = table_content.insert(table_content.end(),
						     std::move(new_crit));

    del_event_to_content[event_name] = pos;
    update_table_content_logic();
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

void html_form_overwrite_combining_criterium::update_table_content_logic()
{
    string target_bool_mode = crit_type.get_selected_id();
    unsigned int target_table_size = table_content.size();

    if(target_bool_mode != current_bool_mode
       || target_table_size != current_table_size)
    {
	string logic_text = bool_op_to_name(target_bool_mode);
	list<entry>::iterator it = table_content.begin();

	    // first line is empty
	if(it != table_content.end())
	{
	    if(!it->logic)
		throw WEBDAR_BUG;
	    it->logic->clear();
	    ++it;
	}

	while(it != table_content.end())
	{
	    if(!it->logic)
		throw WEBDAR_BUG;
	    it->logic->clear();
	    it->logic->add_text(0, logic_text);
	    ++it;
	}

	current_bool_mode = target_bool_mode;
	current_table_size = target_table_size;
    }
}

void html_form_overwrite_combining_criterium::del_crit(const string & event_name)
{
    map<string, list<entry>::iterator>::iterator mit = del_event_to_content.find(event_name);

    if(mit == del_event_to_content.end())
	throw WEBDAR_BUG; // event_name absent from the map!

    list<entry>::iterator it = mit->second;
    if(!it->logic)
	throw WEBDAR_BUG;
    if(!it->crit)
	throw WEBDAR_BUG;
    if(!it->del)
	throw WEBDAR_BUG;

    table.foresake(&(*(it->logic)));
    table.foresake(&(*(it->crit)));
    table.foresake(&(*(it->del)));
    events_to_delete.push_back(event_name);
    	// postponing the object deletion
	// as we may get from here from the it->del pointed to actor event
	// we would not be able to return from this call if deleted from here
}

void html_form_overwrite_combining_criterium::purge_to_delete()
{
    deque<string>::iterator evit = events_to_delete.begin();

    while(evit != events_to_delete.end())
    {
	map<string, list<entry>::iterator>::iterator mit = del_event_to_content.find(*evit);

	if(mit == del_event_to_content.end())
	    throw WEBDAR_BUG;  // event_name absent from the map!

	list<entry>::iterator it = mit->second;

	(void)del_event_to_content.erase(mit);
	(void)table_content.erase(it);

	++evit;
    }

    events_to_delete.clear();
    update_table_content_logic();
}

