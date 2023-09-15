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

#ifndef HTML_ERROR_HPP
#define HTML_ERROR_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_page.hpp"
#include "events.hpp"
#include "html_text.hpp"
#include "html_button.hpp"


class html_error : public html_page, public events, public actor
{
public:
    static const char* acknowledged;

    html_error();
    html_error(const html_error & ref) = default;
    html_error(html_error && ref) noexcept = default;
    html_error & operator = (const html_error & ref) = default;
    html_error & operator = (html_error && ref) noexcept = default;
    ~html_error() = default;

    void set_message(const std::string & msg);

	/// inherited from class actor
    virtual void on_event(const std::string & event_name) override;

	/// defines the name of the session
    void set_session_name(const std::string & sessname);

protected:
	// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    html_div global;
    html_text the_error;
    html_button close;

	// css classes
    static const char* class_message;
    static const char* class_global;
    static const char* class_button;

	// events
    static const char* close_event;
};

#endif
