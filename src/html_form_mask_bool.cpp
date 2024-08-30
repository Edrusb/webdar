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

    //
#include "html_form_mask_bool.hpp"

using namespace std;

html_form_mask_bool::html_form_mask_bool():
    fs(""),
    mask_type("Combining with", bool_changed_event),
    table(3),
    adder("Add a new mask", new_mask_to_add),
    event_del_count(0),
    root_prefix(libdar::FAKE_ROOT)
{

	// components configuration
    mask_type.add_choice(and_op, "AND");
    mask_type.add_choice(or_op, "OR");
    if(mask_type.num_choices() != 2)
	throw WEBDAR_BUG;
    mask_type.set_selected(0);
    current_bool_mode = mask_type.get_selected_id();
    current_table_size = table_content.size();

    adder.add_choice("", "--- select a mask type ---");
    adder.set_selected(0);

    init();
}

html_form_mask_bool::html_form_mask_bool(const html_form_mask_bool & ref):
    html_mask(ref), // parent class
    actor(ref),     // parent class
    fs(ref.fs),
    mask_type(ref.mask_type),
    table(ref.table.get_width()),
    adder(ref.adder),
    list_of_mask_types(ref.list_of_mask_types),
    event_del_count(0),
    root_prefix(ref.root_prefix)
{
	// want the new object to have *AND* operation if ref has *OR*
	// and vice versa. Because it does not make much sense to add
	// and *AND* operator as member of an *AND* operator, same thing
	// having and *OR* operator as member of another *OR* operator,
	// stated that *AND* and *OR* operator can combine arbitrary number
	// of components.
    if(mask_type.num_choices() != 2)
	throw WEBDAR_BUG; // should be *AND* and *OR* choices only

	// *AND* has index 0: 2 - 1 - 0 -> 1
	// *OR* has index 1:  2 - 1 - 1 -> 0
    mask_type.set_selected(mask_type.num_choices() - 1 - mask_type.get_selected_num());
    current_bool_mode = mask_type.get_selected_id();

    current_table_size = table_content.size();
    adder.set_selected(0);

    init();
}

void html_form_mask_bool::add_mask_type(const string & label,
					const html_mask & tobecloned)
{
    adder.add_choice(webdar_tools_convert_to_string(list_of_mask_types.size()), label);
    list_of_mask_types.push_back(available_mask(label, tobecloned));
}

void html_form_mask_bool::add_mask_myself(const string & label)
{
    adder.add_choice(webdar_tools_convert_to_string(list_of_mask_types.size()), label);
    list_of_mask_types.push_back(available_mask(label));
}

void html_form_mask_bool::set_root_prefix(const libdar::path & x_prefix)
{
    list<entry>::iterator it = table_content.begin();

    root_prefix = x_prefix; // record the value for use with future sub-mask

	// updating current existing sub-masks
    while(it != table_content.end())
    {
	if(! (it->mask))
	    throw WEBDAR_BUG; // the unique_ptr points to nothing
	(it->mask)->set_root_prefix(x_prefix);
	++it;
    }
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

void html_form_mask_bool::on_event(const string & event_name)
{
    if(event_name == new_mask_to_add)
    {
	if(adder.get_selected_num() != 0)
	{
		// there is a shift by one between adder which first position (index 0)
		// tells the user to select a filter and list_of_mask_types that have the
		// first valid mask at position 0.
	    add_mask(adder.get_selected_num() - 1);
	    adder.set_selected(0); // resetting 'adder' to undefined
	}
    }
    else if(event_name == bool_changed_event)
	update_table_content_logic();
    else // check whether this is an event from a delete button
	del_mask(event_name); // may throw exception if event_name is not a del event
}

string html_form_mask_bool::inherited_get_body_part(const chemin & path,
						    const request & req)
{
    string ret = get_body_part_from_all_children(path, req);
    purge_to_delete();
    return ret;
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
}


html_form_mask_bool::available_mask::available_mask(const string & lab,
						    const html_mask & tobecloned):
    label(lab)
{
    mask_type = tobecloned.clone();
}

html_form_mask_bool::available_mask::available_mask(const string & lab):
    label(lab)
{
    mask_type.reset();
}

html_form_mask_bool::available_mask::available_mask(available_mask && ref) noexcept
{
    label = std::move(ref.label);
    mask_type = std::move(ref.mask_type);
}

html_form_mask_bool::available_mask::available_mask(const available_mask & ref):
    label(ref.label)
{
    if(ref.mask_type)
	mask_type = ref.mask_type->clone();
    else
	mask_type.reset();
}

void html_form_mask_bool::init()
{
	// adoption tree

    table.adopt(&mask_type);
    table.adopt_static_html(""); // second colum first line
    table.adopt_static_html(""); // third colum first line
    fs.adopt(&table);
    fs.adopt(&adder);
    adopt(&fs);

	// events
    adder.record_actor_on_event(this, new_mask_to_add);

	// visibility


	// css stuff
    table.set_css_class_first_column(css_class_bool_text);

}


void html_form_mask_bool::add_mask(unsigned int num)
{
    entry new_mask;

    if(num >= list_of_mask_types.size())
	throw WEBDAR_BUG;

    if(!list_of_mask_types[num].mask_type)
	new_mask.mask = clone(); // we clone ourself
    else
	new_mask.mask = list_of_mask_types[num].mask_type->clone();

    if(! new_mask.mask)
	throw exception_memory();
    new_mask.mask->set_root_prefix(root_prefix);

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


void html_form_mask_bool::del_mask(const string & event_name)
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

void html_form_mask_bool::purge_to_delete()
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

string html_form_mask_bool::bool_op_to_name(const string & op)
{
    if(op == and_op)
	return "and";
    else if(op == or_op)
	return "or";
    else
	throw WEBDAR_BUG;
}

void html_form_mask_bool::update_table_content_logic()
{
    string target_bool_mode = mask_type.get_selected_id();
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

