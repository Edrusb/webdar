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

#ifndef SSL_CONTEXT_HPP
#define SSL_CONTEXT_HPP

    // C system header files
extern "C"
{
#include <openssl/ssl.h>
}

    // C++ system header files
#include <string>

    // webdar headers
#include "exceptions.hpp"

    /// manages an SSL context and initiate openssl library

    /// SSL_CTX are openssl structure holding certificate, privatekey
    /// connection method and algorithms... they are used to create
    /// an SSL object that ciphers/deciphers/authenticate SSL exchanges
    /// on a particular connection (for example a TCP connection)

class ssl_context
{
public:

	/// constructor
    ssl_context(const std::string & certificate, const std::string & privatekey);

	/// forbidding copy, allowing move
    ssl_context(const ssl_context & ref) = delete;
    ssl_context(ssl_context && ref) noexcept = default;
    ssl_context & operator = (const ssl_context & ref) = delete;
    ssl_context & operator = (ssl_context && ref) noexcept = default;

	/// destructor
    ~ssl_context() { SSL_CTX_free(ctx); };

    SSL_CTX & get_context() { return *ctx; };

private:

    SSL_CTX *ctx;

    static bool initialized;
    static void go_init_openssl();

};

#endif
