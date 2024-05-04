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


    // webdar headers
#include "exceptions.hpp"


    //
#include "events.hpp"

using namespace std;

events & events::operator = (events && ref) noexcept(false)
{
    static_cast<reference>(*this) = std::move(static_cast<reference>(ref));
    carte = std::move(ref.carte); // invoking the move assignment for std::map

    return *this;
}

void events::record_actor_on_event(actor *ptr, const string & name)
{
    map < string , list<actor *> >::iterator it = carte.find(name);

    if(it == carte.end())
	throw WEBDAR_BUG; // unknown event !

    if(ptr == nullptr)
	throw WEBDAR_BUG;

	// shouldn't we first check the ptr actor is not already
	// registered on this object, before adding it to the list?
    it->second.push_back(ptr);
    try
    {
	peer_with(ptr);
    }
    catch(...)
    {
	    // nothing to do, this failure just indicates
	    // that the peer is already recorded on another event on
	    // that events objects
    }
}

void events::broken_peering_from(reference *obj)
{
    map< string, list<actor *> >::iterator it = carte.begin();

    while(it != carte.end())
    {
	list<actor *>::iterator ptr = it->second.begin();
	while(ptr != it->second.end())
	{
	    if(*ptr == nullptr)
		throw WEBDAR_BUG;
	    if(*ptr == obj)
		ptr = it->second.erase(ptr);
	    else
		++ptr;
	}
	++it;
    }
}

void events::register_name(const string & name)
{
    map < string , list<actor *> >::iterator it = carte.find(name);

    if(it != carte.end())
	throw WEBDAR_BUG; // events already registered
    else
    {
	list<actor *> tmp; // we start with an empty list of actors...
	carte[name] = tmp;
    }
}

void events::unregister_name(const string & name)
{
    map< string , list<actor *> >::iterator it = carte.find(name);

    if(it == carte.end())
	throw WEBDAR_BUG; // unknown event!

    for(list<actor*>::iterator ptr = it->second.begin(); ptr != it->second.end(); ++ptr)
	break_peer_with(*ptr);

    carte.erase(it);
}

void events::rename_name(const string & old_name, const string & new_name)
{
    map< string, list<actor*> >::iterator it = carte.find(new_name);

    if(old_name == new_name)
	return; // nothing to do

    if(it != carte.end())
	throw WEBDAR_BUG; // an event of that new name already exists

    it = carte.find(old_name);
    if(it == carte.end())
	throw WEBDAR_BUG; // unknown event name to be renamed

    carte[new_name] = it->second;
    carte.erase(it);
}

void events::act(const string & name)
{
    map < string , list<actor *> >::iterator it = carte.find(name);
    list<actor *>::iterator ptr;

    if(it == carte.end())
	throw WEBDAR_BUG; // unknown event!

    for(ptr = it->second.begin(); ptr != it->second.end(); ++ptr)
    {
	if(*ptr == nullptr)
	    throw WEBDAR_BUG;
	(*ptr)->on_event(name);
    }
}
