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


    // webdar headers



    //
#include "html_form_fieldset.hpp"

using namespace std;

string html_form_fieldset::inherited_get_body_part(const chemin & path,
						   const request & req)
{
    string ret = "";

    ret += "<fieldset " + get_css_classes() + ">\n";
    ret += get_body_part_from_all_children(path, req);
    ret += "</fieldset>\n";

    return ret;
}

void html_form_fieldset::has_adopted(body_builder *obj)
{
    if(obj == nullptr)
	throw WEBDAR_BUG;

    if(! bounded_anchor.empty())
	obj->bind_to_anchor(bounded_anchor);
}

void html_form_fieldset::update_anchor(const std::string & val)
{
    bounded_anchor = val;

    for(unsigned int i = 0; i < size(); ++i)
    {
	if((*this)[i] == nullptr)
	    throw WEBDAR_BUG;
	(*this)[i]->bind_to_anchor(bounded_anchor);
    }
}
