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
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "exceptions.hpp"


    //
#include "reference.hpp"

using namespace std;

reference::reference(reference && ref) noexcept(false)
{
    if(ref.is_empty())
	reset();
    else
	throw WEBDAR_BUG; // cannot copy-construct an object already linked with another one
}


reference & reference::operator = (const reference & ref)
{
    if(ref.is_empty())
	shut_all_peerings();
    else
	throw WEBDAR_BUG;

    return *this;
}

reference & reference::operator = (reference && ref) noexcept(false)
{
    if(ref.is_empty())
	shut_all_peerings();
    else
	throw WEBDAR_BUG;

    return *this;
}

void reference::peer_with(reference *obj)
{
    if(obj == nullptr)
	throw WEBDAR_BUG;

    if(is_peer(obj))
    {
	if(!obj->is_peer(this))
	    throw WEBDAR_BUG;
    }
    else
    {
	peers.insert(obj);
	try
	{
	    obj->peers.insert(this);
	}
	catch(...)
	{
	    peers.erase(obj);
	    throw;
	}
    }
}

void reference::break_peer_with(reference* obj)
{
    if(obj == nullptr)
	throw WEBDAR_BUG;

    if(is_peer(obj))
    {
	try
	{
	    erase_with_coherence(obj);
	}
	catch(...)
	{
	    obj->erase_with_coherence(this);
	    throw;
	}
	obj->erase_with_coherence(this);
	obj->broken_peering_from(this);
    }
    else
	throw WEBDAR_BUG;
}


bool reference::read_next_peer(reference * & peer) const
{
    if(next_to_read != peers.end())
    {
	peer = *next_to_read;
	++next_to_read;
	return true;
    }
    else
	return false;
}


void reference::reset()
{
    peers.clear();
    next_to_read = peers.begin();
}


void reference::shut_all_peerings()
{
    while(peers.begin() != peers.end())
    {
	if(*(peers.begin()) == nullptr)
	    throw WEBDAR_BUG;
	break_peer_with(*(peers.begin()));
    }

    reset();
}

void reference::erase_with_coherence(reference* ptr)
{
    set<reference*>::iterator it = peers.begin();

    if(ptr == nullptr)
	throw WEBDAR_BUG;

    while(it != peers.end()
	  && *it != ptr)
	++it;

    if(it == peers.end())
	throw WEBDAR_BUG;
	// asking to erase an non-peered reference object

    if(it == next_to_read) // erasing the next to read object
	++next_to_read;
	// seeking past the element that we will
	// erase here:

    peers.erase(it);
}
