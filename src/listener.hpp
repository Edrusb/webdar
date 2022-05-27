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

#ifndef LISTENER_HPP
#define LISTENER_HPP

    // C++ system header files
#include <libthreadar/libthreadar.hpp>


    // webdar headers
#include "central_report.hpp"
#include "exceptions.hpp"
#include "authentication.hpp"


    /// class listener
    ///
    /// listen to the defined TCP port for new connexions.
    /// Upon new connexion, it builds a connexion objects
    /// and assign it to a new server object that will
    /// manage incoming request on this connexion accordingly


class listener : public libthreadar::thread
{
public:
    listener(central_report *log,  //< where to send reports
	     authentication *auth, //< where to request for authentications
	     unsigned int port);   //< listen on localhost IPv4 or IPv6
    listener(central_report *log,    //< where to send reports
	     authentication *auth,   //< where to request for authentications
	     const std::string & ip, //< interface to listen on
	     unsigned int port);     //< port to listen on
    listener(const listener & ref) = delete;
    listener(listener && ref) noexcept = delete;
    listener & operator = (const listener & ref) = delete;
    listener & operator = (listener && ref) noexcept = delete;
    ~listener() { close(sockfd); };

protected:
	// inherited from libthreadar::thread;
    virtual void inherited_run() override;

private:
    central_report *rep; //< where to report events
    authentication *src; //< where to validate authentications
    int sockfd;          //< socket descriptor
    int famille;         //< domain familly of the socket
    std::string l_ip;    //< listening IP address
    std::string l_port;  //< listening port

    void set_sockfd(int domain);
    void init(central_report *log,
	      authentication *auth,
	      const std::string & ip,
	      unsigned int port);
};


#endif
