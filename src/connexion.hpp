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

#ifndef CONNEXION_HPP
#define CONNEXION_HPP

    // C++ system header files
#include <string>

    // webdar headers
#include "exceptions.hpp"

    /// class connexion - holds TCP sessions from clients

    /// this class provide read/write access to a TCP connexion
    /// it may support future evolution by mean of inherited class
    /// that will provide for example ssl ciphering for example
class connexion
{
public:
    enum status
    {
	connected,    //< both read and write are allowed
	not_connected //< session is closed both directions
    };

	/// constructor: create a new object based on a existing socket filedescriptor
    connexion(int fd, const std::string & peerip, unsigned int peerport);

	/// forbidding copy constuctor and assignment operator
    connexion(const connexion & ref) { throw WEBDAR_BUG; };
    const connexion & operator = (const connexion & ref) { throw WEBDAR_BUG; };

	/// destructor is virtual to permit further evolution by mean of inherited classes
    virtual ~connexion();

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

	/// write data to the socket

	/// param[in] a data to write
	/// param[in] size amount of byte to write
    virtual void write(const char *a, unsigned int size);

	/// flush pending writings
    virtual void flush_write();

private:
    status etat;       //< connexion status
    int filedesc;      //< socket file descriptor
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


	/// close the connexion
    void fermeture();

	/// read data from the socket

	/// \param[in] a where to store read data
	/// \param[in] size width of the buffer pointed to by a
	/// \param[in] blocking whether reading is blocking or not
    unsigned int atomic_read(char *a, unsigned int size, bool blocking);


	/// manages to get (read) data in buffer and set relative variables acordingly
    void fill_buffer(bool blocking);

    bool atomic_write(const char *a, unsigned int size);

};

#endif
