/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013 Denis Corbin
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

#ifndef HTML_CRYPTO_ALGO_HPP
#define HTML_CRYPTO_ALGO_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <vector>
#include <string>
#include <dar/libdar.hpp>

    // webdar headers
#include "html_form_select.hpp"
#include "body_builder.hpp"

class html_crypto_algo : public html_form_select
{
public:
    static const std::string changed;

    html_crypto_algo(const std::string & title);

    libdar::crypto_algo get_value() const;
    void set_value(libdar::crypto_algo val);

	/// actor inheritance
    virtual void on_event(const std::string & event_name);
};

#endif
