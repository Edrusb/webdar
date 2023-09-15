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

#include "pipe_connexion.hpp"

using namespace std;

pipe_connexion::pipe_connexion(int read_fd, int write_fd, const std::string & peerip, unsigned int peerport):
    proto_connexion(peerip, peerport)
{
    readfd = read_fd;
    writefd = write_fd;
}

pipe_connexion::~pipe_connexion() { fermeture(); };

unsigned int pipe_connexion::read_impl(char *a, unsigned int size, bool blocking)
{
    bool loop = true;
    ssize_t lu = 0;

    if(get_status() != connected)
	throw WEBDAR_BUG;

    int flag = fcntl(readfd, F_GETFL);
    if(blocking)
	fcntl(readfd, F_SETFL, flag & ~O_NONBLOCK);
    else
	fcntl(readfd, F_SETFL, flag | O_NONBLOCK);

    try
    {
	while(loop)
	{
	    loop = false;
	    lu = read(readfd, a, size);
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
    }
    catch(...)
    {
	fcntl(readfd, F_SETFL, flag);
	throw;
    }
    fcntl(readfd, F_SETFL, flag);

    return (unsigned int)lu;
}

void pipe_connexion::write_impl(const char *a, unsigned int size)
{
    unsigned int wrote = 0;
    ssize_t tmp;

    if(get_status() != connected)
        throw WEBDAR_BUG;

    while(wrote < size)
    {
        tmp = ::write(writefd, (void *)(a + wrote), size - wrote);
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


void pipe_connexion::fermeture()
{
    if(get_status() == connected)
    {
//        int errnono;

        close(readfd);
	close(writefd);
        set_status(not_connected);
    }
}
