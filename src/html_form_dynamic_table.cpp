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
#include "html_form_dynamic_table.hpp"

using namespace std;

const string html_form_dynamic_table::changed = "html_form_dynamic_table_changed";


html_form_dynamic_table::html_form_dynamic_table(bool has_left_labels,
						 bool selector_below,
						 const string & adder_label,
						 const string & adder_default_choice):
    my_provider(nullptr),
    left_label(has_left_labels),
    table(has_left_labels ? 3 : 2),
    adder(adder_label, new_line_to_add),
    ignore_events(false),
    default_choice(adder_default_choice)
{

	// components configuration/
    adder.add_choice("", default_choice);
    adder.set_selected_num(0);

	// adoption tree
    if(!selector_below)
	adopt(&adder);
    adopt(&table);
    if(selector_below)
	adopt(&adder);

	// events
    register_name(changed);
    adder.record_actor_on_event(this, new_line_to_add);

	// visibility

	// css stuff
    table.add_css_class(webdar_css_style::width_full);
}

void html_form_dynamic_table::set_obj_type_provider(const html_form_dynamic_table_object_provider* provider)
{
    if(provider == nullptr)
	throw WEBDAR_BUG;
    my_provider = provider;
}


void html_form_dynamic_table::clear_obj_type()
{
    adder.clear();
    adder.add_choice("", default_choice);
}

void html_form_dynamic_table::add_obj_type(const string & label)
{
    if(my_provider == nullptr)
	throw WEBDAR_BUG;

    adder.add_choice(label, label);
    adder.set_selected_num(0);
}

void html_form_dynamic_table::add_line(unsigned int typenum)
{
    line newline;
    string changed_event;

    if(left_label)
    {
	newline.left_label.reset(new (nothrow) html_text());
	if(! newline.left_label)
	    throw exception_memory();
    }
    else
	newline.left_label.reset();

    newline.object_type_index = typenum;
    if(my_provider == nullptr)
	throw WEBDAR_BUG;
    newline.dynobj = my_provider->provide_object_of_type(typenum, context, changed_event);
    if(! newline.dynobj)
	throw WEBDAR_BUG;
    if(!changed_event.empty())
    {
	events* ptr = dynamic_cast<events*>(newline.dynobj.get());
	if(ptr == nullptr)
	    throw WEBDAR_BUG; // if an event name is given the provided object must be able to generate events
	ptr->record_actor_on_event(this, changed_event);
    }

    newline.del.reset(new (nothrow) html_form_input("delete",
						    html_form_input::check,
						    "",
						    "1"));
    if(! newline.del)
	throw exception_memory();

    string event_name = webdar_tools_convert_to_string(event_del_count++);
    newline.del->set_change_event_name(event_name);
    newline.del->record_actor_on_event(this, event_name);
    if(newline.left_label)
	table.adopt(&(*newline.left_label));
    table.adopt(&(*newline.dynobj));
    table.adopt(&(*newline.del));
    list<line>::iterator pos = table_content.insert(table_content.end(),
						    std::move(newline));
    del_event_to_content[event_name] = pos;
}

void html_form_dynamic_table::load_json(const json & source)
{
    try
    {
	unsigned int version;
	string class_id;
	json listing;
	jsoner* jsnptr = nullptr;
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

	left_label = config.at(jlabel_has_left_label);
	listing = config.at(jlabel_contents);

	if(! listing.is_array() && ! listing.is_null())
	    throw exception_range(libdar::tools_printf("Expecting json list as content for %s json configuration",
						       myclass_id));

	if(my_provider == nullptr)
	    throw WEBDAR_BUG;

	ignore_events = true;
	try
	{

	    clear(); // removing all table content (but keeping object provider and context)

	    for(json::iterator it = listing.begin();
		it != listing.end();
		++it)
	    {
		add_line(it->at(jlabel_index_type)); // new object is added at the end of table_content
		if(table_content.empty())
		    throw WEBDAR_BUG;
		if(table_content.back().object_type_index != it->at(jlabel_index_type))
		    throw WEBDAR_BUG;

		if(left_label)
		{
		    if(! table_content.back().left_label)
			throw WEBDAR_BUG;
		    table_content.back().left_label->set_raw_value(it->at(jlabel_left_label));
		}

		if(! table_content.back().dynobj)
		    throw WEBDAR_BUG;
		jsnptr = dynamic_cast<jsoner*>(table_content.back().dynobj.get());
		if(jsnptr == nullptr)
		    throw WEBDAR_BUG;
		jsnptr->load_json(it->at(jlabel_dynobj));
	    }
	}
	catch(...)
	{
	    ignore_events = false;
	    throw;
	}
	ignore_events = false;

    }
    catch(json::exception & e)
    {
	throw exception_json(libdar::tools_printf("Error loading %s config", myclass_id), e);
    }
}

