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

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "exceptions.hpp"


    //
#include "web_user_interaction.hpp"


using namespace std;

web_user_interaction::web_user_interaction(unsigned int x_warn_size):
    libdar_sem(1),
    warn_size(x_warn_size)
{
    if(warn_size == 0)
	throw WEBDAR_BUG;
    clear();
}

web_user_interaction::~web_user_interaction()
{
    libdar_sem.unlock();
}

void web_user_interaction::set_warning_list_size(unsigned int size)
{
	// critical section begins
	//
    control.lock();
    try
    {
	warn_size = size;
	while(warnings.size() > size)
	    warnings.pop_front();
    }
    catch(...)
    {
	control.unlock();
	throw;
    }
    control.unlock();
	//
	// critical section ended
}

void web_user_interaction::clear()
{
    control.lock();
    try
    {
	libdar_sem.reset();
	libdar_sem.lock(); // to have libdar thread waiting on it
	answered = false;
	pause_pending = false;
	get_string_pending = false;
	get_secu_string_pending = false;
	warnings.clear();
    }
    catch(...)
    {
	control.unlock();
	throw;
    }
    control.unlock();
}

list<string> web_user_interaction::get_warnings()
{
    list<string> ret;

    	// critical section begins
	//
    control.lock();
    try
    {
	ret = warnings;
    }
    catch(...)
    {
	control.unlock();
	throw;
    }
    control.unlock();
	//
	// critical section ended

    return ret;
}

bool web_user_interaction::pending_pause(string & msg) const
{
    bool ret = false;

    	// critical section begins
	//
    control.lock();
    try
    {
	ret = pause_pending && !answered;
	if(ret)
	    msg = pause_msg;
    }
    catch(...)
    {
	control.unlock();
	throw;
    }
    control.unlock();
	//
	// critical section ended

    return ret;
}

bool web_user_interaction::pending_get_string(string & msg, bool & echo) const
{
    bool ret = false;

    	// critical section begins
	//
    control.lock();
    try
    {
	ret = get_string_pending && !answered;
	if(ret)
	{
	    msg = get_string_msg;
	    echo = get_string_echo;
	}
    }
    catch(...)
    {
	control.unlock();
	throw;
    }
    control.unlock();
	//
	// critical section ended

    return ret;
}

bool web_user_interaction::pending_get_secu_string(string & msg, bool & echo) const
{
    bool ret = false;

    	// critical section begins
	//
    control.lock();
    try
    {
	ret = get_secu_string_pending && !answered;
	if(ret)
	{
	    msg = get_secu_string_msg;
	    echo = get_secu_string_echo;
	}
    }
    catch(...)
    {
	control.unlock();
	throw;
    }
    control.unlock();
	//
	// critical section ended

    return ret;
}

void web_user_interaction::set_pause_answer(bool val)
{
    	// critical section begins
	//
    control.lock();
    try
    {
	if(!pause_pending)
	    throw WEBDAR_BUG;
	pause_ans = val;
	answered = true;
	libdar_sem.unlock(); // freeing the libdar thread
    }
    catch(...)
    {
	control.unlock();
	throw;
    }
    control.unlock();
	//
	// critical section ended

}

void web_user_interaction::set_get_string_answer(const string & val)
{
    	// critical section begins
	//
    control.lock();
    try
    {
	if(!get_string_pending)
	    throw WEBDAR_BUG;
	get_string_ans = val;
	answered = true;
	libdar_sem.unlock(); // feeing the libdar thread
    }
    catch(...)
    {
	control.unlock();
	throw;
    }
    control.unlock();
	//
	// critical section ended

}

void web_user_interaction::set_get_secu_string_answer(const libdar::secu_string & val)
{
    	// critical section begins
	//
    control.lock();
    try
    {
	if(!get_secu_string_pending)
	    throw WEBDAR_BUG;
	get_secu_string_ans = val;
	answered = true;
	libdar_sem.unlock(); // feeing the libdar thread
    }
    catch(...)
    {
	control.unlock();
	throw;
    }
    control.unlock();
	//
	// critical section ended

}

