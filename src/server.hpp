/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2023 Denis Corbin
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

#ifndef SERVER_HPP
#define SERVER_HPP

    // C++ system header files
#include <list>
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "parser.hpp"
#include "central_report.hpp"
#include "session.hpp"
#include "authentication.hpp"

    /// thread object that read request from the provided proto_connexion send then to the
    /// appropriated session managing authentication and send back the answer to the session

    /// \note relies on a parser object to split byte flow into structured requests, challenge object
    /// for authentication validation and request, and session class to find and interrogate
    /// the proper session

class server: public libthreadar::thread_signal
{
public:
	// constructor & Destructor are intentionally set as private methods

    static bool run_new_server(const std::shared_ptr<central_report> & log,
			       const std::shared_ptr<const authentication> & auth,
			       std::unique_ptr<proto_connexion> & source);
    static void set_max_server(unsigned int val) { max_server = val; };
    static void kill_server(pthread_t tid);
    static void kill_all_servers();
    static void throw_a_pending_exception();

	/// used by another server to ask this object to release the session it uses
    void release_session() { can_keep_session = false; }; // no need of mutex here, several concurrent call will lead to the same result.

protected:
	/// inherited from libthreadar::thread
    virtual void inherited_run() override;

	// no need to override thread::signaled_inherited_cancel();

private:
    server(const std::shared_ptr<central_report> & log,
	   const std::shared_ptr<const authentication> & auth,
	   std::unique_ptr<proto_connexion> & source);
    server(const server & ref) = delete;
    server(server && ref) noexcept = delete;
    server & operator = (const server & ref) = delete;
    server & operator = (server && ref) noexcept = delete;
    ~server() { cancel(); join(); };

    parser src;              //< this object manages the given proto_connexion in constructor
    std::shared_ptr<central_report> rep; //< where do logs should go
    std::shared_ptr<const authentication> authsrc; //< object to consult for user authentications
    bool can_keep_session;   //< whether aonther object asked interacting with the session we use
    session *locked_session; //< the current session we use (we have acquired its mutex)

	/// static fields
    static libthreadar::mutex lock_counter; //< manages access to all static fields
    static unsigned int max_server;         //< max allowed number of concurrent thread
    static std::list<server *> instances;   //< list of existing server objects
};

#endif
