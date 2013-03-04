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

