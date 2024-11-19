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

#ifndef HTML_DISCONNECT_HPP
#define HTML_DISCONNECT_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "html_button.hpp"

    /// html component used for the user to define a has algorithm

class html_disconnect : public html_button
{
public:
	/// disconnect event
    static const std::string event_disconn;

    html_disconnect();
    html_disconnect(const html_disconnect & ref) = delete;
    html_disconnect(html_disconnect && ref) noexcept = delete;
    html_disconnect & operator = (const html_disconnect & ref) = delete;
    html_disconnect & operator = (html_disconnect && ref) noexcept = delete;
    ~html_disconnect() = default;

protected:

	/// inherited from body_builder from html_button
    virtual void new_css_library_available() override;


private:
    static const std::string css_disconn;
};

#endif
