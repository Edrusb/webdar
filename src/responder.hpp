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

#ifndef RESPONDER_HPP
#define RESPONDER_HPP

    // C++ system header files
#include <string>
#include <vector>
#include <map>

    // webdar headers
#include "uri.hpp"
#include "webdar_tools.hpp"
#include "exceptions.hpp"
#include "request.hpp"
#include "answer.hpp"
#include "chemin.hpp"

    /////////////////////////////////////////////////////////////////////////////////////
    /// pure virtual class, base class for all classes that can generate an answer to a request

class responder
{
public:
    responder() = default;
    responder(const responder & ref) = default;
    responder(responder && ref) noexcept = default;
    responder & operator = (const responder & ref) = default;
    responder & operator = (responder && ref) noexcept = default;
    virtual ~responder() {};

	/// used for body_builder object for they have a proper prefix in path
    void set_prefix(const chemin & chem) { prefix = chem; prefix_has_changed(); };
	/// get the recorded URI path prefix for body_builder root hierarchy
    const chemin & get_prefix() const { return prefix; };

	/// provides a standard mean for inherited class to provide answer to requests
    virtual answer give_answer(const request & req) = 0;

protected:
	/// hook for inherited class, when the body builder object had its prefix changed
    virtual void prefix_has_changed() {};

private:
    chemin prefix;

};


#endif
