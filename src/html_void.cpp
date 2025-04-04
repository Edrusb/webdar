/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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
#include "html_void.hpp"

using namespace std;


string html_void::inherited_get_body_part(const chemin & path,
					  const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_void::my_visibility_has_changed()
{
    html_void_parent_notifier* parent = parent_notifier();

    if(parent != nullptr)
	parent->void_child_visibility_has_changed(this);
}

void html_void::has_adopted(body_builder *obj)
{
    html_void_parent_notifier* parent = parent_notifier();

    if(parent != nullptr)
	parent->void_child_has_adopted(this, obj);
}

void html_void::will_foresake(body_builder *obj)
{
    html_void_parent_notifier* parent = parent_notifier();

    if(parent != nullptr)
	parent->void_child_will_foresake(this, obj);
}

html_void_parent_notifier* html_void::parent_notifier()
{
    return dynamic_cast<html_void_parent_notifier*>(get_parent());
}
