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
#include "html_div.hpp"
#include "actor.hpp"
#include "events.hpp"
#include "html_text.hpp"
#include "html_button.hpp"

    /// html component used for the user to define a has algorithm

class html_disconnect : public html_div, public actor, public events
{
public:
	/// disconnect event
    static const std::string event_disconn;

    html_disconnect(const std::string & webdar_title);
    html_disconnect(const html_disconnect & ref) = delete;
    html_disconnect(html_disconnect && ref) noexcept = delete;
    html_disconnect & operator = (const html_disconnect & ref) = delete;
    html_disconnect & operator = (html_disconnect && ref) noexcept = delete;
    ~html_disconnect() = default;

	/// set the username to display for the session ownership
    void set_username(const std::string & username);

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:

	/// inherited from body_builder from html_button
    virtual void new_css_library_available() override;


private:
    static const std::string css_global;
    static const std::string css_title;
    static const std::string css_title_box;
    static const std::string css_status;
    static const std::string css_status_box;
    static const std::string css_quit_box;
    static const std::string css_quit_link;

    html_div title_box;
    html_text title;
    html_div status_box;
    html_text status;
    html_button quit;
};

#endif