bool web_user_interaction::has_libdar_pending() const
{
	// we do not use a critical section here
	// to access fields pointed to by data
	// because each field is boolean, thus has
	// an atomic *read* operation. The fact also
	// that these three reads may be out of sync
	// relative to one another is not a problem here.

    return (pause_pending
	    || get_string_pending
	    || get_secu_string_pending) && !answered;
}

void web_user_interaction::inherited_message(const string & message)
{
    	// critical section begins
	//
    control.lock();
    try
    {
	warnings.push_back(message);
	while(warnings.size() >= warn_size)
	    warnings.pop_front();
    }
    catch(...)
    {
	control.unlock();
	throw;
    }
    control.unlock();
	//
	// critical section ended
}

bool web_user_interaction::inherited_pause(const string & message)
{
    bool ret;

	// critical section begins
	//
    control.lock();
    try
    {
	if(pause_pending || get_string_pending || get_secu_string_pending)
	    throw WEBDAR_BUG; // already waiting for an answer!
	answered = false;
	pause_msg = message;
	pause_pending = true;
    }
    catch(...)
    {
	control.unlock();
	throw;
    }
    control.unlock();
	//
	// critical section ended

	// waiting on this semaphor for the answer to be filled by the html thread
    libdar_sem.lock();
	// the answer has been filled so we use it to return the info to libdar

	// critical section begins
	//
    control.lock();
    try
    {
	if(!pause_pending)
	    throw WEBDAR_BUG; // answer already read!?!
	pause_pending = false;
	ret = pause_ans;
	pause_msg = "";
    }
    catch(...)
    {
	control.unlock();
	throw;
    }
    control.unlock();
	//
	// critical section ended

    return ret;
}

string web_user_interaction::inherited_get_string(const string & message, bool echo)
{
    string ret;

	// critical section begins
	//
    control.lock();
    try
    {
	if(pause_pending || get_string_pending || get_secu_string_pending)
	    throw WEBDAR_BUG; // already waiting for an answer!
	answered = false;
	get_string_msg = message;
	get_string_echo = echo;
	get_string_pending = true;
    }
    catch(...)
    {
	control.unlock();
	throw;
    }
    control.unlock();
	//
	// critical section ended

	// waiting on this semaphor for the answer to be filled by the html thread
    libdar_sem.lock();
	// the answer has been filled so we use it to return the info to libdar

	// critical section begins
	//
    control.lock();
    try
    {
	if(!get_string_pending)
	    throw WEBDAR_BUG; // answer already read!
	get_string_pending = false;
	ret = get_string_ans;
	get_string_msg = "";
	get_string_ans = "";
    }
    catch(...)
    {
	control.unlock();
	throw;
    }
    control.unlock();
	//
	// critical section ended

    return ret;
}

libdar::secu_string web_user_interaction::inherited_get_secu_string(const string & message, bool echo)
{
    libdar::secu_string ret;

	// critical section begins
	//
    control.lock();
    try
    {
	if(pause_pending || get_string_pending || get_secu_string_pending)
	    throw WEBDAR_BUG; // already waiting for an answer!
	answered = false;
	get_secu_string_msg = message;
	get_secu_string_echo = echo;
	get_secu_string_pending = true;
    }
    catch(...)
    {
	control.unlock();
	throw;
    }
    control.unlock();
	//
	// critical section ended

	// waiting on this semaphor for the answer to be filled by the html thread
    libdar_sem.lock();
	// the answer has been filled so we use it to return the info to libdar

	// critical section begins
	//
    control.lock();
    try
    {
	if(!get_secu_string_pending)
	    throw WEBDAR_BUG; // answer already read!
	get_secu_string_pending = false;
	ret = get_secu_string_ans;
	get_string_msg = "";
	get_string_ans = "";
    }
    catch(...)
    {
	control.unlock();
	throw;
    }
    control.unlock();
	//
	// critical section ended

    return ret;
}
