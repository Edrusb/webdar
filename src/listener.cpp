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

    // C system header files
#include "my_config.h"
extern "C"
{
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#if HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#if HAVE_NETINET_IP_H
#include <netinet/ip.h>
#endif

#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#if HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#if HAVE_STRINGS_H
#include <strings.h>
#endif

#if HAVE_STRING_H
#include <string.h>
#endif

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if HAVE_ERRNO_H
#include <errno.h>
#endif
}

    // C++ system header files
#include <new>

    // libraries header files
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "listener.hpp"
#include "connexion.hpp"
#include "server.hpp"
#include "webdar_tools.hpp"
#include "ssl_connexion.hpp"
#include "global_parameters.hpp"

using namespace std;

static struct in_addr string_to_network_IPv4(const string & ip);
static struct in6_addr string_to_network_IPv6(const string & ip);
static string network_IPv4_to_string(const struct in_addr & ip);
static string network_IPv6_to_string(const struct in6_addr & ip);

listener::listener(const shared_ptr<central_report> & log,
		   const shared_ptr<const authentication> & auth,
		   unique_ptr<ssl_context> & ciphering,
		   shared_ptr<server_pool> & pool,
		   unsigned int port)
{
#ifdef LIBTHREADAR_STACK_FEATURE
    set_stack_size(DEFAULT_STACK_SIZE);
#endif

    try
    {
	init(log, auth, ciphering, pool, "::1", port);
    }
    catch(exception_bug & e)
    {
	throw;
    }
    catch(...)
    {
	init(log, auth, ciphering, pool, "127.0.0.1", port);
	    // no throw;
    }
}

listener::listener(const shared_ptr<central_report> & log,
		   const shared_ptr<const authentication> & auth,
		   unique_ptr<ssl_context> & ciphering,
		   shared_ptr<server_pool> & pool,
		   const string & ip,
		   unsigned int port)
{
    init(log, auth, ciphering, pool, ip, port);
}

void listener::init(const shared_ptr<central_report> & log,
		    const shared_ptr<const authentication> & auth,
		    unique_ptr<ssl_context> & ciphering,
		    shared_ptr<server_pool> & pool,
		    const string & ip,
		    unsigned int port)
{
    sigset_t sigs;

    if(!log)
	throw WEBDAR_BUG;
    if(!auth)
	throw WEBDAR_BUG;
    if(!pool)
	throw WEBDAR_BUG;

    l_ip = ip;
    l_port = webdar_tools_convert_to_string(port);
    if(sigfillset(&sigs) != 0)
	throw exception_system("failed creating a full signal set", errno);
    if(sigdelset(&sigs, THREAD_SIGNAL) != 0)
	throw exception_system("failed removing the THREAD_SIGNAL from signal set", errno);
    set_signal_mask(sigs);

    rep = log;
    src = auth;
    sockfd = -1;
    ssl_ctx = std::move(ciphering);
    srv = pool;

    try
    {
	struct sockaddr *ptr;
	socklen_t ptr_len = 0;
	struct sockaddr_in sin;
	struct sockaddr_in6 sin6;

	try
	{
	    (void)memset(&sin6, 0, sizeof(sin6));
	    sin6.sin6_family = AF_INET6;
	    sin6.sin6_port = htons(port);
		// sin6.sin6_flowinfo is not modified (set to zero here)
	    sin6.sin6_addr = string_to_network_IPv6(ip.c_str());
		// sin6.sin6_scope_id is not modified (set to zero here)
	    ptr = (struct sockaddr *)(&sin6);
	    ptr_len = sizeof(sin6);

	    set_sockfd(AF_INET6);
	    rep->report(debug, "listener object: IPv6 socket datastructure setup done");
	}
	catch(exception_bug & e)
	{
	    throw;
	}
	catch(...)
	{
	    rep->report(debug, "listener object: IPv6 socket datastructure failed, considering given IP as IPv4 address");
	    (void)memset(&sin, 0, sizeof(sin));
	    sin.sin_family = AF_INET;
	    sin.sin_port = htons(port);
	    sin.sin_addr = string_to_network_IPv4(ip.c_str());
	    ptr = (struct sockaddr *)(&sin);
	    ptr_len = sizeof(sin);

	    set_sockfd(AF_INET);
		// no throw
	    rep->report(debug, "listener object: IPv4 socket datastructure setup done");
	}

	if(::bind(sockfd, ptr, ptr_len) < 0)
	    throw exception_system("Error while binding socket to network", errno);
	rep->report(debug, "listener object: Socket binding done");

	    // we use SOMAXCONN here because its meaning depends on OS implementations
	    // So we do not use to control the max number of concurrent session
	if(listen(sockfd, SOMAXCONN) < 0)
	    throw exception_system("Error while trying to set the socket to listen", errno);
	rep->report(debug, "listener object: Socket set as a listenning socket");
    }
    catch(...)
    {
	if(sockfd >= 0)
	    close(sockfd);
	sockfd = -1;
	throw;
    }
}

void listener::set_sockfd(int domain)
{
    sockfd = socket(domain, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd < 0)
	throw exception_system("Error creating socket", errno);
    int val = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val)) != 0)
	throw exception_system("Error activating TCP keepalive on socket", errno);
    famille = domain;
}

