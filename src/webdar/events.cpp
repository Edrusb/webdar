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

void events_referrer::add_me_to_referred(events_referred *obj)
{
    if(obj == NULL)
	throw WEBDAR_BUG;
    obj->add_referrer(this);
}

void events_referred::break_all_links()
{
    set<events_referrer *>::iterator it = links.begin();

    while(it != links.end())
    {
	if((*it) == NULL)
	    throw WEBDAR_BUG;
	(*it)->breaking_link(this);
	++it;
    }
    links.clear();
}

void events_trigger::bind(const string & events_name, events_actor *obj)
{
    if(obj == NULL)
	throw WEBDAR_BUG;
    associated[events_name] = obj;
    add_me_to_referred(obj);
}

void events_trigger::breaking_link(const void *ptr)
{
    map<string, events_actor *>::iterator it;

    do
    {
	it = associated.begin();
	while(it != associated.end() && it->second != ptr)
	    ++it;
	if(it != associated.end())
	{
	    associated.erase(it);
	    it = associated.begin();
	}
    }
    while(it != associated.end());
}

void events_trigger::trigger(const string & events_name) const
{
    map<string, events_actor *>::const_iterator it = associated.find(events_name);

    if(it != associated.end())
    {
	if(it->second != NULL)
	    it->second->on_events(events_name);
    }
}
