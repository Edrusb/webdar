/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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
#include "html_form_input_file.hpp"
#include "webdar_css_style.hpp"

    //
#include "html_form_ignore_as_symlink.hpp"

using namespace std;

const string html_form_ignore_as_symlink::changed = "hfias_changed";

html_form_ignore_as_symlink::html_form_ignore_as_symlink():
    table(false,
	  true,
	  "Symlink to follow",
	  "--- select ---")
{
    table.set_obj_type_provider(this);
    table.add_obj_type("add new symlink to follow"); // index 0 in provide_object_of_type()

	// adoption tree

    adopt(&table);

	// events

    table.record_actor_on_event(this, html_form_dynamic_table::changed);
    register_name(changed);

	// css
}

set<string> html_form_ignore_as_symlink::get_symlink_list() const
{
    set<string> ret;
    html_form_dynamic_table::iterator it = table.begin();

    while(it != table.end())
    {
	if(it.get_object_type() == 0) // this is the only expected type
	{
	    shared_ptr<body_builder> obj = it.get_object();

	    if(!obj)
		throw WEBDAR_BUG;
	    else
	    {
		html_form_input_file* hfi = dynamic_cast<html_form_input_file*>(obj.get());
		if(hfi == nullptr)
		    throw WEBDAR_BUG; // the body_builder object is not an html_form_input
		ret.insert(hfi->get_value());
	    }
	}
	else
	    throw WEBDAR_BUG;

	++it;
    }

    return ret;
}

unique_ptr<body_builder> html_form_ignore_as_symlink::provide_object_of_type(unsigned int num,
									     const string & context,
									     string & changed_event) const
{
    unique_ptr<html_form_input_file> tmp;

    switch(num)
    {
    case 0:
	tmp.reset(new (nothrow)
		  html_form_input_file("Followed symlink",
				       "/",
				       "",
				       webdar_css_style::width_80vw,
				       "Select a symlink..."));
	break;
    default:
	throw WEBDAR_BUG;
    }

    if(! tmp)
	throw exception_memory();
    else
	tmp->set_select_mode(html_form_input_file::select_symlink);

    return tmp;
}

void html_form_ignore_as_symlink::load_json(const json & source)
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
	    throw exception_range(libdar::tools_printf("Json format version too hight for %s, upgrade your webdar software", myclass_id));

	table.load_json(config.at(jlabel_contents));
    }
    catch(json::exception & e)
    {
	throw exception_json(libdar::tools_printf("Error loading %s config", myclass_id), e);
    }
}

json html_form_ignore_as_symlink::save_json() const
{
    json ret;

    ret[jlabel_contents] = table.save_json();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					ret);
}

void html_form_ignore_as_symlink::clear_json()
{
    table.clear_json();
}

void html_form_ignore_as_symlink::on_event(const string & event_name)
{
    if(event_name == html_form_dynamic_table::changed)
	act(changed);
    else
	throw WEBDAR_BUG;
}

string html_form_ignore_as_symlink::inherited_get_body_part(const chemin & path,
						    const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_form_ignore_as_symlink::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}
