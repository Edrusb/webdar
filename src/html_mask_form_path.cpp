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
#include "html_form_mask_subdir.hpp"
#include "html_form_mask_file.hpp"

    // webdar headers


    //
#include "html_mask_form_path.hpp"

using namespace std;

html_mask_form_path::html_mask_form_path(bool allow_absolute_paths):
    html_form("Update"),
    allow_abs_paths(allow_absolute_paths)
{
    labels.push_back("Path expression");
    labels.push_back("File listing");
    labels.push_back("Logical combination");
    init_bool_obj(root);
    adopt(&root);
}

void html_mask_form_path::set_fs_root(const std::string & prefix)
{
    try
    {
	if(!prefix.empty())
	{
	    libdar::path tmp(prefix);
	    root.set_root_prefix(tmp);
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

    switch(num)
    {
    case 0: // path expression
	ret.reset(new (nothrow) html_form_mask_subdir(allow_abs_paths));
	break;
    case 1: /// file listing
	ret.reset(new (nothrow) html_form_mask_file());
	break;
    case 2: // "logical combination"
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


void html_mask_form_path::init_bool_obj(html_form_mask_bool & obj) const
{
    obj.set_obj_type_provider(this);
    for(deque<string>::const_iterator it = labels.begin(); it != labels.end(); ++it)
	obj.add_mask_type(*it);
}
