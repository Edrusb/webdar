    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "exceptions.hpp"


    //
#include "reference.hpp"

using namespace std;


reference::reference(const reference & ref)
{
    if(ref.is_empty())
	reset();
    else
	throw WEBDAR_BUG; // cannot copy-construct an object already linked with another one
}

const reference & reference::operator = (const reference & ref)
{
    if(ref.is_empty())
    {
	shut_all_peerings();
	reset();
    }
    else
	throw WEBDAR_BUG;
}


void reference::peer_with(reference *obj)
{
    if(obj == NULL)
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

void reference::break_peer_with(reference *obj)
{
    if(obj == NULL)
	throw WEBDAR_BUG;

    if(is_peer(obj))
    {
	try
	{
	    peers.erase(obj);
	}
	catch(...)
	{
	    obj->peers.erase(this);
	    throw;
	}
	obj->peers.erase(this);
	obj->broken_peering_from(this);
    }
    else
	throw WEBDAR_BUG;
}


bool reference::read_next_peer(reference * & peer)
{
    if(next_to_read != peers.end())
    {
	peer = *next_to_read;
	++next_to_read;
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
	if(*(peers.begin()) == NULL)
	    throw WEBDAR_BUG;
	break_peer_with(*(peers.begin()));
    }

    reset();
}

