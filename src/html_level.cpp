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
#include "html_level.hpp"

using namespace std;


html_level::html_level()
{
    nxt = table.begin();
}


void html_level::adopt_static_html(const std::string & html)
{
    bundle tmp;

    tmp.obj = nullptr;
    tmp.static_text = html;

    table.push_back(tmp);
    reset_read_next();
}

void html_level::clear()
{
    orphan_all_children();
    table.clear();
    reset_read_next();
}

void html_level::has_adopted(body_builder *obj)
{
    bundle tmp;

    if(obj == nullptr)
	throw WEBDAR_BUG;
    tmp.obj = obj;
    table.push_back(tmp);
    reset_read_next();
}

void html_level::will_foresake(body_builder *obj)
{
    list<bundle>::iterator it = table.begin();

    while(it != table.end() && it->obj != obj)
	++it;

    if(it != table.end())
    {
	if(nxt == it)
	    ++nxt;   // pass over the object about to be removed
	table.erase(it);
    }
}

bool html_level::read_next(bundle & bdl)
{
    if(nxt != table.end())
    {
	bdl = (*nxt);
	++nxt;
	return true;
    }
    else
	return false;
}

string html_level::get_body_part_from_children_as_a_block(const chemin & path,
							  const request & req)
{
    string ret = "";
    chemin sub_path = path;
    bundle bdl;

    if(sub_path.size() > 0)
	sub_path.pop_front();

    reset_read_next();
    while(read_next(bdl))
    {
	if(bdl.obj != nullptr)
	    ret += bdl.obj->get_body_part(sub_path, req);
	else
	    ret += bdl.static_text;
    }

    return ret;
}

string html_level::inherited_get_body_part(const chemin & path,
					   const request & req)
{
    return get_body_part_from_children_as_a_block(path, req);
}
