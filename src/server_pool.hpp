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

#ifndef SERVER_POOL_HPP
#define SERVER_POOL_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "reference.hpp"
#include "central_report.hpp"
#include "authentication.hpp"
#include "proto_connexion.hpp"
#include "server.hpp"

    /// class managing a pool of server objects

class server_pool : public libthreadar::thread_signal, public reference
{
public:
    server_pool(const unsigned int pool_size,
		const std::shared_ptr<central_report> & log);
    server_pool(const server_pool & ref) = delete;
    server_pool(server_pool && ref) noexcept = delete;
    server_pool & operator = (const server_pool & ref) = delete;
    server_pool & operator = (server_pool && ref) noexcept = delete;
    virtual ~server_pool();

    bool run_new_server(const std::shared_ptr<const authentication> & auth,
			std::unique_ptr<proto_connexion> & source);

	// run() method (inherited from libthreadar::thrad_signal to be used
	// to run the server_pool.

protected:

	/// inherited from libthreadar::thread
    virtual void inherited_run() override;

	/// inherited from libthreadar::thread_signal
    virtual void signaled_inherited_cancel() override;

    	/// to be informed when a peer has broken the peering with me

	/// \note
    virtual void broken_peering_from(reference *obj) override;


private:
    unsigned int max_server;             ///< max allowed number of concurrent thread
    std::shared_ptr<central_report> log; ///< the central report
    std::deque<server*> dying_ones;      ///< list of server object that have to be deleted
    libthreadar::condition verrou;       ///< manages access inherited reference class fields
	/// \note this mutex (a condition is a particular mutex) is necessary
	/// because the reference class fields
	/// are accessed and modified from the server thread when the thread
	/// is ending and from other thread (listener class) when creating
	/// a new server object in this server_pool. Moreover several listerner
	/// objects may request server creation on the same server_pool.
	/// \note the condition extension of this mutex is used by the destructor
	/// to be notified when all server object are deleted (broken_peering_from())

    void cancel_all_servers(); ///< must be called from within a critical section on verrou

};

#endif
