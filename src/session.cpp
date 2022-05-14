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

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "webdar_tools.hpp"
#include "user_interface.hpp"

    //
#include "session.hpp"

using namespace std;

const unsigned int INITIAL_SESSION_ID_WIDTH = 4;
const unsigned int MAXIMUM_SESSION_ID_WIDTH = 20;
const unsigned int MAX_COLLISION = 300;


    //////////////////////////
    //  object method implementation
    //

session::session(): lock_wui(1)
{
    session_ID = "";
    tid = 0;

    wui.record_actor_on_event(this, user_interface::closing);
}

void session::set_session_id(const string & sessid)
{
    session_ID = sessid;
    wui.set_prefix(chemin(sessid));
}

void session::check_caller() const
{
    if(pthread_self() != tid)
	throw WEBDAR_BUG;
    if(session_ID == "")
	throw WEBDAR_BUG;
}

answer session::give_answer(const request & req)
{
    check_caller();

    return wui.give_answer(req);
}

void session::on_event(const std::string & event_name)
{
    if(event_name == user_interface::closing)
	close_session(get_session_ID()); // will kill libdar thread if running
    else
	throw WEBDAR_BUG; // what's that event !?!
}

    //////////////////////////
    //  class fields and methods implementation
    //

libthreadar::mutex session::lock_running;
map<string, session::table> session::running_session;

unsigned int session::get_num_session()
{
    unsigned int ret = 0;

    lock_running.lock();
    try
    {
	ret = running_session.size();
    }
    catch(...)
    {
        lock_running.unlock();
	throw;
    }
    lock_running.unlock();

    return ret;
}

unsigned int session::get_num_session(const string & user)
{
    unsigned int ret = 0;
    map<string, table>::iterator it;

    lock_running.lock();
    try
    {
	for(it = running_session.begin(); it != running_session.end(); ++it)
	{
	    if(it->second.owner == user)
		++ret;
	}
    }
    catch(...)
    {
        lock_running.unlock();
	throw;
    }
    lock_running.unlock();

    return ret;
}


vector<session::session_summary> session::get_summary()
{
    vector<session_summary> ret;
    map<string, table>::iterator it;
    session_summary tmp;

    lock_running.lock();
    try
    {
	it = running_session.begin();
	while(it != running_session.end())
	{
	    if(it->second.reference == NULL)
		throw WEBDAR_BUG;
	    tmp = publish(it);
	    ret.push_back(tmp);
	    ++it;
	}
    }
    catch(...)
    {
	lock_running.unlock();
	throw;
    }
    lock_running.unlock();

    return ret;
}

bool session::get_session_info(const std::string & session_ID, session_summary & val)
{
    bool ret = false;
    map<string, table>::iterator it;

    lock_running.lock();
    try
    {
	it = running_session.find(session_ID);
	if(it != running_session.end())
	{
	    val = publish(it);
	    if(val.session_ID != session_ID)
		throw WEBDAR_BUG;
	    ret = true;
	}

    }
    catch(...)
    {
	lock_running.unlock();
	throw;
    }
    lock_running.unlock();

    return ret;
}

string session::create_new(const string & owner)
{
    table entry;
    string sessID;
    session *obj = new (nothrow) session();
    unsigned int collision = 0;
    unsigned id_width = INITIAL_SESSION_ID_WIDTH;

    if(obj == NULL)
	throw exception_memory();

    try
    {
	entry.clear();

	lock_running.lock();
	try
	{
		// looking whether the new session_ID is not already used
	    do
	    {
		sessID = webdar_tools_generate_random_string(id_width);
		    // OK, this may lead to an endless loop if all sessions
		    // are used so we count up to MAX_FAILURE and then
		    // increase the session_ID length by one
		++collision;
		if(collision > MAX_COLLISION)
		{
		    collision = 0;
		    ++id_width;
		    if(id_width > MAXIMUM_SESSION_ID_WIDTH)
			throw exception_range("Cannot allocate new session, namespace full");
		}
	    }
	    while(running_session.find(sessID) != running_session.end());

	    obj->set_session_id(sessID);
	    entry.owner = owner;
	    entry.reference = obj;

	    running_session[sessID] = entry;
	}
	catch(...)
	{
	    lock_running.unlock();
	    throw;
	}
	lock_running.unlock();
    }
    catch(...)
    {
	if(obj != NULL)
	    delete obj;
	throw;
    }

    return sessID;
}

session *session::acquire_session(const std::string & session_ID)
{
    session *ret = NULL;
    map<string,table>::iterator it;

    lock_running.lock();

    try
    {
	it = running_session.find(session_ID);
	if(it != running_session.end() && !(it->second.closing))
	{
	    if(it->second.reference == NULL)
		throw WEBDAR_BUG;
	    ret = it->second.reference;
	    ++(it->second.ref_given);
	}
    }
    catch(...)
    {
	lock_running.unlock();
	throw;
    }
    lock_running.unlock();

    if(ret != NULL)
    {
	ret->lock_wui.lock(); // eventually waiting for another thread to release the mutex
	ret->tid = pthread_self();
    }

    return ret;
}


void session::release_session(session *sess)
{
    map<string,table>::iterator it;

    if(sess == NULL)
	throw WEBDAR_BUG;

    lock_running.lock();
    try
    {
	it = running_session.begin();

	while(it != running_session.end() && it->second.reference != sess)
	    ++it;

	    // checks
	if(it == running_session.end())
	    throw WEBDAR_BUG; // releasing an unknown session !?!
	sess->check_caller();

	    // all check passed, we can proceed
	--(it->second.ref_given);
	sess->lock_wui.unlock();
	if(it->second.ref_given == 0 && it->second.closing)
	{
	    if(it->second.reference == NULL)
		throw WEBDAR_BUG;
	    if(it->second.reference->has_working_server())
		throw WEBDAR_BUG;
	    delete it->second.reference;
	    running_session.erase(it);
	}
    }
    catch(...)
    {
	lock_running.unlock();
	throw;
    }
    lock_running.unlock();
}


bool session::close_session(const string & session_ID)
{
    lock_running.lock();
    map<string, table>::iterator it;
    bool ret = false;

    try
    {
	it = running_session.find(session_ID);
	if(it != running_session.end())
	{
	    if(!it->second.closing)
	    {
		it->second.closing = true;
		if(it->second.ref_given == 0)
		{
		    if(it->second.reference != NULL)
		    {
			try
			{
			    delete it->second.reference;
			}
			catch(...)
			{
			    running_session.erase(it);
			    throw;
			}
			    running_session.erase(it);
		    }
		    else
			throw WEBDAR_BUG;
		}
		    // else we the object will be destroyed when no more reference will point it
		ret = true; // session will be destroyed as soon as possible
	    }
	    else
		ret = true; // this session end has already been asked
	}
	else
	    ret = false;
    }
    catch(...)
    {
	lock_running.unlock();
	throw;
    }

    lock_running.unlock();

    return ret;
}

session::session_summary session::publish(std::map<std::string, table>::iterator it)
{
    session_summary ret;

    if(it->second.reference == NULL)
	throw WEBDAR_BUG;
    ret.clear();
    ret.owner = it->second.owner;
    ret.session_ID = it->first;
    ret.session_name = it->second.reference->wui.get_session_name();
    ret.locked = it->second.reference->has_working_server();
    ret.libdar_running = it->second.reference->wui.is_libdar_running(); // yes an access without locking the object but read only and on an atomic field
    ret.closing = it->second.closing;

    return ret;
}
