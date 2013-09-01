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

}

    // C++ system header files


    // webdar headers



    //
#include "web_user_interaction.hpp"


using namespace std;

web_user_interaction::web_user_interaction(unsigned int x_warn_size)
{
    data = new (nothrow) shared_data(x_warn_size);
    if(data == NULL)
	throw exception_memory();
}

void web_user_interaction::set_warning_list_size(unsigned int size)
{
    check_data();

	// critical section begins
	//
    data->control.lock();
    try
    {
	data->warn_size = size;
	while(data->warnings.size() > size)
	    data->warnings.pop_front();
    }
    catch(...)
    {
	data->control.unlock();
	throw;
    }
    data->control.unlock();
	//
	// critical section ended
}

void web_user_interaction::clear()
{
    check_data();

    data->control.lock();
    try
    {
	data->clear();
    }
    catch(...)
    {
	data->control.unlock();
	throw;
    }
    data->control.unlock();
}

bool web_user_interaction::pause2(const string & message)
{
    bool ret;

    check_data();

	// critical section begins
	//
    data->control.lock();
    try
    {
	if(data->pause2_pending || data->get_string_pending || data->get_secu_string_pending)
	    throw WEBDAR_BUG; // already waiting for an answer!
	data->answered = false;
	data->pause2_msg = message;
	data->pause2_pending = true;
    }
    catch(...)
    {
	data->control.unlock();
	throw;
    }
    data->control.unlock();
	//
	// critical section ended

	// waiting on this semaphor for the answer to be filled by the html thread
    data->libdar_sem.lock();
	// the answer has been filled so we use it to return the info to libdar

	// critical section begins
	//
    data->control.lock();
    try
    {
	if(!data->pause2_pending)
	    throw WEBDAR_BUG; // answer already read!?!
	data->pause2_pending = false;
	ret = data->pause2_ans;
	data->pause2_msg = "";
    }
    catch(...)
    {
	data->control.unlock();
	throw;
    }
    data->control.unlock();
	//
	// critical section ended

    return ret;
}

string web_user_interaction::get_string(const string & message, bool echo)
{
    string ret;

    check_data();

	// critical section begins
	//
    data->control.lock();
    try
    {
	if(data->pause2_pending || data->get_string_pending || data->get_secu_string_pending)
	    throw WEBDAR_BUG; // already waiting for an answer!
	data->answered = false;
	data->get_string_msg = message;
	data->get_string_echo = echo;
	data->get_string_pending = true;
    }
    catch(...)
    {
	data->control.unlock();
	throw;
    }
    data->control.unlock();
	//
	// critical section ended

	// waiting on this semaphor for the answer to be filled by the html thread
    data->libdar_sem.lock();
	// the answer has been filled so we use it to return the info to libdar

	// critical section begins
	//
    data->control.lock();
    try
    {
	if(!data->get_string_pending)
	    throw WEBDAR_BUG; // answer already read!
	data->get_string_pending = false;
	ret = data->get_string_ans;
	data->get_string_msg = "";
	data->get_string_ans = "";
    }
    catch(...)
    {
	data->control.unlock();
	throw;
    }
    data->control.unlock();
	//
	// critical section ended

    return ret;
}

libdar::secu_string web_user_interaction::get_secu_string(const string & message, bool echo)
{
    libdar::secu_string ret;

    check_data();

	// critical section begins
	//
    data->control.lock();
    try
    {
	if(data->pause2_pending || data->get_string_pending || data->get_secu_string_pending)
	    throw WEBDAR_BUG; // already waiting for an answer!
	data->answered = false;
	data->get_secu_string_msg = message;
	data->get_secu_string_echo = echo;
	data->get_secu_string_pending = true;
    }
    catch(...)
    {
	data->control.unlock();
	throw;
    }
    data->control.unlock();
	//
	// critical section ended

	// waiting on this semaphor for the answer to be filled by the html thread
    data->libdar_sem.lock();
	// the answer has been filled so we use it to return the info to libdar

	// critical section begins
	//
    data->control.lock();
    try
    {
	if(!data->get_secu_string_pending)
	    throw WEBDAR_BUG; // answer already read!
	data->get_secu_string_pending = false;
	ret = data->get_secu_string_ans;
	data->get_string_msg = "";
	data->get_string_ans = "";
    }
    catch(...)
    {
	data->control.unlock();
	throw;
    }
    data->control.unlock();
	//
	// critical section ended

    return ret;
}


libdar::user_interaction *web_user_interaction::clone() const
{
    libdar::user_interaction *ret = new (nothrow) web_user_interaction(*this);
    if(ret == NULL)
	throw libdar::Ememory("web_user_interaction");

    return ret;
}

list<string> web_user_interaction::get_warnings()
{
    list<string> ret;

    check_data();
    	// critical section begins
	//
    data->control.lock();
    try
    {
	ret = data->warnings;
    }
    catch(...)
    {
	data->control.unlock();
	throw;
    }
    data->control.unlock();
	//
	// critical section ended

    return ret;
}

