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

    // C system header files
extern "C"
{
#include <openssl/err.h>
}

    // C++ system header files


    // webdar headers



    //
#include "exceptions.hpp"

using namespace std;

exception_system::exception_system(const string & context, int error_code) : exception_base("")
{
#if (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE
    const unsigned int SIZE = 300;
    char buffer[SIZE];
    int ret = strerror_r(error_code, buffer, SIZE);
    buffer[SIZE - 1] = '\0'; // that does not hurt
    if(ret == 0) // (successfull call)
	change_message(context + ": " + buffer);
    else
#endif
	change_message(string(strerror(error_code)));
}

exception_libcall::exception_libcall(const libdar::Egeneric & e): exception_base(e.get_message())
{
    if(dynamic_cast<const libdar::Ebug *>(&e) != nullptr)
	change_message(e.dump_str());
}

exception_openssl::exception_openssl(): exception_base(get_ssl_error()) {};

string exception_openssl::get_ssl_error()
{
    string ret;
    unsigned long code;
    char err_buf[ERR_BUF_LEN];

    while((code = ERR_get_error()) > 0)
    {
	if(!ret.empty())
	    ret += "\n";
	ERR_error_string_n(code, err_buf, ERR_BUF_LEN);
	err_buf[ERR_BUF_LEN - 1] = '\0'; // just in case
	ret += string(err_buf);
    }

    if(ret.empty())
	ret = "cannot get error reason from openssl";

    return ret;
}


void throw_as_most_derivated_class(exception_base *ebase)
{
    exception_memory *emem = dynamic_cast<exception_memory *>(ebase);
    exception_bug *ebug = dynamic_cast<exception_bug *>(ebase);
    exception_system *esys = dynamic_cast<exception_system *>(ebase);
    exception_range *erange = dynamic_cast<exception_range *>(ebase);
    exception_input *input = dynamic_cast<exception_input *>(ebase);
    exception_feature *feature = dynamic_cast<exception_feature *>(ebase);
    exception_libcall *libcall = dynamic_cast<exception_libcall *>(ebase);

    if(emem != nullptr)
	throw *emem;
    if(ebug != nullptr)
	throw *ebug;
    if(esys != nullptr)
	throw *esys;
    if(erange != nullptr)
	throw *erange;
    if(input != nullptr)
	throw *input;
    if(feature != nullptr)
	throw *feature;
    if(libcall != nullptr)
	throw *libcall;

    throw WEBDAR_BUG; // unknown exception
}
