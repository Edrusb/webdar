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

#ifndef CONNEXION_HPP
#define CONNEXION_HPP

#include "my_config.h"

    // C++ system header files
#include <string>

    // webdar headers
#include "exceptions.hpp"
#include "proto_connexion.hpp"

    /// provides read/write implementation of a socket object

    /// \note used for http connections (non ssl/tls connections)

class connexion : public proto_connexion
{
public:

	/// constructor: create a new object based on a existing socket filedescriptor
    connexion(int fd, const std::string & peerip, unsigned int peerport);

	/// forbidding copy constuctor and assignment operator
    connexion(const connexion & ref) = delete;
    connexion(connexion && ref) noexcept = delete;
    connexion & operator = (const connexion & ref) = delete;
    connexion & operator = (connexion && ref) noexcept = delete;

	/// destructor
    ~connexion();

protected:

	/// inherited from proto_connexion
    virtual void write_impl(const char *a, unsigned int size) override;

    	/// inherited from proto_connexion
    virtual unsigned int read_impl(char *a, unsigned int size, bool blocking) override;


private:

    int filedesc; ///< file descriptor to operate on

	/// close the connexion
    void fermeture();

};

#endif
