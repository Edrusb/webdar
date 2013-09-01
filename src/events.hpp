/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013 Denis Corbin
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
    //. events which is a collection of events
    /// events contains a set of labelled events, each one having a name
    /// an actor object must register for one or more named event of a events object
    /// (or of different events objects) to be triggered by a named event
    ///
    /// when a named event occurs the actor is triggered calling its on_event() method
    /// having the name of the event passed as argument

    /// class of object that are pointed/triggered to by others
    ///
    /// and that informs these pointers about their imminent death in order
    /// to be removed from the pointed objects list
class actor : virtual public reference
{
public:
    virtual ~actor() {};

    virtual void on_event(const std::string & event_name) = 0;
};


// Note: both class actor and events inherit from the same class "reference"
// It is possible that a class inherit from both actor and events, being an
// actor with certain peers and a events for other peers. Having non virtual
// inheritance for actor and events in relation to class reference would not
// be a problem, as it would lead such object to have two parent reference
// objects one containing the peer with which it is actor, and another with
// which it is event. However, as both type of relation are forever, having
// a single base for both roles is possible. It also reduces a bit memory
// requirement.

class events : virtual public reference
{
public:
	/// record an actor for an given event
    void record_actor_on_event(actor *ptr, const std::string & name);

protected:
	/// inherited from class reference
    void broken_peering_from(reference *obj);

	/// add a new event for actors to register against
    void register_name(const std::string & name);

	/// trigger a given event
    void act(const std::string & name);

private:
    std::map< std::string , std::list<actor *> > carte;

};


#endif
