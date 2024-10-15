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

#ifndef EVENTS_HPP
#define EVENTS_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <set>
#include <map>
#include <string>

    // webdar headers
#include "reference.hpp"
#include "actor.hpp"

    /// \file events.hpp defines the event class

    /// class events

    /// an events object contains a set of events, each one having a name.
    /// an actor object must register for one or more named event of a events object
    /// (or of different events objects) to be triggered by a named event from that object

class events : public reference
{
public:
    events() = default;

	/// copy constructor

	/// \note all events are kept, but all registered
	/// actors to these events are not registered to the
	/// newly created object (see copy constructor of reference class)
    events(const events & ref);
    events(events && ref) noexcept(false) = default;
    events & operator = (const events & ref) = default;
    events & operator = (events && ref) noexcept(false);
    ~events() = default;

	/// record an actor for an given event
    void record_actor_on_event(actor *ptr, const std::string & name);

protected:
	/// inherited from class reference
    virtual void broken_peering_from(reference *obj) override;

	/// add a new event for actors to register against
    void register_name(const std::string & name);

	/// remove an event and remove all actors that registed on it
    void unregister_name(const std::string & name);

	/// rename an event and keep all existing peering to this new event

	/// \note the new name must not already exist for that same object
    void rename_name(const std::string & old_name, const std::string & new_name);

	/// trigger a given event
    void act(const std::string & name);

private:
	/// maps for each event, the list of pending actors
    std::map< std::string , std::list<actor *> > carte;

};

#endif
