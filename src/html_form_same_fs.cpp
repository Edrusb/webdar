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
#include "html_form_input_file.hpp"
#include "webdar_css_style.hpp"

    //
#include "html_form_same_fs.hpp"

using namespace std;

const string html_form_same_fs::changed = "hfsfs-changed";

html_form_same_fs::html_form_same_fs():
    table(false,
	  true,
	  "File system mount point to add",
	  "--- select a type of filter ---")
{
	// components setup

    table.set_obj_type_provider(this);
    table.add_obj_type("included mount point"); // index 0 in provide_object_of_type()
    table.add_obj_type("excluded mount point"); // index 1 in provide_object_of_type()

	// adption tree

    adopt(&table);

	// events

    table.record_actor_on_event(this, html_form_dynamic_table::changed);
    register_name(changed);

	// css
}

vector<string> html_form_same_fs::get_included_fs_path() const
{
    return gather_content_of_type(0);
}

vector<string> html_form_same_fs::get_excluded_fs_path() const
{
    return gather_content_of_type(1);
}

unique_ptr<body_builder> html_form_same_fs::provide_object_of_type(unsigned int num,
								   const string & context,
								   string & changed_event) const
{
    unique_ptr<html_form_input_file> ret;

    switch(num)
    {
    case 0:
	ret.reset(new (nothrow)
		  html_form_input_file("Included mount point",
				       "/",
				       "",
				       webdar_css_style::width_80vw,
				       "Select a path which filesystem will be included in the operation..."));
	break;
    case 1:
	ret.reset(new (nothrow)
		  html_form_input_file("Excluded mount point",
				       "/",
				       "",
				       webdar_css_style::width_80vw,
				       "Select a path which filesystem will be excluded from the operation..."));
	break;
    default:
	throw WEBDAR_BUG;
    }

    if(! ret)
	throw exception_memory();
    else
    {
	ret->set_select_mode(html_form_input_file::select_dir);
	ret->set_can_create_dir(false);
    }

    return ret;
}

void html_form_same_fs::load_json(const json & source)
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

json html_form_same_fs::save_json() const
{
    json ret;

    ret[jlabel_contents] = table.save_json();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					ret);
}

void html_form_same_fs::clear_json()
{
    table.clear_json();
}

void html_form_same_fs::on_event(const string & event_name)
{
    if(event_name == html_form_dynamic_table::changed)
	act(changed);
    else
	throw WEBDAR_BUG;
}


string html_form_same_fs::inherited_get_body_part(const chemin & path,
						    const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_form_same_fs::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}


vector<string> html_form_same_fs::gather_content_of_type(unsigned int type) const
{
    vector<string> ret;
    html_form_dynamic_table::iterator it = table.begin();

    while(it != table.end())
    {
	if(it.get_object_type() == type) // this is an object of the the type we gather
	{
	    shared_ptr<body_builder> obj = it.get_object();

	    if(!obj)
		throw WEBDAR_BUG;
	    else
	    {
		html_form_input* hfi = dynamic_cast<html_form_input*>(obj.get());
		if(hfi == nullptr)
		    throw WEBDAR_BUG; // the body_builder object is not an html_form_input
		ret.push_back(hfi->get_value());
	    }
	}
	    // else we ignore other types

	++it;
    }

    return ret;
}
