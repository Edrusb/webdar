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

#ifndef PARSER_HPP
#define PARSER_HPP

    // C++ system header files
#include <memory>

    // webdar headers
#include "exceptions.hpp"
#include "connexion.hpp"
#include "central_report.hpp"
#include "request.hpp"
#include "answer.hpp"

class parser
{
public:
	/// constructor
	///
	/// \param[in] input is the connexion to read data from
	/// \param[in] log where to log messages
	/// \note the connexion object passed to this becomes under the responsibility
	/// of this parser object and will be deleted when necessary. (where from the unique_ptr since C++11 webdar code update)
	/// The log object is out of the responsibility of this object
	/// however it must survive this parser object (where from the shared_ptr since C++11 webdar code update)
    parser(std::unique_ptr<connexion> & input,
	   const std::shared_ptr<central_report> & log);

    parser(const parser & ref) = delete;
    parser(parser && ref) noexcept = delete;
    parser & operator = (const parser & ref) = delete;
    parser & operator = (parser && ref) noexcept = delete;

	/// destructor
    ~parser() { close(); };

	/// provides visibility on the connection status
    connexion::status get_status() const { if(!source) return connexion::not_connected; return source->get_status(); };

	/// get the next request URI
	///
	/// \param[out] uri the value of the next request uri
	/// \return false if not enough data is available to provide the uri
    bool get_next_request_uri(uri & val);

	/// provides the next request
    const request & get_request();

	/// modify the answer to conform to RFC 1945 before sending it
    void send_answer(answer & ans);

	/// closes the current connection
    void close();

private:
    bool answered;             //< whether last request was answered or not
    std::unique_ptr<connexion> source; //< the connexion to the client
    request req;               //< value of the last request

    void valid_source() const { if(!source || source->get_status() != connexion::connected) throw exception_range("socket disconnected"); };
    void checks_main(const request & req, answer & ans);
    void checks_webdar(const request & req, answer & ans);
    void checks_rfc1945(const request & req, answer & ans);
};


#endif
