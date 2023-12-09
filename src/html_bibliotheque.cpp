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
#include "html_bibliotheque.hpp"

using namespace std;

const char* html_bibliotheque::css_tabs = "html_bibliotheque_tabs";

void html_bibliotheque::begin(category & v)
{
    v = static_cast<category>(0);
}

void html_bibliotheque::incr(category & v)
{
    if(v > EOE)
	throw WEBDAR_BUG;
    else
	v = static_cast<category>(v+1);
}

html_bibliotheque::html_bibliotheque()
{
    initialize_content_and_indexes();
    adopt(&tabs);
    tabs.add_tab("File Filters");
    tabs.add_tab("Path Filters");
    tabs.add_tab("Commands");
    tabs.add_tab("Repositories");
    tabs.add_tab("Save Options");
    tabs.add_tab("Test Options");
    tabs.add_tab("Diff Options");
    tabs.add_tab("List Options");
    tabs.add_tab("Restore Options");
    tabs.add_tab("Merge Options");
    tabs.add_tab("Repair Options");
    tabs.add_tab("Common Options");
    for(unsigned int i = 0; i < numtabs; ++i)
    {
	text[i].add_text(1, string("Page under construction: #") + to_string(i));
	tabs.sub_adopt(&(text[i]));
    }
    tabs.add_css_class(css_tabs);
}

void html_bibliotheque::add(category cat, const string & name, const bibli_referable & obj, bool can_replace)
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

bool html_bibliotheque::remove(category cat, const string & name)
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

void html_bibliotheque::reset_read(category cat) const
{
    const_content_index it = content.find(cat);
    if(it == content.end())
	throw WEBDAR_BUG;

    it->second.reset_read();
}

bool html_bibliotheque::read_next(category cat, string & name) const
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

bool html_bibliotheque::find_by_name(category cat, string & name, bibli_referable & found) const
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

string html_bibliotheque::inherited_get_body_part(const chemin & path,
					     const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_bibliotheque::new_css_library_available()
{
    std::unique_ptr<css_library> & csslib = lookup_css_library();

    if(!csslib)
	throw WEBDAR_BUG;

    css tmp;
    tmp.css_width("95%", true);

    if(!csslib->class_exists(css_tabs))
	csslib->add(css_tabs, tmp);
}

void html_bibliotheque::initialize_content_and_indexes()
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

void html_bibliotheque::reset_read_iterators()
{
    category cat;

    begin(cat);
    while(!end(cat))
    {
	reset_read(category(cat));
	incr(cat);
    }
}
