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
#include "exceptions.hpp"


    //
#include "css_class_group.hpp"

using namespace std;

void css_class_group::add_css_class(const string & name)
{
    if(content.find(name) != content.end())
	throw exception_range(string("the css_class name to add is already present: ") + name);

    content.insert(name);
}

void css_class_group::add_css_class(const css_class_group & cg)
{
    set<string>::iterator it = cg.content.begin();

    while(it != cg.content.end())
    {
	add_css_class(*it);
	++it;
    }
}

void css_class_group::remove_css_class(const string & name)
{
    if(content.find(name) == content.end())
	throw exception_range(string("the css_class name to remove is not present in the list: ") + name);

    content.erase(name);
    reader = content.begin();
}

void css_class_group::clear_css_classes()
{
    content.clear();
    reader = content.begin();
}

void css_class_group::reset_read() const
{
    reader = content.begin();
}

bool css_class_group::read_next(string & next_class) const
{
    if(reader == content.end())
	return false;
    else
    {
	next_class = *reader;
	++reader;
	return true;
    }
}

