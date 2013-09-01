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

    // C system header files
extern "C"
{
#include <errno.h>
}

    // webdar headers
#include "mutex.hpp"

mutex::mutex()
{
    int ret = pthread_mutex_init(&mut, NULL);
    if(ret != 0)
	throw exception_system("Error while creating mutex", ret);
}

mutex::~mutex()
{
    (void)pthread_mutex_destroy(&mut);
}

void mutex::lock()
{
    switch(pthread_mutex_lock(&mut))
    {
    case 0:
	break;
    case EINVAL:
	throw WEBDAR_BUG;
    case EDEADLK:
	throw WEBDAR_BUG;
    case EPERM:
	throw WEBDAR_BUG;
    default:
	throw WEBDAR_BUG;
    }
}

void mutex::unlock()
{
    if(pthread_mutex_unlock(&mut) != 0)
	throw WEBDAR_BUG;
}


bool mutex::try_lock()
{
    int ret = pthread_mutex_trylock(&mut);
    if(ret != 0 && ret != EBUSY)
	throw exception_system("Error while trying locking mutex", ret);

    return ret == 0;
}

