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
#include "actor.hpp"

    /// \file events.hpp defines the event class
    ///
    /// an events object contains a set of labelled events, each one having a name.
    /// an actor object must register for one or more named event of a events object
    /// (or of different events objects) to be triggered by a named event

class events : virtual public reference
{
public:
    events() = default;
    events(const events & ref) = default;
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

	/// trigger a given event
    void act(const std::string & name);

private:
    std::map< std::string , std::list<actor *> > carte;

};

#endif
