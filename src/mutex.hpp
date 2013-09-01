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

#ifndef MUTEX_HPP
#define MUTEX_HPP

    // C system header files
extern "C"
{
#include <pthread.h>
}

    // webdar headers
#include "exceptions.hpp"

class mutex
{
public:
    mutex();
    mutex(const mutex & ref) { throw WEBDAR_BUG; };
    const mutex & operator = (const mutex & ref) { throw WEBDAR_BUG; };
    ~mutex();

	/// lock the mutex
	///
	/// \note this call puts the caller in sleep until the resource is released by
	/// another thread
    void lock();

	/// unlock the mutex
	///
	/// \note a single thread suspended on this mutex is then awaken
    void unlock();

	/// Tells whether calling lock() would currently suspend the caller or not
	///
	/// \return true if lock is acquired false if mutex is already locked
    bool try_lock();

private:
    pthread_mutex_t mut; //< the mutex
};

#endif

