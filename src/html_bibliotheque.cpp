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
	tabs.adopt_in_section(i, &(text[i]));
    }
    tabs.add_css_class(css_tabs);
}


string html_bibliotheque::inherited_get_body_part(const chemin & path,
					     const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_bibliotheque::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();

    if(!csslib)
	throw WEBDAR_BUG;

    css tmp;
    tmp.css_width("95%", true);

    if(!csslib->class_exists(css_tabs))
	csslib->add(css_tabs, tmp);
}
