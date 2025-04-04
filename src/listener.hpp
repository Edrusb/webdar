/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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

#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "my_config.h"

    // C++ system header files
#include <libthreadar/libthreadar.hpp>
#include <memory>

    // webdar headers
#include "central_report.hpp"
#include "exceptions.hpp"
#include "authentication.hpp"
#include "ssl_context.hpp"
#include "server_pool.hpp"

    /// class listener

    /// listen to the defined TCP port for new proto_connexions.
    /// Upon new proto_connexion, it builds proto_connexion objects
    /// and assign it to a new server object that will
    /// manage incoming request on this proto_connexion accordingly


class listener : public libthreadar::thread_signal
{
public:
    listener(const std::shared_ptr<central_report> & log,        ///< where to send reports, used but also passed to the generated server objects
	     const std::shared_ptr<const authentication> & auth, ///< where to request for authentications (passed to generated server objects)
	     std::unique_ptr<ssl_context> & ciphering,           ///< if emtpy, a connexion object is provided to the generated server objects else a ssl_connexion is passed instead
	     std::shared_ptr<server_pool> & pool,                ///< the server_pool which will create and manage servers objects for us
	     unsigned int port                                   ///< listen on localhost IPv4 or IPv6
	);
    listener(const std::shared_ptr<central_report> & log,        ///< where to send reports, used but also passed to the generated server objects
	     const std::shared_ptr<const authentication> & auth, ///< where to request for authentications (passed to generated server objects)
	     std::unique_ptr<ssl_context> & ciphering,           ///< if emtpy, a connexion object is provided to the generated server objects else a ssl_connexion is passed instead
	     std::shared_ptr<server_pool> & pool,                ///< the server_pool which will create and manage servers objects for us
	     const std::string & ip,                             ///< interface to listen on
	     unsigned int port                                   ///< port to listen on
	);
    listener(const listener & ref) = delete;
    listener(listener && ref) noexcept = delete;
    listener & operator = (const listener & ref) = delete;
    listener & operator = (listener && ref) noexcept = delete;
    ~listener() { close(sockfd); cancel(); join(); };

protected:
	// inherited from libthreadar::thread;
    virtual void inherited_run() override;

	// no need to override thread::signaled_inherited_cancel()
	// we rely on the cancellation_checkpoint() mechanism
	// in inherited_run()

private:
    std::shared_ptr<central_report> rep;       ///< where to report events
    std::shared_ptr<const authentication> src; ///< where to validate authentications
    int sockfd;                                ///< socket descriptor
    int famille;                               ///< domain familly of the socket
    std::string l_ip;                          ///< listening IP address
    std::string l_port;                        ///< listening port
    std::unique_ptr<ssl_context> ssl_ctx;      ///< ciphering context
    std::shared_ptr<server_pool> srv;          ///< current servers

    void set_sockfd(int domain);
    void init(const std::shared_ptr<central_report> & log,
	      const std::shared_ptr<const authentication> & auth,
	      std::unique_ptr<ssl_context> & ciphering,
	      std::shared_ptr<server_pool> & pool,
	      const std::string & ip,
	      unsigned int port);
};


#endif
