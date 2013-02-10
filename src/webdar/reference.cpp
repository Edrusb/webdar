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


void reference::peer_with(reference *obj)
{
    if(obj == NULL)
	throw WEBDAR_BUG;

    peers.push_back(obj);
    try
    {
	obj->peers.push_back(this);
    }
    catch(...)
    {
	peers.pop_back();
	throw;
    }
}

void reference::break_peer_from(reference *obj)
{
    list<reference *>::iterator it = peers.begin();
    list<reference *>::iterator ot;

    if(obj == NULL)
	throw WEBDAR_BUG;

    while(it != peers.end() && *it != obj)
	++it;

    if(it == peers.end())
	throw exception_range("Cannot break peering with an object we do not peer with");

    ot = obj->peers.begin();

    while(ot != obj->peers.end() && *ot != this)
	++ot;

    if(ot == obj->peers.end())
	throw WEBDAR_BUG; // asymetrical peering

    try
    {
	peers.erase(it);
    }
    catch(...)
    {
	obj->peers.erase(ot);
	throw;
    }
    obj->peers.erase(ot);

    obj->broken_peering_from(this);
}

bool reference::read_next_peer(reference * & peer)
{
    if(rd != peers.end())
    {
	peer = *rd;
	++rd;
	return true;
    }
    else
	return false;
}
