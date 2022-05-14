/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2022 Denis Corbin
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
#include "html_aiguille.hpp"

using namespace std;

void html_aiguille::set_mode(unsigned int m)
{
    if(m < size())
	mode = m;
    else
	throw WEBDAR_BUG;
}

void html_aiguille::has_been_adopted(body_builder *obj)
{
    if(obj == NULL)
	throw WEBDAR_BUG;
    obj->css_inherit_from(*this, true, false);
}

string html_aiguille::get_body_part(const chemin & path,
			      const request & req)
{
    if(mode < size())
    {
	if((*this)[mode] == NULL)
	    throw WEBDAR_BUG;
	else
	{
	    chemin sub_path = path;

	    if(sub_path.size() > 1)
		sub_path.pop_front();


	    return (*this)[mode]->get_body_part(sub_path, req);
	}
    }
    else
	throw WEBDAR_BUG;
}


void html_aiguille::css_updated(bool inherited)
{
    unsigned int s = size();

    for(unsigned int i = 0; i < s; ++i)
    {
	if((*this)[i] == NULL)
	    throw WEBDAR_BUG;
	(*this)[i]->css_inherit_from(*this, true, false);
    }
}
