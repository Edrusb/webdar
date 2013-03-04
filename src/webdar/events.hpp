#ifndef EVENTS_HPP
#define EVENTS_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <set>
#include <map>
#include <string>

    // webdar headers
#include "reference.hpp"

    /// \file events.hpp defines 2 classes:
    //. actor that get triggered upon certain events
    //. a collection of events an actor may register against for notification

    /// actors are triggered by events
    /// when an event occurrs the actor is triggered calling its on_event() method
class actor : public reference
{
public:
    virtual ~actor() {};

    virtual void on_event(const std::string & event_name) = 0;
};


    /// class of object that are pointed to by others
    /// and that informs these pointers about their imminent death
class events : public reference
{
public:

	/// add a new event actors can register against
    void register_name(const std::string & name);

	/// record an actor for an given event
    void record_actor_on_event(actor *ptr, const std::string & name);

	/// trigger a given event
    void act(const std::string & name);

	/// inherited from class reference
    void broken_peering_from(reference *obj);


private:
    std::map< std::string , std::list<actor *> > carte;

};


#endif
