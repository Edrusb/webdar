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

#ifndef ACTOR_HPP
#define ACTOR_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "reference.hpp"

    /// \file actor.hpp defines actor class
    ///
    /// actor links to events and get triggered upon certain events
    /// an actor object must register for one or more named event of a events object
    /// (using the events::record_actor_on_event() method)
    /// (or of different events objects) to be triggered by a named event
    ///
    /// when a named event occurs the actor is triggered calling its on_event() method
    /// having the name of the event passed as argument

    /// class of object that are pointed/triggered to by others

    /// and that informs these pointers about their imminent death in order
    /// to be removed from the pointed objects list
class actor : public reference
{
public:
    actor() = default;
    actor(const actor & ref) = default;
    actor(actor && ref) noexcept(false) = default;
    actor & operator = (const actor & ref) = default;
    actor & operator = (actor && ref) noexcept(false);
    virtual ~actor() {};

	/// implementation in inherited class of the action triggered by the event given in argument
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

#endif
