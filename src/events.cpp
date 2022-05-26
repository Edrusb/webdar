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

void events::register_name(const string & name)
{
    map < string , list<actor *> >::iterator it = carte.find(name);

    if(it != carte.end())
	throw WEBDAR_BUG; // events already registered
    else
    {
	list<actor *> tmp;
	carte[name] = tmp;
    }
}

void events::record_actor_on_event(actor *ptr, const string & name)
{
    map < string , list<actor *> >::iterator it = carte.find(name);

    if(it == carte.end())
	throw WEBDAR_BUG; // unknown event !

    if(ptr == NULL)
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


void events::act(const std::string & name)
{
    map < string , list<actor *> >::iterator it = carte.find(name);
    list<actor *>::iterator ptr;

    if(it == carte.end())
	throw WEBDAR_BUG; // unknown event !

    for(ptr = it->second.begin(); ptr != it->second.end(); ++ptr)
    {
	if(*ptr == NULL)
	    throw WEBDAR_BUG;
	(*ptr)->on_event(name);
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
	    if(*ptr == NULL)
		throw WEBDAR_BUG;
	    if(*ptr == obj)
		ptr = it->second.erase(ptr);
	    else
		++ptr;
	}
	++it;
    }
}