json html_form_dynamic_table::save_json() const
{
    json config;
    json listing;
    json tmp;
    const jsoner* ptr = nullptr;

    for(list<line>::const_iterator it = table_content.begin();
	it != table_content.end();
	++it)
    {
	tmp.clear();

	if(left_label)
	{
	    if(! it->left_label)
		throw WEBDAR_BUG;
	    tmp[jlabel_left_label] = it->left_label->get_raw_value();
	}

	tmp[jlabel_index_type] = it->object_type_index;

	ptr = dynamic_cast<const jsoner*>(it->dynobj.get());
	if(ptr == nullptr)
	    throw WEBDAR_BUG;
	tmp[jlabel_dynobj] = ptr->save_json();

	listing.push_back(tmp);
    }

    config[jlabel_has_left_label] = left_label;
    config[jlabel_contents] = listing;

    return wrap_config_with_json_header(format_version,
					myclass_id,
					config);
}

void html_form_dynamic_table::clear_json()
{
    clear();
}

void html_form_dynamic_table::on_event(const std::string & event_name)
{
    if(event_name == new_line_to_add)
    {
	if(adder.get_selected_num() != 0)
	{
		// there is a shift by one between adder which first position (index 0)
		// tells the user to select a filter and list_of_mask_types that have the
		// first valid mask at position 0.
	    add_line(adder.get_selected_num() - 1);
	    adder.set_selected_num(0); // resetting 'adder' to undefined
	    if(! ignore_events)
		act(changed);
	}
    }
    else
	del_line(event_name);
	// checks whether this is an event from a delete button
	// as well as handle change_events from generated and adopted objects
}

string html_form_dynamic_table::inherited_get_body_part(const chemin & path,
							const request & req)
{
    string ret = html_div::inherited_get_body_part(path, req);
    purge_to_delete();
    return ret;
}

void html_form_dynamic_table::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}


void html_form_dynamic_table::del_line(const string & event_name)
{
    map<string, list<line>::iterator>::iterator mit = del_event_to_content.find(event_name);

    if(mit == del_event_to_content.end())
    {
	if(! ignore_events)
	    act(changed);
	return;
	    // event_name absent from the map!
	    // this may be a changed_event from a generated object
    }

	// for delete events
	// act(changed) will be done by purge_to_delete() outside the on_event()
	// cascada calls

    list<line>::iterator it = mit->second;

    if(left_label && !it->left_label)
	throw WEBDAR_BUG;
    if(!it->dynobj)
	throw WEBDAR_BUG;
    if(!it->del)
	throw WEBDAR_BUG;

    if(it->left_label)
	table.foresake(&(*(it->left_label)));
    table.foresake(&(*(it->dynobj)));
    table.foresake(&(*(it->del)));
    events_to_delete.push_back(event_name);
    	// postponing the object deletion
	// as we may get from here from the it->del pointed to actor event
	// we would not be able to return from this call if deleted from here
}

void html_form_dynamic_table::purge_to_delete()
{
    deque<string>::iterator evit = events_to_delete.begin();
    bool action = ! events_to_delete.empty();

    while(evit != events_to_delete.end())
    {
	map<string, list<line>::iterator>::iterator mit = del_event_to_content.find(*evit);

	if(mit == del_event_to_content.end())
	    throw WEBDAR_BUG;  // event_name absent from the map!

	list<line>::iterator it = mit->second;

	(void)del_event_to_content.erase(mit);
	(void)table_content.erase(it);

	++evit;
    }

    events_to_delete.clear();
    if(action && ! ignore_events)
	act(changed);
}
