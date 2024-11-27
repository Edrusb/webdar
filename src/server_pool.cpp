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
#if HAVE_ERRNO_H
#include <errno.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
}

    // C++ system header files
#include <new>

    // libraries header files
#include <libthreadar/libthreadar.hpp>


    // webdar headers
#include "global_parameters.hpp"

    //
#include "server_pool.hpp"

using namespace std;

server_pool::server_pool(const unsigned int pool_size,
			 const shared_ptr<central_report> & creport):
    max_server(pool_size),
    log(creport),
    verrou(1)
{
    if(!log)
	throw WEBDAR_BUG;
    if(max_server < 1)
	throw WEBDAR_BUG;
    run();
	// this launches the local thread (see inherited_run()) for sever
	// object destruction handling
}

server_pool::~server_pool()
{
    try
    {
	cancel();
    }
    catch(...)
    {
	    // no throw
    }

    try
    {
	join();
    }
    catch(...)
    {
	    // no throw
    }
}

bool server_pool::run_new_server(const shared_ptr<const authentication> & auth,
				 unique_ptr<proto_connexion> & source)
{
    bool ret = false;

    if(!auth)
	throw WEBDAR_BUG;
    if(!source)
	throw WEBDAR_BUG;

    verrou.lock();

    try
    {
	    //////////
	    // creating a new server object if allowed
	    // and adding it to the list

	if(size() >= max_server)
	    ret = false;
	else
	{
	    sigset_t sigs;
	    server *tmp = new (nothrow) server(log, auth, source);

	    if(tmp == nullptr)
		throw exception_memory();
	    peer_with(tmp);
		// we only record the allocated address relying on the
		// peering mechanism of class reference (!)

	    if(sigfillset(&sigs) != 0)
		throw exception_system("failed creating a full signal set", errno);
	    if(sigdelset(&sigs, THREAD_SIGNAL) != 0)
		throw exception_system("failed removing the THREAD_SIGNAL from signal set", errno);

	    tmp->set_signal_mask(sigs);
	    tmp->run();
		// we run the object now. The peering we have setup
		// will end once the thread will be about to end, and
		// we will then be notified of receiving a call to
		// broken_peering_from()

	    ret = true;
	}
    }
    catch(...)
    {
	verrou.unlock();
	throw;
    }

    verrou.unlock();

    return ret;
}

void server_pool::inherited_run()
{
	// this thread manages the server objects destruction

    verrou.lock();

    try
    {
	while(max_server > 0    // not asked to stop by our destructor
	      || size() > 0     // or there are still server running
	      || !dying_ones.empty()) // or somme servers need to be deleted
	{
	    try
	    {
		while(dying_ones.size() > 0)
		{
		    if(dying_ones.front() == nullptr)
			throw WEBDAR_BUG;
		    dying_ones.front()->join(); // may throw an exception
		    delete dying_ones.front();  // deleting the object
		    dying_ones.pop_front();     // removing from the list
		}

		if(max_server > 0
		   || size() > 0)
		    verrou.wait(); // release the lock and wait for a signal()
	    }
	    catch(libthreadar::thread::cancel_except & e)
	    {
		throw;
	    }
	    catch(exception_base & e)
	    {
		log->report(priority_t::crit, e.get_message());
		    // no exception propagation
	    }
	}
    }
    catch(...)
    {
	verrou.unlock();
	throw;
    }
    verrou.unlock();
}

void server_pool::signaled_inherited_cancel()
{
    verrou.lock();
    try
    {
	cancel_all_servers(); // ask all servers to end
	max_server = 0; // ask inherited_thread to end asap
	verrou.signal(); // awake the thread.
	    // in case no server are running, the thread
	    // would stay pending on verrou waiting for
	    // a signal() forever
    }
    catch(...)
    {
	    // no exception propagation in destructor
	verrou.unlock();
	throw;
    }
    verrou.unlock();
}


void server_pool::broken_peering_from(reference* obj)
{
    verrou.lock();

    try
    {
	server* dying_server = dynamic_cast<server*>(obj);

	if(dying_server == nullptr)
	    throw WEBDAR_BUG; // was not a server object as peer!

	dying_ones.push_back(dying_server);
	verrou.signal(); // wake up inherited_run() for cleanup of the object
	    // we cannot do this here because broken_peering_from
	    // is called indirectly from the inherited_run() thread of
	    // the server 'dying_server'
    }
    catch(...)
    {
	verrou.unlock();
	throw;
    }
    verrou.unlock();
}

void server_pool::cancel_all_servers()
{
    reference* ptr = nullptr;

    reset_read_peers();
    while(read_next_peer(ptr))
    {
	server* srv = dynamic_cast<server*>(ptr);
	if(srv == nullptr)
	    throw WEBDAR_BUG; // was not a server object as peer!
	srv->cancel();
    }
}
