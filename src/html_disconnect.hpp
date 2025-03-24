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
#include "html_image.hpp"

    /// html component used for the user to define a has algorithm

class html_disconnect : public html_div, public actor, public events
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

	/// set the username to display for the session ownership
    void set_username(const std::string & username);

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// whether to emulate user logout while using basic authentication (see also class server)

	/// \note in version 1.0 web authentication relies on basic authentication schema, which does not
	/// have any mean to provide disconnection. To emulate such disconnection, each new TCP session requires
	/// to reauthentification from the user. A web browser may decide to tear down and recreate a new TCP session
	/// after some time, which will requirer the user to reauthenticate while from user stand point this is
	/// the same web/http session. To avoid this, it is possible to only rely on the basic authnentication
	/// and upon new TCP session, the server object only validate that the HTTP header has a valid credentials.
	/// The drawback is that deauthentication is not possible unless the browser is restarted. The following call
	/// define the either the first (true) ot the second (false) previously described behaviors for all server
	/// objects. This is a static method because it is expected to be removed in the future when the session object
	/// will keep trace of valid authentication tokens (stored as cookies or basic authentication with volatile temporary
	/// password also stored and eventually removed from an internal base the challenge object would have access to.
    static void force_disconnection_at_end_of_session(bool val) { default_basic_auth = ! val; };

protected:

	/// inherited from body_builder from html_button
    virtual void new_css_library_available() override;


private:
    static const std::string css_global;
    static const std::string css_logo;
    static const std::string css_title;
    static const std::string css_title_vers;
    static const std::string css_title_box;
    static const std::string css_status;
    static const std::string css_status_box;
    static const std::string css_quit_box;
    static const std::string css_quit_link;

    html_image logo;
    html_div title_box;
    html_text title;
    html_text title_vers;
    html_div status_box;
    html_text status;
    html_button quit;

    static bool default_basic_auth;     ///< if true, no disconnection is provided (unless browser is restarted)

};

#endif
