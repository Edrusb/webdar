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

#ifndef PIPE_CONNEXION_HPP
#define PIPE_CONNEXION_HPP

    // C++ system header files
#include <string>

    // webdar headers
#include "exceptions.hpp"
#include "proto_connexion.hpp"

    /// provides read/write implementation of a socket object

    /// \note used for http connections (non ssl/tls connections)

class pipe_connexion : public proto_connexion
{
public:

	/// constructor: create a new object based on a existing socket filedescriptor
    pipe_connexion(int read_fd, int write_fd, const std::string & peerip, unsigned int peerport);

	/// forbidding copy constuctor and assignment operator
    pipe_connexion(const pipe_connexion & ref) = delete;
    pipe_connexion(pipe_connexion && ref) noexcept = delete;
    pipe_connexion & operator = (const pipe_connexion & ref) = delete;
    pipe_connexion & operator = (pipe_connexion && ref) noexcept = delete;

	/// destructor
    ~pipe_connexion();

protected:

	/// inherited from proto_pipe_connexion
    virtual void write_impl(const char *a, unsigned int size) override;

    	/// inherited from proto_pipe_connexion
    virtual unsigned int read_impl(char *a, unsigned int size, bool blocking) override;


private:
    int readfd;
    int writefd;

	/// close the file descriptors
    void fermeture();

};

#endif