bool web_user_interaction::pending_pause2(string & msg) const
{
    bool ret = false;

    check_data();

    	// critical section begins
	//
    data->control.lock();
    try
    {
	ret = data->pause2_pending && !data->answered;
	if(ret)
	    msg = data->pause2_msg;
    }
    catch(...)
    {
	data->control.unlock();
	throw;
    }
    data->control.unlock();
	//
	// critical section ended

    return ret;
}

bool web_user_interaction::pending_get_string(string & msg, bool & echo) const
{
    bool ret = false;

    check_data();

    	// critical section begins
	//
    data->control.lock();
    try
    {
	ret = data->get_string_pending && !data->answered;
	if(ret)
	{
	    msg = data->get_string_msg;
	    echo = data->get_string_echo;
	}
    }
    catch(...)
    {
	data->control.unlock();
	throw;
    }
    data->control.unlock();
	//
	// critical section ended

    return ret;
}

bool web_user_interaction::pending_get_secu_string(string & msg, bool & echo) const
{
    bool ret = false;

    check_data();

    	// critical section begins
	//
    data->control.lock();
    try
    {
	ret = data->get_secu_string_pending && !data->answered;
	if(ret)
	{
	    msg = data->get_secu_string_msg;
	    echo = data->get_secu_string_echo;
	}
    }
    catch(...)
    {
	data->control.unlock();
	throw;
    }
    data->control.unlock();
	//
	// critical section ended

    return ret;
}

void web_user_interaction::set_pause2_answer(bool val)
{
    check_data();

    	// critical section begins
	//
    data->control.lock();
    try
    {
	if(!data->pause2_pending)
	    throw WEBDAR_BUG;
	data->pause2_ans = val;
	data->answered = true;
	data->libdar_sem.unlock(); // freeing the libdar thread
    }
    catch(...)
    {
	data->control.unlock();
	throw;
    }
    data->control.unlock();
	//
	// critical section ended

}

void web_user_interaction::set_get_string_answer(const string & val)
{
    check_data();

    	// critical section begins
	//
    data->control.lock();
    try
    {
	if(!data->get_string_pending)
	    throw WEBDAR_BUG;
	data->get_string_ans = val;
	data->answered = true;
	data->libdar_sem.unlock(); // feeing the libdar thread
    }
    catch(...)
    {
	data->control.unlock();
	throw;
    }
    data->control.unlock();
	//
	// critical section ended

}

void web_user_interaction::set_get_secu_string_answer(const libdar::secu_string & val)
{
    check_data();

    	// critical section begins
	//
    data->control.lock();
    try
    {
	if(!data->get_secu_string_pending)
	    throw WEBDAR_BUG;
	data->get_secu_string_ans = val;
	data->answered = true;
	data->libdar_sem.unlock(); // feeing the libdar thread
    }
    catch(...)
    {
	data->control.unlock();
	throw;
    }
    data->control.unlock();
	//
	// critical section ended

}

bool web_user_interaction::has_libdar_pending() const
{
    check_data();

	// we do not use a critical section here
	// to access fields pointed to by data
	// because each field is boolean, thus has
	// an atomic *read* operation. The fact also
	// that these three reads may be out of sync
	// relative to one another is not a problem here.

    return (data->pause2_pending
	    || data->get_string_pending
	    || data->get_secu_string_pending) && !data->answered;
}

void web_user_interaction::inherited_warning(const string & message)
{
    check_data();

    	// critical section begins
	//
    data->control.lock();
    try
    {
	data->warnings.push_back(message);
	while(data->warnings.size() >= data->warn_size)
	    data->warnings.pop_front();
    }
    catch(...)
    {
	data->control.unlock();
	throw;
    }
    data->control.unlock();
	//
	// critical section ended
}

void web_user_interaction::check_data() const
{
    if(data == NULL)
	throw WEBDAR_BUG;
}

void web_user_interaction::copy_from(const web_user_interaction & ref)
{
    data = const_cast<web_user_interaction *>(&ref)->data;

    check_data();
    data->incr();
}

void web_user_interaction::detruit()
{
    check_data();
    if(data->decr_and_can_delete())
	delete data;
    data = NULL; // in any case
}



///////////// shared_data methods //////////////////

web_user_interaction::shared_data::shared_data(unsigned int x_warn_size)
{
    instances = 1;
    warn_size = x_warn_size;
    if(warn_size == 0)
	throw WEBDAR_BUG;
    clear();
}

web_user_interaction::shared_data::~shared_data()
{
    if(instances == 0)
    {
	libdar_sem.unlock();
	if(pause2_pending || get_string_pending || get_secu_string_pending)
	    throw WEBDAR_BUG;
    }
    else
	throw WEBDAR_BUG;
}

void web_user_interaction::shared_data::clear()
{
    libdar_sem.reset();
    libdar_sem.lock(); // to have libdar thread waiting on it
    answered = false;
    pause2_pending = false;
    get_string_pending = false;
    get_secu_string_pending = false;
    warnings.clear();
}

void web_user_interaction::shared_data::incr()
{

    	// critical section begins
	//
    control.lock();
    try
    {
	++instances;
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

bool web_user_interaction::shared_data::decr_and_can_delete()
{
    bool ret = false;

    	// critical section begins
	//
    control.lock();
    try
    {
	if(instances == 0)
	    throw WEBDAR_BUG;
	--instances;
	ret = (instances == 0);
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
