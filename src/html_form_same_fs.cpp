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
#include "html_form_same_fs.hpp"

using namespace std;

html_form_same_fs::html_form_same_fs():
    table(false,
	  true,
	  "Filesystem mount point to add",
	  "--- select a type of filter ---")
{
    table.set_obj_type_provider(this);
    table.add_obj_type("included mount point"); // index 0 in provide_object_of_type()
    table.add_obj_type("excluded mount point"); // index 1 in provide_object_of_type()
    adopt(&table);
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
								      const string & context) const
{
    unique_ptr<body_builder> ret;

    switch(num)
    {
    case 0:
	ret.reset(new (nothrow)
		  html_form_input("Included mount point",
				  html_form_input::text,
				  "",
				  50));
	break;
    case 1:
	ret.reset(new (nothrow)
		  html_form_input("Excluded mount point",
				  html_form_input::text,
				  "",
				  50));
	break;
    default:
	throw WEBDAR_BUG;
    }

    if(! ret)
	throw exception_memory();

    return ret;
}


string html_form_same_fs::inherited_get_body_part(const chemin & path,
						    const request & req)
{
    return get_body_part_from_all_children(path, req);
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
