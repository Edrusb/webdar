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
#include "html_over_guichet.hpp"

using namespace std;

const string html_over_guichet::changed = "hog_changed";

html_over_guichet::html_over_guichet():
    wicket(true)
{
	// component config

	// adoption tree (is dynamically made through set_child() method

	// events
    register_name(changed);

	// css
}

void html_over_guichet::set_child(const shared_ptr<bibliotheque> & ptr,
				  unique_ptr<body_builder> & to_give,
				  bibliotheque::category cat,
				  const string & changed_event)
{
    if(!ptr)
	throw WEBDAR_BUG;

    inner = std::move(to_give);
    if(!inner)
	throw WEBDAR_BUG;

	// component configuration

    wicket.set_child(ptr,
		     cat,
		     inner,
		     false);


	// adoption tree

    adopt(&wicket);

	// events
    if(!changed_event.empty())
    {
	events* ptr = dynamic_cast<events*>(inner.get());
	if(ptr == nullptr)
	    throw WEBDAR_BUG; // if an event is provided the to_give object must inherit from events class !
	child_event = changed_event; // for sanity check in on_event();
	ptr->record_actor_on_event(this, changed_event);
    }

	// css

}

unique_ptr<libdar::mask> html_over_guichet::get_mask() const
{
    check_inner();

    const html_mask* inner_mask = dynamic_cast<const html_mask*>(inner.get());
    if(inner_mask == nullptr)
	throw WEBDAR_BUG;

    return inner_mask->get_mask();
}

void html_over_guichet::load_json(const json & source)
{
    wicket.load_json(source);
}

json html_over_guichet::save_json() const
{
    return wicket.save_json();
}

void html_over_guichet::clear_json()
{
    wicket.clear_json();
}

bibliotheque::using_set html_over_guichet::get_using_set() const
{
    return wicket.get_using_set();
}

void html_over_guichet::on_event(const string & event_name)
{
    if(event_name == child_event)
	act(changed);
    else
	throw WEBDAR_BUG;
}


string html_over_guichet::inherited_get_body_part(const chemin & path,
						  const request & req)
{
    return get_body_part_from_all_children(path, req);
}

