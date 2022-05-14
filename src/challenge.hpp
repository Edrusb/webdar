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

#ifndef CHALLENGE_HPP
#define CHALLENGE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "responder.hpp"
#include "authentication.hpp"


class challenge: public responder
{
public:
	/// constructor
	///
	/// \param[in] base points to the authentication base
	/// \note the base argument must survive this challenge object and is
    challenge(const authentication *base) { if(base == NULL) throw WEBDAR_BUG; database = base; };

	/// returns whether the request is authoritative
	///
	/// \param[in] req the request to analyse
	/// \param[out] user the authenticated user
	/// \return true when the request is authoritative and the set "user" to the name of the authenticated user
    bool is_an_authoritative_request(const request & req, std::string & user);

	/// to be used to answer a non authoritative request
    answer give_answer(const request & req);

private:
    const authentication *database;

};

#endif
