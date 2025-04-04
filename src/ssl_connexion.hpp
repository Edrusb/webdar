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

#ifndef SSL_CONNEXION_HPP
#define SSL_CONNEXION_HPP

#include "my_config.h"

    // C system header files

extern "C"
{
#if HAVE_OPENSSL_SSL_H
#include <openssl/ssl.h>
#endif
}

    // C++ system header files
#include <string>

    // webdar headers
#include "exceptions.hpp"
#include "connexion.hpp"

    /// provides read/write implementation of a socket object

    /// \note used for http connections (non ssl/tls connections)

class ssl_connexion : public connexion
{
public:

	/// constructor: create a new object based on a existing socket filedescriptor
    ssl_connexion(int fd, SSL_CTX & ctx, const std::string & peerip, unsigned int peerport);

	/// copy is forbidden, move is allowed
    ssl_connexion(const ssl_connexion & ref) = delete;
    ssl_connexion(ssl_connexion && ref) noexcept = default;
    ssl_connexion & operator = (const ssl_connexion & ref) = delete;
    ssl_connexion & operator = (ssl_connexion && ref) noexcept = default;

	/// destructor
    ~ssl_connexion();

protected:

	/// inherited from proto_connexion
    virtual void write_impl(const char *a, unsigned int size) override;

    	/// inherited from proto_connexion
    virtual unsigned int read_impl(char *a, unsigned int size, bool blocking) override;

private:

    SSL *ssl;     ///< holds ssl status

};

#endif
