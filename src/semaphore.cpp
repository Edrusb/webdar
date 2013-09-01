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

#include "semaphore.hpp"

semaphore::semaphore()
{
    value = 1;
}

semaphore::~semaphore()
{
    if(value != 1)
	throw WEBDAR_BUG;
}

bool semaphore::waiting_thread() const
{
    return value < 0; // reading of integer is atomic CPU single operation, no need to lock val_mutex
}

bool semaphore::working_thread() const
{
    return value < 1; // reading of integer is atomic CPU single operation, no need to lock val_mutex
}

void semaphore::lock()
{
    val_mutex.lock();
    --value;
    val_mutex.unlock();
    semaph.lock();
}

void semaphore::unlock()
{
    val_mutex.lock();
    try
    {
	if(value == 1)
	    throw WEBDAR_BUG;
	++value;
    }
    catch(...)
    {
	val_mutex.unlock();
	throw;
    }
    val_mutex.unlock();
    semaph.unlock();
}

void semaphore::reset()
{
    val_mutex.lock();
    try
    {
	while(value < 1)
	{
	    semaph.unlock();
	    ++value;
	}
    }
    catch(...)
    {
	val_mutex.unlock();
	throw;
    }
    val_mutex.unlock();
}




