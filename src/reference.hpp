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

#ifndef REFERENCE_HPP
#define REFERENCE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <list>

    // webdar headers

class reference
{
public:
    reference() { reset(); }; // default constructor
    reference(const reference & ref); // copy constructor
    reference(reference && ref) noexcept(false); // move constructor
    reference & operator = (const reference & ref); // assignment operator
    reference & operator = (reference && ref) noexcept(false); // move assignment operator
    virtual ~reference() { shut_all_peerings(); };

	/// method used to create a relation between two objects
	///
	/// \note the relation is symetrical [a.peer_with(&b) is the same
	/// as b.peer_with(&a)]. The relation ends when one or the other object
	/// is destroyed. The other object is then notified of this event by
	/// a call to its broken_peering_from() method

    void peer_with(reference *obj);

	/// break the peering with the object given as argument
    void break_peer_with(reference *obj);

	/// whether a peering exists with that object
    bool is_peer(reference *obj) const { return peers.find(obj) != peers.end(); };

	/// whether the current object has peering
    bool is_empty() const { return peers.empty(); };

	/// the number of peers
    unsigned int size() const { return peers.size(); };

protected:

	/// to be informed when a peer has broke the peering with me
    virtual void broken_peering_from(reference *obj) {};


	/// reset the peers reading
    void reset_read_peers() { next_to_read = peers.begin(); };

	/// provide the next peer
	///
	/// \param[in] the address of the next peer upon successful call
	/// \return true if a next peer exists, false else in that case
	/// the argument is undefined
    bool read_next_peer(reference * & peer);

private:
    std::set<reference *>::iterator next_to_read;
    std::set<reference *> peers;
    bool already_moved;

    void reset();
    void shut_all_peerings();
};

#endif
