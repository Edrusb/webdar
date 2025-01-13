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
#include "html_over_guichet.hpp"


    //
#include "html_mask_form_filename.hpp"

using namespace std;

const std::string html_mask_form_filename::changed = "hmff_changed";

html_mask_form_filename::html_mask_form_filename(const string & subject):
    form("Update")
{
    sujet.reset(new (nothrow) string(subject));
    if(!sujet)
	throw exception_memory();

    labels.reset(new (nothrow) deque<string>);
    if(!labels)
	throw exception_memory();
    update_labels();

	// adoption tree

    form.adopt(&root);
    adopt(&form);

	// events
    register_name(changed);
    register_name(html_form_mask_expression::update);
    register_name(html_form_mask_bool::update);
    form.record_actor_on_event(this, html_form::changed);

	// must be done after event registration above
    init_bool_obj(root);
}

void html_mask_form_filename::set_child(const std::shared_ptr<bibliotheque> & ptr,
					bibliotheque::category cat)
{
    biblio = ptr;
    categ = cat;
}

void html_mask_form_filename::change_subject(const std::string & subject)
{
    check_ptr();

    *sujet = subject;
    act(html_form_mask_expression::update);
	// inform objects we have created about
	// the change of subject string

    update_labels();
    act(html_form_mask_bool::update);
	// inform objects we have created about
	// the change of subject string
}


unique_ptr<body_builder> html_mask_form_filename::provide_object_of_type(unsigned int num,
									 const string & context) const
{
    unique_ptr<body_builder> ret;
    unique_ptr<html_form_mask_bool> tmp;
    unique_ptr<html_form_mask_expression> as_actor;
    unique_ptr<html_over_guichet> ovgui;

    check_ptr();

    switch(num)
    {
    case 0: // title provided in constructor
	as_actor.reset(new (nothrow) html_form_mask_expression(sujet));
	if(as_actor)
	    const_cast<html_mask_form_filename*>(this)->record_actor_on_event(as_actor.get(),
									      html_form_mask_expression::update);
	ret = std::move(as_actor);
	break;
    case 1: // "logical combination"
	tmp.reset(new (nothrow) html_form_mask_bool(html_form_mask_bool::invert_logic(context)));
	if(!tmp)
	    throw exception_memory();

	init_bool_obj(*tmp);
	ret = std::move(tmp);
	break;
    case 2: // "recorded configuration"
	ovgui.reset(new (nothrow) html_over_guichet());
	if(!ovgui)
	    throw exception_memory();

	ovgui->set_child(biblio, categ);
	init_bool_obj(*ovgui);
	ret = std::move(ovgui);
	break;
    default:
	if(num < labels->size())
	    throw WEBDAR_BUG; // problem in html_mask_form_filename?
	else
	    throw WEBDAR_BUG; // problem in html_form_mask_bool?
    }

    if(!ret)
	throw exception_memory();

    return ret;
}

void html_mask_form_filename::load_json(const json & source)
{
    check_ptr();

    try
    {
	unsigned int version;
	string class_id;
	json config = unwrap_config_from_json_header(source,
						     version,
						     class_id);

	if(class_id != "html_mask_form_filename")
	    throw exception_range(libdar::tools_printf("Unexpected class_id in json data, found %s while expecting html_mask_form_filename",
						       class_id.c_str()));

	if(version > format_version)
	    throw exception_range("Json format version too hight for html_form_mask_filename, upgrade your webdar software");

	    // setting back the bool mode and mask_type selected value

	root.load_json(config.at(jlabel_bool_config));
    }
    catch(json::exception & e)
    {
	throw exception_json("Error loading html_form_mask_bool config", e);
    }

    act(html_form_mask_expression::update);
    act(html_form_mask_bool::update);
    act(changed);
}

json html_mask_form_filename::save_json() const
{
    json ret;

    check_ptr();

	// the 'subjet' is not to be saved,
	// it depends on the context where
	// the html_mask_form_filename is used
	// file filtering, EA filtering, bibliotheque

    ret[jlabel_bool_config] = root.save_json();

    return wrap_config_with_json_header(format_version,
					"html_mask_form_filename",
					ret);
}

void html_mask_form_filename::clear_json()
{
    check_ptr();

    root.clear_json();

    act(html_form_mask_expression::update);
    act(html_form_mask_bool::update);
    act(changed);
}

bibliotheque::using_set html_mask_form_filename::get_using_set() const
{
    return root.get_using_set();
}

void html_mask_form_filename::on_event(const std::string & event_name)
{
    if(event_name == html_form::changed)
	act(changed);
    else
	throw WEBDAR_BUG;
}

string html_mask_form_filename::inherited_get_body_part(const chemin & path,
							const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_mask_form_filename::update_labels()
{
    check_ptr();

    labels->clear();
	// we use tools_printf to ease future message translation
	// if in other languages the subject is not at the beginning
	// but in the middle or at the end of the translated string:
    labels->push_back(libdar::tools_printf("%S expression", sujet.get()));
    labels->push_back("Logical combination");
    labels->push_back("Recorded configuration");
}

void html_mask_form_filename::check_ptr() const
{
    if(!sujet)
	throw WEBDAR_BUG;

    if(!labels)
	throw WEBDAR_BUG;
}
