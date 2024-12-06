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
#include "choose.hpp"
#include "reference.hpp"

    /// class server for TCP session management

    /// thread object that read request from the provided proto_connexion, send them to the
    /// appropriated session, managing authentication and sending back the answer to the browser
    /// at the other end of the proto_connexion.

    /// \note relies on a parser object to split byte flow into structured requests, challenge object
    /// for authentication validation of requests, and session class to find and interrogate
    /// the proper session. Server are spaws when an new TCP connection is received, while sessions
    /// objects stay alive accross TCP connections and are tear down on by user action on through the web
    /// interface.

class server: public libthreadar::thread_signal,
	      public reference // this inheritance is used to notify server_pool objects
{
public:
    server(const std::shared_ptr<central_report> & creport,
	   const std::shared_ptr<const authentication> & auth,
	   std::unique_ptr<proto_connexion> & source);
    server(const server & ref) = delete;
    server(server && ref) noexcept = delete;
    server & operator = (const server & ref) = delete;
    server & operator = (server && ref) noexcept = delete;
    ~server() { cancel(); join(); };

	/// used by another server to ask this object to release the session it uses
    void release_session() { can_keep_session = false; }; // no need of mutex here, several concurrent call will lead to the same result.


	/// wether to emulate user logout while using basic authentication (see also class html_disconnect)
    static void force_disconnection_at_end_of_session(bool val) { default_basic_auth = ! val; };


protected:

	/// inherited from libthreadar::thread
    virtual void inherited_run() override;

	// no need to override thread::signaled_inherited_cancel();

private:

    enum auth_consideration
    {
	ignore_auth_redir,  ///< user has just disconnected and will be redirected to steady page
	ignore_auth_steady, ///< user has to be redirected to the steady page
	no_ignore           ///< user has authenticated and can access webdar
    };

    parser src;                          ///< this object manages the given proto_connexion in constructor
    std::shared_ptr<central_report> rep; ///< where do logs should go
    std::shared_ptr<const authentication> authsrc; ///< object to consult for user authentications
    bool can_keep_session;               ///< whether another object asked interacting with the session we use
    session* locked_session;             ///< the current session we use (we have acquired its mutex)
    auth_consideration ignore_auth;      ///< how to consider authentication info in request

    void end_all_peers();

    static bool default_basic_auth;     ///< if true, no disconnection is provided (unless browser is restarted)

};

#endif