void listener::inherited_run()
{
    int ret = 0;
    struct sockaddr *addr = nullptr;
    socklen_t addrlen = 0;
    socklen_t addrlen_ref = 0;
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;
    string ip;
    unsigned int port;
    unique_ptr<proto_connexion> con;

    rep->report(debug, "listener object: started in its own thread");

    switch(famille)
    {
    case AF_INET6:
	addr = (struct sockaddr *)(&sin6);
	addrlen = sizeof(sin6);
	break;
    case AF_INET:
	addr = (struct sockaddr *)(&sin);
	addrlen = sizeof(sin);
	break;
    default:
	throw WEBDAR_BUG;
    }

    addrlen_ref = addrlen;

    while(true)
    {
	cancellation_checkpoint();
	rep->report(info, "listener object: waiting for incoming connections on " + l_ip + " port " + l_port);
	(void)memset(addr, 0, addrlen);
	ret = accept(sockfd, addr, &addrlen);
	rep->report(debug, "listener object: exiting from accept(), a new connection has come on " + l_ip + " port " + l_port + "?");

	if(ret < 0)
	{
	    switch(errno)
	    {
	    case ECONNABORTED:
	    case EFAULT:
	    case EINTR:
	    case EPROTO:
	    case EPERM:
	    case ENOSR:
	    case ETIMEDOUT:
		rep->report(err, "listener object: exiting from accept(), a new connection has come: NO");
		break;
	    case EMFILE:
	    case ENOBUFS:
	    case ENOMEM:
		rep->report(err, "listener object: exiting from accept(), a new connection has come: NO, sleeping 1s");
		sleep(1);
		break;
	    case EAGAIN:
		throw WEBDAR_BUG;
	    case EBADF:
		throw WEBDAR_BUG;
	    case EINVAL:
		throw WEBDAR_BUG;
	    case ENOTSOCK:
		throw WEBDAR_BUG;
	    case EOPNOTSUPP:
		throw WEBDAR_BUG;
	    default:
		throw WEBDAR_BUG;
	    }
	    continue;
	}

	rep->report(debug, "listener object: exiting from accept(), a new connection has come: YES");
	if(addrlen > addrlen_ref) // addr is truncated
	    throw WEBDAR_BUG;

	switch(famille)
	{
	case AF_INET6:
	    ip = network_IPv6_to_string(sin6.sin6_addr);
	    port = ntohs(sin6.sin6_port);
	    rep->report(warning, string("listener object: New IPv6 connection has come from ") + ip + " port " + webdar_tools_convert_to_string(port));
	    break;
	case AF_INET:
	    ip = network_IPv4_to_string(sin.sin_addr);
	    port = ntohs(sin.sin_port);
	    rep->report(warning, string("listener object: New IPv4 connection has come from ") + ip + " port " + webdar_tools_convert_to_string(port));
	    break;
	default:
	    throw WEBDAR_BUG;
	}

	if(ssl_ctx)
	{
	    rep->report(debug, "listener object: creating a new \"ssl_connexion\" object");
	    con.reset(new (nothrow) ssl_connexion(ret, ssl_ctx->get_context(), ip, port));
	}
	else
	{
	    rep->report(debug, "listener object: creating a new \"connexion\" object");
	    con.reset(new (nothrow) connexion(ret, ip, port));
	}

	if(!con)
	    throw exception_memory();
	else
	{
	    rep->report(debug, "listener object: creating a server thread to answer requests received from the new connection");
	    if(!srv->run_new_server(src, con))
	    {
		rep->report(warning, "failed to create a new server maximum connection reached");
		con.reset();
	    }
	    else
	    {
		if(con)
		    throw WEBDAR_BUG;
		    // con should have passed
		    // the connexion pointed to
		    // object to the server object/class
	    }
	}
    }
}


static struct in_addr string_to_network_IPv4(const string & ip)
{
    struct in_addr ret;

    switch(inet_pton(AF_INET, ip.c_str(), &ret.s_addr))
    {
    case 0:
	throw exception_range("Badly formated IPv4 address");
    case -1:
	throw exception_system("Error while parsing IPv4 address", errno);
    case 1:
	break; // success
    default:
	throw WEBDAR_BUG;
    }

    return ret;
}

static struct in6_addr string_to_network_IPv6(const string & ip)
{
    struct in6_addr ret;

    switch(inet_pton(AF_INET6, ip.c_str(), &ret.s6_addr))
    {
    case 0:
	throw exception_range("Badly formated IPv6 address");
    case -1:
	throw exception_system("Error while parsing IPv6 address", errno);
    case 1:
	break; // success
    default:
	throw WEBDAR_BUG;
    }

    return ret;
}

static string network_IPv4_to_string(const struct in_addr & ip)
{
    string ret;
    const int BUFSZ = 20;
    char buffer[BUFSZ];
    const char *ptr = inet_ntop(AF_INET, &ip, buffer, BUFSZ);

    if(ptr == nullptr)
	throw exception_system("error converting IPv4 address to its textual representation", errno);
    else
	ret = *ptr;

    return ret;
}

static string network_IPv6_to_string(const struct in6_addr & ip)
{
    string ret;
    const int BUFSZ = 50;
    char buffer[BUFSZ];
    const char * ptr = inet_ntop(AF_INET6, &ip, buffer, BUFSZ);

    if(ptr == nullptr)
	throw exception_system("error converting IPv4 address to its textual representation", errno);
    else
	ret = *ptr;

    return ret;
}


