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
#include "html_form_filename_mask.hpp"


    //
#include "html_form_bool_mask.hpp"

using namespace std;

html_form_bool_mask::html_form_bool_mask(bool include_html_form):
    fs(""),
    mask_type("Combining with", bool_changed_event),
    table(3),
    adder("Add a new mask", new_mask_to_add),
    papillotte("Update"),
    event_del_count(0)
{

	// components configuration
    mask_type.add_choice(and_op, "AND");
    mask_type.add_choice(or_op, "OR");
    mask_type.set_selected(0);
    current_bool_mode = mask_type.get_selected_id();
    current_table_size = table_content.size();

    adder.add_choice(type_undefined, "select a mask type");
    adder.add_choice(type_filename, "filename expression");
    adder.add_choice(type_bool, "boolean combinaison");
    adder.set_selected(0);

	// adoption tree

    table.adopt(&mask_type);
    table.adopt_static_html(""); // second colum first line
    table.adopt_static_html(""); // third colum first line
    fs.adopt(&table);
    fs.adopt(&adder);
    if(include_html_form)
    {
	papillotte.adopt(&fs);
	adopt(&papillotte);
    }
    else
	adopt(&fs);

	// events
    adder.record_actor_on_event(this, new_mask_to_add);


	// visibility


	// css stuff
    table.set_css_class_first_column(css_class_bool_text);
}

unique_ptr<libdar::mask> html_form_bool_mask::get_mask() const
{
    unique_ptr<libdar::mask> ret;
    bool and_mode;

    if(mask_type.get_selected_id() == and_op)
	and_mode = true;
    else if(mask_type.get_selected_id() == or_op)
	and_mode = false;
    else
	throw WEBDAR_BUG;

    if(table_content.empty())
	ret.reset(new (nothrow) libdar::bool_mask(true));
    else
    {
	list<entry>::const_iterator it = table_content.begin();
	unique_ptr<libdar::et_mask> tmp;

	if(and_mode)
	    tmp.reset(new (nothrow) libdar::et_mask());
	else
	    tmp.reset(new (nothrow) libdar::ou_mask());

	if(!tmp)
	    throw exception_memory();

	    // adding member to the mask

	while(it != table_content.end())
	{
	    if(!it->mask)
		throw WEBDAR_BUG;
	    unique_ptr<libdar::mask> toadd(it->mask->get_mask());

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

void html_form_bool_mask::on_event(const string & event_name)
{
    if(event_name == new_mask_to_add)
    {
	if(adder.get_selected_id() != type_undefined)
	{
	    add_mask(adder.get_selected_id());
	    adder.set_selected(0); // resetting 'adder' to undefined
	}
    }
    else if(event_name == bool_changed_event)
	update_table_content_logic();
    else // check whether this is an event from a delete button
	del_mask(event_name); // may throw exception if event_name is not a del event
}

string html_form_bool_mask::inherited_get_body_part(const chemin & path,
						    const request & req)
{
    string ret = get_body_part_from_all_children(path, req);
    purge_to_delete();
    return ret;
}

void html_form_bool_mask::new_css_library_available()
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

void html_form_bool_mask::add_mask(const string & mask_type)
{
    entry new_mask;

    if(mask_type == type_filename)
    {
	new_mask.mask.reset(new (nothrow) html_form_filename_mask());
    }
    else if(mask_type == type_bool)
    {
	new_mask.mask.reset(new (nothrow) html_form_bool_mask(false));
    }
    else if(mask_type == type_undefined)
	throw WEBDAR_BUG;  // cannot create a mask of that type
    else
	throw WEBDAR_BUG;  // unknown mask type

    if(! new_mask.mask)
	throw exception_memory();

    new_mask.del.reset(new (nothrow) html_form_input("delete",
						     html_form_input::check,
						     "",
						     1));
    if(! new_mask.del)
	throw exception_memory();
	// no worries here regarding new_mask.mask we just allocated before
	// the unique_ptr will delete it exiting from this method

    new_mask.logic.reset(new (nothrow) html_text(0, ""));
    if(! new_mask.logic)
	throw exception_memory();

    string event_name = webdar_tools_convert_to_string(event_del_count++);
    new_mask.del->set_change_event_name(event_name);
    new_mask.del->record_actor_on_event(this, event_name);
    table.adopt(&(*new_mask.logic));
    table.adopt(&(*new_mask.mask));
    table.adopt(&(*new_mask.del));
    list<entry>::iterator pos = table_content.insert(table_content.end(),
						     std::move(new_mask));
    del_event_to_content[event_name] = pos;
    update_table_content_logic();
}


void html_form_bool_mask::del_mask(const string & event_name)
{
    map<string, list<entry>::iterator>::iterator mit = del_event_to_content.find(event_name);

    if(mit == del_event_to_content.end())
	throw WEBDAR_BUG;  // event_name absent from the map!

    list<entry>::iterator it = mit->second;
    if(!it->mask)
	throw WEBDAR_BUG;
    if(!it->del)
	throw WEBDAR_BUG;

    table.foresake(&(*(it->mask)));
    table.foresake(&(*(it->del)));
    events_to_delete.push_back(event_name);
	// postponing the object deletion
	// as we may get from here from the it->del pointed to actor event
	// we would not be able to return from this call if deleted from here
}

void html_form_bool_mask::purge_to_delete()
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

std::string html_form_bool_mask::bool_op_to_name(const std::string & op)
{
    if(op == and_op)
	return "and";
    else if(op == or_op)
	return "or";
    else
	throw WEBDAR_BUG;
}

void html_form_bool_mask::update_table_content_logic()
{
    std::string target_bool_mode = mask_type.get_selected_id();
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
