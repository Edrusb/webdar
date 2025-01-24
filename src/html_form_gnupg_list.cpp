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


    //
#include "html_form_gnupg_list.hpp"

using namespace std;

html_form_gnupg_list::html_form_gnupg_list():
    table(false,
	  true,
	  "Gnupg persona to add",
	  "--- select a type of persona ---")
{
    table.set_obj_type_provider(this);
    table.add_obj_type("Gnupg recipient"); // index 0 in provide_object_of_type()
    table.add_obj_type("Gnupg signatory"); // index 1 in provide_object_of_type()
    adopt(&table);
}

vector<string> html_form_gnupg_list::get_gnupg_recipients() const
{
    return gather_content_of_type(0);
}

vector<string> html_form_gnupg_list::get_gnupg_signatories() const
{
    return gather_content_of_type(1);
}

unique_ptr<body_builder> html_form_gnupg_list::provide_object_of_type(unsigned int num,
								      const string & context) const
{
    unique_ptr<body_builder> ret;

    switch(num)
    {
    case 0:
	ret.reset(new (nothrow)
		  html_form_input("Gnupg recipient key id or email",
				  html_form_input::text,
				  "",
				  "80%"));
	break;
    case 1:
	ret.reset(new (nothrow)
		  html_form_input("Gnupg signatory email",
				  html_form_input::text,
				  "",
				  "80%"));
	break;
    default:
	throw WEBDAR_BUG;
    }

    if(! ret)
	throw exception_memory();

    return ret;
}

void html_form_gnupg_list::load_json(const json & source)
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

	json recipients = config.at(jlabel_recipients);
	json signatories = config.at(jlabel_signatories);

	if(! recipients.is_array() && ! recipients.is_null())
	    throw exception_range(libdar::tools_printf("Expecting table or recipients for label %s in %s json configuration",
						       jlabel_recipients,
						       myclass_id));

	if(! signatories.is_array() && ! signatories.is_null())
	    throw exception_range(libdar::tools_printf("Expecting table or signatories for label %s in %s json configuration",
						       jlabel_signatories,
						       myclass_id));

	json::iterator itrecept = recipients.begin();
	json::iterator itsignat = signatories.begin();
	html_form_input* ptr = nullptr;

	table.clear();
	for(unsigned int i = 0; i < recipients.size(); ++i)
	    table.add_line(0);

	for(unsigned int i = 0; i < signatories.size(); ++i)
	    table.add_line(1);

	for(html_form_dynamic_table::iterator it = table.begin();
	    it != table.end();
	    ++it)
	{
	    ptr = dynamic_cast<html_form_input*>(it.get_object().get());
	    if(ptr == nullptr)
		throw WEBDAR_BUG; // both type 0 and 1 lead to the creation of an html_form_input object

	    switch(it.get_object_type())
	    {
	    case 0: // recipient
		if(itrecept == recipients.end())
		    throw WEBDAR_BUG; // we should have paired objects between json and dynamic_table structures
		ptr->set_value(itrecept->at(jlabel_email));
		++itrecept;
		break;
	    case 1: // signatory
		if(itsignat == signatories.end())
		    throw WEBDAR_BUG;  // we should have paired objects between json and dynamic_table structures
		ptr->set_value(itsignat->at(jlabel_email));
		++itsignat;
		break;
	    default:
		throw WEBDAR_BUG;
	    }
	}
    }
    catch(json::exception & e)
    {
	throw exception_json(libdar::tools_printf("Error loading %s config", myclass_id), e);
    }
}

json html_form_gnupg_list::save_json() const
{
    json ret;
    vector<json> ret_per_type;

    for(unsigned int i = 0; i < 2 ; ++i) // we currently only have two types of objects
    {
	vector<string> objects = gather_content_of_type(i);
	json ret_objs;
	json tmp;

	for(vector<string>::iterator it = objects.begin();
	    it != objects.end();
	    ++it)
	{
	    tmp.clear();
	    tmp[jlabel_email] = *it;
	    ret_objs.push_back(tmp);
	}
	ret_per_type.push_back(ret_objs);
    }

    if(ret_per_type.size() != 2)
	throw WEBDAR_BUG;
    ret[jlabel_recipients] = ret_per_type[0];
    ret[jlabel_signatories] = ret_per_type[1];

    return wrap_config_with_json_header(format_version,
					myclass_id,
					ret);
}

void html_form_gnupg_list::clear_json()
{
    table.clear();
}

string html_form_gnupg_list::inherited_get_body_part(const chemin & path,
						    const request & req)
{
    return get_body_part_from_all_children(path, req);
}

vector<string> html_form_gnupg_list::gather_content_of_type(unsigned int type) const
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
