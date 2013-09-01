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

#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include <semaphore.h>

#include "exceptions.hpp"
#include "mutex.hpp"

    /// class semaphore is used as replacement of sem_t type in order to detect whether other
    /// thread than the one currently having the lock are waiting for the semaphore to be released

class semaphore
{
public:
    semaphore();
    semaphore(const semaphore & ref) { throw WEBDAR_BUG; };
    const semaphore & operator = (const semaphore & ref) { throw WEBDAR_BUG; };
    ~semaphore();

	/// return whether the semaphore has at least a pending thread waiting for another thread to unlock it
    bool waiting_thread() const;


	/// return whether the semaphore is locked (may it has pending thread or not)
    bool working_thread() const;

	/// request lock
	///
	/// \note this call puts the caller in sleep until the resource semaphore is released by
	/// another thread
    void lock();

	/// release lock
	///
	/// \note a single thread suspended on this semaphore is then awaken
    void unlock();

	/// reset to initial state releasing any thread that could wait on us
    void reset();

private:
    int value;       //< this is the semaphore value
    mutex val_mutex; //< this controls modification to value
    mutex semaph;    //< this is mutex is used to suspend thread when more than one is requesting the lock
};


#endif
