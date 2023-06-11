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
#include <sys/types.h>
#include <sys/socket.h>
}

    // webdar headers

#include "connexion.hpp"

using namespace std;

connexion::connexion(int fd, const string & peerip, unsigned int peerport):
    proto_connexion(peerip, peerport)
{
    filedesc = fd;
}

connexion::~connexion()
{
    fermeture();
}

unsigned int connexion::read_impl(char *a, unsigned int size, bool blocking)
{
    bool loop = true;
    ssize_t lu = 0;
    int flag = blocking ? 0 : MSG_DONTWAIT;

    if(get_status() != connected)
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

void connexion::write_impl(const char *a, unsigned int size)
{
    unsigned int wrote = 0;
    ssize_t tmp;

    if(get_status() != connected)
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


void connexion::fermeture()
{
    if(get_status() == connected)
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
        set_status(not_connected);
        if(shuted != 0)
            throw exception_system("failed shutting down the socket", errnono);
    }
}
