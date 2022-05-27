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
}

    // webdar headers

#include "connexion.hpp"

using namespace std;

#define BUFFER_SIZE 10240
#ifdef SSIZE_MAX
#if SSIZE_MAX < BUFFER_SIZE
#undef BUFFER_SIZE
#define BUFFER_SIZE SSIZE_MAX
#endif
#endif

connexion::connexion(int fd, const string & peerip, unsigned int peerport)
{
    etat = connected;
    filedesc = fd;
    ip = peerip;
    port = peerport;
    buffer_size = BUFFER_SIZE;
    buffer = nullptr;
    already_read = 0;
    data_size = 0;
    out_buf_size = BUFFER_SIZE;
    out_buf = nullptr;
    last_unwrote = 0;

    try
    {
	buffer = new (nothrow) char[buffer_size];
	if(buffer == nullptr)
	    throw exception_memory();

	out_buf = new (nothrow) char[out_buf_size];
	if(out_buf == nullptr)
	    throw exception_memory();
    }
    catch(...)
    {
	if(buffer != nullptr)
	    delete [] buffer;
	if(out_buf != nullptr)
	    delete [] out_buf;
	throw;
    }
}

connexion::~connexion()
{
    fermeture();
    if(buffer != nullptr)
	delete [] buffer;
    if(out_buf != nullptr)
	delete [] out_buf;
}

char connexion::read_one(bool blocking)
{
    if(already_read == data_size)
	fill_buffer(blocking);
    if(already_read == data_size)
	throw exception_range("no more data available from connection");

    return buffer[already_read++];
}

char connexion::read_test_first(bool blocking)
{
    if(already_read == data_size)
	fill_buffer(blocking);
    if(already_read == data_size)
	throw exception_range("no more data available from connection");

    return buffer[already_read];
}

char connexion::read_test_second(bool blocking)
{
    if(data_size - already_read < 2)
	fill_buffer(blocking);
    if(data_size - already_read < 2)
	throw exception_range("no more data available from connection");

    return buffer[already_read + 1];
}


void connexion::write(const char *a, unsigned int size)
{
    if(size + last_unwrote < out_buf_size)
    {
	(void)memcpy(out_buf + last_unwrote, a, size);
	last_unwrote += size;
    }
    else
    {
	flush_write();
	atomic_write(a, size);
    }
}

void connexion::flush_write()
{
    if(last_unwrote > 0)
    {
	atomic_write(out_buf, last_unwrote);
	last_unwrote = 0;
    }
}

void connexion::fermeture()
{
    flush_write();

    if(etat == connected)
    {
	int shuted;
	int errnono;
	if(filedesc < 0)
	    throw WEBDAR_BUG;
	shuted = shutdown(filedesc, SHUT_RDWR);
	if(shuted != 0)
	    errnono = errno;
	close(filedesc);
	filedesc = -1;
	etat = not_connected;
	if(shuted != 0)
	    throw exception_system("failed shutting down the socket", errnono);
    }
}

unsigned int connexion::atomic_read(char *a, unsigned int size, bool blocking)
{
    bool loop = true;
    ssize_t lu = 0;
    int flag = blocking ? 0 : MSG_DONTWAIT;

    if(etat != connected)
	throw WEBDAR_BUG;

    while(loop)
    {
	loop = false;
	lu = recv(filedesc, a, size, flag);
	if(lu == 0)
	{
	    fermeture();
	    throw exception_range("reached end of data on socket");
	}
	if(lu < 0)
	{
	    switch(errno)
	    {
	    case EINTR:    // system call interrupted by a signal
		loop = true;
		break;
	    case EAGAIN:   // means no data avaiable in non-blocking mode
		if(blocking)
		    throw WEBDAR_BUG;
		else
		    lu = 0;
		break;
	    default:
		throw exception_system("Error met while receiving data: ", errno);
	    }
	}
    }

    return (unsigned int)lu;
}

void connexion::fill_buffer(bool blocking)
{
    if(data_size < buffer_size
       || already_read == data_size) // there is some room to receive more data in the buffer
    {
	if(get_status() == connexion::connected)
	{
	    if(already_read == data_size)   // no more data to read, so we restart at the beginning
		already_read = data_size = 0;
	    else
	    {
		if(already_read > data_size)
		    throw WEBDAR_BUG;
		if(data_size > buffer_size)
		    throw WEBDAR_BUG;
		if(already_read > 0)
		{
		    (void)memmove(buffer, buffer + already_read, data_size - already_read);
		    data_size -= already_read;
		    already_read = 0;
		}
	    }

	    try
	    {
		data_size += atomic_read(buffer + data_size, buffer_size - data_size, blocking);
	    }
	    catch(exception_bug & e)
	    {
		throw;
	    }
	    catch(exception_base & e)
	    {
		if(already_read == data_size) // no more data in buffer
		    throw;
	    }
	}
	else
	{
	    if(already_read == data_size)
		throw exception_range("Connexion closed and no more data available for reading");
	}
    }
}

void connexion::atomic_write(const char *a, unsigned int size)
{
    unsigned int wrote = 0;
    ssize_t tmp;

    if(etat != connected)
	throw WEBDAR_BUG;

    while(wrote < size)
    {
	tmp = send(filedesc, (void *)(a + wrote), size - wrote, MSG_NOSIGNAL);
	if(tmp < 0)
	{
	    switch(errno)
	    {
	    case EPIPE:
		    // the connection is broken / closed by the other end
		fermeture();
		throw exception_system("Error met while sending data: ", errno);
		break;
	    case EINTR:
		break;
	    default:
		throw exception_system("Error met while sending data: ", errno);
	    }
	}
	else
	    wrote += tmp;
    }
}

