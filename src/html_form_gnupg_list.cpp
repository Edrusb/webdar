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
#include "html_form_gnupg_list.hpp"

using namespace std;

html_form_text_list::html_form_text_list():
    table(false,
	  true,
	  "Gnupg personas",
	  "--- select a type of persona ---")
{
    html_form_input recip("Gnupg recipient",
			  html_form_input::text,
			  "",
			  50);
    html_form_input signator("Gnupg signatory",
			     html_form_input::text,
			     "",
			     50);

    table.add_obj_type("Gnupg recipient", recip);
    table.add_obj_type("Gnupg signatory", signator);
}

vector<string> html_form_text_list::get_gnupg_recipients() const
{
    return gather_content_of_type(0);
}

vector<string> html_form_text_list::get_gnupg_signatories() const
{
    return gather_content_of_type(1);
}

string html_form_text_list::inherited_get_body_part(const chemin & path,
						    const request & req)
{
    return get_body_part_from_all_children(path, req);
}

vector<string> html_form_text_list::gather_content_of_type(unsigned int type) const
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
