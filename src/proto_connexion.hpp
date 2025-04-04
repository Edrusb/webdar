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

#ifndef PROTO_CONNEXION_HPP
#define PROTO_CONNEXION_HPP

#include "my_config.h"

    // C++ system header files
#include <string>

    // webdar headers
#include "exceptions.hpp"

    /// buffers data from a TCP connexion, this is a pure virtual class

    /// this class provide buffering mechanism to read block of data
    /// and broke then in pieces of bytes or single byte upon request

class proto_connexion
{
public:
    enum status
    {
	connected,    //< both read and write are allowed
	not_connected //< session is closed both directions
    };

	/// constructor
    proto_connexion(const std::string & peerip, unsigned int peerport);

	/// forbidding copy constuctor and assignment operator
    proto_connexion(const proto_connexion & ref) = delete;
    proto_connexion(proto_connexion && ref) noexcept = delete;
    proto_connexion & operator = (const proto_connexion & ref) = delete;
    proto_connexion & operator = (proto_connexion && ref) noexcept = delete;

	/// destructor
    virtual ~proto_connexion();

    status get_status() const { return etat; };
    const std::string & get_ip() const { return ip; };
    unsigned int get_port() const { return port; };

	/// extracts one byte form the buffer / exception thrown if not available
    char read_one(bool blocking);

	/// gives the next char to be read, but do not remove it from the
	/// reading buffer / throw exception if not available
    char read_test_first(bool blocking);

	/// gives the second next char to be read, but do not remove it
	/// from the reading buffer / throw exception if not available
    char read_test_second(bool blocking);

	/// write data

	/// param[in] a data to write
	/// param[in] size amount of byte to write
    void write(const char *a, unsigned int size);

	/// flush pending writings if any
    void flush_write();


protected:

	/// implementation of the low level (without buffering) writing operation
    virtual void write_impl(const char *a, unsigned int size) = 0;

    	/// implementation of the low level (without buffering) reading operation
    virtual unsigned int read_impl(char *a, unsigned int size, bool blocking) = 0;

	/// let inherited class modifying the object status
    void set_status(status st) { etat = st; };

private:
    status etat;       //< proto_connexion status
    std::string ip;    //< IP of the peer host
    unsigned int port; //< port of the peer port

	// buffer management for reading
    unsigned buffer_size;      //< size of the buffer
    char *buffer;              //< temporary area used for parsing, reading data FROM network
    unsigned int already_read; //< amount of data already read
    unsigned int data_size;    //< total of data in buffer, already read or not

	// output buffer
    unsigned out_buf_size;     //< allocated space for the output buffer (out_buf)
    char *out_buf;             //< temporary areas used to gather bytes for writing
    unsigned int last_unwrote; //< amount of byte pending for writing

	/// manages to get (read) data in buffer and set relative variables acordingly
    void fill_buffer(bool blocking);
};

#endif
