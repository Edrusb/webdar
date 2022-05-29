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

    //  C system header files
extern "C"
{
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
}

    // webdar headers

#include "ssl_connexion.hpp"

using namespace std;

ssl_connexion::ssl_connexion(int fd, SSL_CTX & ctx, const string & peerip, unsigned int peerport):
    connexion(fd, peerip, peerport)
{
    ssl = SSL_new(&ctx);
    if(ssl == nullptr)
	throw exception_openssl();
    if(! SSL_set_fd(ssl, fd))
	throw exception_openssl();
    (void)SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY); // is set by default, but this does not hurt forcing this mode here
    if(!SSL_accept(ssl))
	throw exception_openssl();
}

ssl_connexion::~ssl_connexion()
{
    SSL_free(ssl);
}

unsigned int ssl_connexion::read_impl(char *a, unsigned int size, bool blocking)
{
    size_t lu = 0;
    int fd = SSL_get_fd(ssl);

    if(fd < 0)
	throw exception_openssl();

    int flag = fcntl(fd, F_GETFL);
    if(blocking)
	fcntl(fd, F_SETFL, flag & ~O_NONBLOCK);
    else
	fcntl(fd, F_SETFL, flag | O_NONBLOCK);

    try
    {
	if(! SSL_read_ex(ssl, (void *)a, size, &lu))
	    throw exception_openssl();
    }
    catch(...)
    {
	fcntl(fd, F_SETFL, flag);
	throw;
    }
    fcntl(fd, F_SETFL, flag);

    return (unsigned int)lu;
}

void ssl_connexion::write_impl(const char *a, unsigned int size)
{
    size_t wrote = 0;
    size_t wrote_total = 0;

    while(wrote_total < size)
    {
	if(! SSL_write_ex(ssl, (void *)(a + wrote_total), size - wrote_total, &wrote))
	    throw exception_openssl();
	else
	    wrote_total += wrote;
    }
}
