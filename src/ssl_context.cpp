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

    //  C system header files
extern "C"
{
}

    // webdar headers

#include "ssl_context.hpp"

using namespace std;

bool ssl_context::initialized = false;

ssl_context::ssl_context(const string & certificate, const string & privatekey)
{
    go_init_openssl();

    const SSL_METHOD *ssl_meth = TLS_server_method();
    if(ssl_meth == nullptr)
	throw exception_openssl();

    ctx = SSL_CTX_new(ssl_meth);
    if(ctx == nullptr)
	throw exception_openssl();

    try
    {
	if(! SSL_CTX_use_certificate_file(ctx, certificate.c_str(), SSL_FILETYPE_PEM))
	    throw exception_openssl();

	if(! SSL_CTX_use_PrivateKey_file(ctx, privatekey.c_str(), SSL_FILETYPE_PEM))
	    throw exception_openssl();

	if(! SSL_CTX_check_private_key(ctx))
	    throw exception_openssl();
    }
    catch(...)
    {
	SSL_CTX_free(ctx);
	throw;
    }
}


void ssl_context::go_init_openssl()
{
    if(!initialized)
    {
	initialized = true;

	SSL_library_init();
	SSL_load_error_strings();
	    // this one is deprecated. Should set the OPENSSL_API_COMPAT macro
	    // https://www.openssl.org/docs/manmaster/man7/openssl_user_macros.html
    }
}

