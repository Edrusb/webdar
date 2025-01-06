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
#include "html_mask_form_filename.hpp"

using namespace std;

html_mask_form_filename::html_mask_form_filename(const string & subject):
    html_form("Update"),
    sujet(subject)
{
	// we use tools_printf to ease future message translation
	// if in other languages the subject is not at the beginning
	// but in the middle or at the end of the translated string:
    labels.push_back(libdar::tools_printf("%S expression", &sujet));
    labels.push_back("Logical combination");
    init_bool_obj(root);

    adopt(&root);
}


unique_ptr<body_builder> html_mask_form_filename::provide_object_of_type(unsigned int num,
									 const string & context) const
{
    unique_ptr<body_builder> ret;
    unique_ptr<html_form_mask_bool> tmp;

    switch(num)
    {
    case 0: // title provided in constructor
	ret.reset(new (nothrow) html_form_mask_expression(sujet));
	break;
    case 1: // "logical combination"
	tmp.reset(new (nothrow) html_form_mask_bool(html_form_mask_bool::invert_logic(context)));
	if(!tmp)
	    throw exception_memory();

	init_bool_obj(*tmp);
	ret = std::move(tmp);
	break;
    default:
	if(num < labels.size())
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
    root.load_json(source);
}

json html_mask_form_filename::save_json() const
{
    return root.save_json();
}

void html_mask_form_filename::clear_json()
{
    root.clear_json();
}

bibliotheque::using_set html_mask_form_filename::get_using_set() const
{
    return root.get_using_set();
}

void html_mask_form_filename::init_bool_obj(html_form_mask_bool & obj) const
{
    obj.set_obj_type_provider(this);
    for(deque<string>::const_iterator it = labels.begin(); it != labels.end(); ++it)
	obj.add_mask_type(*it);
}
