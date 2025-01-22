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
#include "html_form_mask_subdir.hpp"
#include "html_form_mask_file.hpp"
#include "html_over_guichet.hpp"

    //
#include "html_mask_form_path.hpp"

using namespace std;

const std::string html_mask_form_path::changed = "hmff_changed";

html_mask_form_path::html_mask_form_path(bool allow_absolute_paths):
    form("Update"),
    allow_abs_paths(allow_absolute_paths)
{
    fs_root.reset(new (nothrow) libdar::path(libdar::FAKE_ROOT));
    if(!fs_root)
	throw exception_memory();

    labels.push_back("Path expression");
    labels.push_back("File listing");
    labels.push_back("Logical combination");
    labels.push_back("Recorded configuration");

    init();
}

html_mask_form_path::html_mask_form_path(const html_mask_form_path & ref):
    form(ref.form),
    root(ref.root.get_bool_mode()),
    allow_abs_paths(ref.allow_abs_paths),
    labels(ref.labels),
    fs_root(ref.fs_root),
    categ(ref.categ),
    biblio(ref.biblio)
{
    init();

	// both new and ref objects share the same fs_root
	// but only the ref one should have its set_fs_root() method
	// invoked, thus it does not matter that nobody acts upon
	// the new objects events.
	// But, the html_form_mask_subdirs objects created from this new
	// object will register to this new object events, thus this new one
	// must cascade the event from its parent:
    const_cast<html_mask_form_path*>(&ref)->record_actor_on_event(this, html_form_mask_subdir::update);
}

void html_mask_form_path::set_child(const std::shared_ptr<bibliotheque> & ptr,
				    bibliotheque::category cat)
{
    biblio = ptr;
    categ = cat;
}

void html_mask_form_path::set_fs_root(const std::string & prefix)
{
    try
    {
	if(!prefix.empty())
	{
	    check_ptr();

	    *fs_root = libdar::path(prefix);
	    act(html_form_mask_subdir::update);
		// this to trigger objects of html_form_mask_subdir class
		// we created so far to reconsider both fs_root field
	}
    }
    catch(libdar::Egeneric & e)
    {
	e.prepend_message("Error while setting prefix for path base filtering");
	throw exception_libcall(e);
    }
}

unique_ptr<body_builder> html_mask_form_path::provide_object_of_type(unsigned int num,
								     const string & context) const
{
    unique_ptr<body_builder> ret;
    unique_ptr<html_form_mask_bool> tmp;
    unique_ptr<html_form_mask_subdir> as_actor;
    unique_ptr<html_over_guichet> ovgui;

    check_ptr();

    switch(num)
    {
    case 0: // path expression
	as_actor.reset(new (nothrow) html_form_mask_subdir(allow_abs_paths, fs_root));
	if(as_actor)
	    const_cast<html_mask_form_path*>(this)->record_actor_on_event(as_actor.get(), html_form_mask_subdir::update);
	ret = std::move(as_actor); // we check below globally for all cases that memory allocation succeeded
	break;
    case 1: /// file listing
	ret.reset(new (nothrow) html_form_mask_file(fs_root));
	break;
    case 2: // "logical combination"
	tmp.reset(new (nothrow) html_form_mask_bool(html_form_mask_bool::invert_logic(context)));
	if(!tmp)
	    throw exception_memory();

	init_bool_obj(*tmp);
	ret = std::move(tmp);
	break;
    case 3: // "recorded configuration"
	ret.reset(new (nothrow) html_mask_form_path(*this));
	if(!ret)
	    throw WEBDAR_BUG;

	ovgui.reset(new (nothrow) html_over_guichet());
	if(!ovgui)
	    throw exception_memory();

	ovgui->set_child(biblio, ret, categ);

	if(ret)
	    throw WEBDAR_BUG;
	    // object pointed to by ret
	    // should have been passed to the
	    // object pointed to by ovgui

	ret = std::move(ovgui);
	break;
    default:
	if(num < labels.size())
	    throw WEBDAR_BUG; // problem in html_mask_form_path?
	else
	    throw WEBDAR_BUG; // problem in html_form_mask_bool?
    }

    if(!ret)
	throw exception_memory();

    return ret;
}

void html_mask_form_path::load_json(const json & source)
{
    check_ptr();

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
	    throw exception_range("Json format version too hight for html_form_mask_bool, upgrade your webdar software");

	    // setting back the bool mode and mask_type selected value

	root.load_json(config.at(jlabel_bool_config));
    }
    catch(json::exception & e)
    {
	throw exception_json("Error loading html_form_mask_bool config", e);
    }

    act(html_form_mask_subdir::update);
    act(changed);
}

json html_mask_form_path::save_json() const
{
    json ret;

    check_ptr();

	// we do not save prefix nor allow_abs_paths as json
	// for these configurations stay local to the component
	// that has created/adopted an html_mask_form_path object

    ret[jlabel_bool_config] = root.save_json();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					ret);
}

void html_mask_form_path::clear_json()
{
    check_ptr();

    root.clear_json();

    act(html_form_mask_subdir::update);
    act(changed);
}

bibliotheque::using_set html_mask_form_path::get_using_set() const
{
    return root.get_using_set();
}

void html_mask_form_path::on_event(const std::string & event_name)
{
    if(event_name == html_form::changed)
	act(changed);
    else if(event_name == html_form_mask_subdir::update)
	act(html_form_mask_subdir::update); // cascading from copy constructed parent
    else
	throw WEBDAR_BUG;
}

string html_mask_form_path::inherited_get_body_part(const chemin & path,
							const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_mask_form_path::init()
{
    init_bool_obj(root);

	// adoption tree

    form.adopt(&root);
    adopt(&form);

	// events
    register_name(changed);
    register_name(html_form_mask_subdir::update);
    form.record_actor_on_event(this, html_form::changed);
}

void html_mask_form_path::init_bool_obj(html_form_mask_bool & obj) const
{
    obj.set_obj_type_provider(this);
    for(deque<string>::const_iterator it = labels.begin(); it != labels.end(); ++it)
	obj.add_mask_type(*it);
}


void html_mask_form_path::check_ptr() const
{
    if(!fs_root)
	throw WEBDAR_BUG;
}
