/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2023 Denis Corbin
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
#include <utility>

    // webdar headers

    //
#include "bibliotheque.hpp"

using namespace std;

void bibliotheque::begin(category & v)
{
    v = static_cast<category>(0);
}

void bibliotheque::incr(category & v)
{
    if(v > EOE)
	throw WEBDAR_BUG;
    else
	v = static_cast<category>(v+1);
}

bibliotheque::bibliotheque(): text(1, "Configuration page, work under progress")
{
    initialize_content_and_indexes();
    adopt(&text);
}

void bibliotheque::add(category cat, const string & name, const bibli_referable & obj, bool can_replace)
{
    content_index it = content.find(cat);
    if(it == content.end())
	throw WEBDAR_BUG;

    refs_index rit = it->second.refs.find(name);
    if(rit != it->second.refs.end() && ! can_replace)
	throw exception_range("an object of the same name already exists in that category and overwriting disabled, cannot add this item");
    else
	it->second.refs[name] = obj;
}

bool bibliotheque::remove(category cat, const string & name)
{
    content_index it = content.find(cat);
    if(it == content.end())
	throw WEBDAR_BUG;

    refs_index rit = it->second.refs.find(name);
    if(rit != it->second.refs.end()) // object found
    {
	if(rit->second.referred())
	    throw exception_range("cannot remove, the object is still referred by another configuration");
	it->second.refs.erase(rit);
	return true;
    }
    else
	return false;
}

void bibliotheque::reset_read(category cat) const
{
    const_content_index it = content.find(cat);
    if(it == content.end())
	throw WEBDAR_BUG;

    it->second.reset_read();
}

bool bibliotheque::read_next(category cat, string & name) const
{
    const_content_index it = content.find(cat);
    if(it == content.end())
	throw WEBDAR_BUG;

    if(it->second.read_index != it->second.refs.end())
    {
	name = it->second.read_index->first;
	it->second.read_index++;
	return true;
    }
    else
	return false; // end of referable_list
}

bool bibliotheque::find_by_name(category cat, string & name, bibli_referable & found) const
{
    const_content_index it = content.find(cat);
    if(it == content.end())
	throw WEBDAR_BUG;

    const_refs_index rit = it->second.refs.find(name);
    if(rit != it->second.refs.end()) // item found
    {
	found = rit->second;
	return true;
    }
    else
	return false;
}

string bibliotheque::inherited_get_body_part(const chemin & path,
					     const request & req)
{
    return get_body_part_from_all_children(path, req);
}


void bibliotheque::initialize_content_and_indexes()
{
    category cat;

    begin(cat);
    content.clear();
    while(!end(cat))
    {
	content[category(cat)] = referable_list();
	incr(cat);
    }
}

void bibliotheque::reset_read_iterators()
{
    category cat;

    begin(cat);
    while(!end(cat))
    {
	reset_read(category(cat));
	incr(cat);
    }
}
