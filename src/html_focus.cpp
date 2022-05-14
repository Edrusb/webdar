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
#include "html_focus.hpp"

using namespace std;

void html_focus::given_for_temporary_adoption(body_builder *obj)
{
    if(obj == NULL)
	throw WEBDAR_BUG;
    if(adopted != NULL)
	foresake(adopted);
    adopted = obj;
    adopt(obj);
    obj->css_inherit_from(*this, true, true);
}

string html_focus::get_body_part(const chemin & path,
				 const request & req)
{
    string ret;

    if(get_visible())
	ret = get_body_part_from_all_children(path, req);

    if(!get_next_visible())
	ret = "";
    ack_visible();

    return ret;
}
