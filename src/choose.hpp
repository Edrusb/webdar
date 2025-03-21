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

#ifndef CHOOSE_HPP
#define CHOOSE_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <string>
#include <vector>
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "session.hpp"
#include "responder.hpp"
#include "actor.hpp"
#include "events.hpp"
#include "html_page.hpp"
#include "html_url.hpp"
#include "html_form.hpp"
#include "html_div.hpp"
#include "html_table.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_input.hpp"
#include "html_form_radio.hpp"
#include "html_yes_no_box.hpp"
#include "html_disconnect.hpp"

    /// display current existing user sessions and let user kill or change of session

    ///  \note chooser object is a per user and list all sessions owned by that user
    ///  but the creation if chooser object is managed by the class chooser, which exposes
    ///  the static method give_answer_for() for that purpose

class choose : public responder, public actor
{
public:
	// constructor
    choose();
    choose(const choose & ref) = delete;
    choose(choose && ref) noexcept = delete;
    choose & operator = (const choose & ref) = delete;
    choose & operator = (choose && ref) noexcept = delete;
    ~choose() { release_boxes(); };

	/// mandatory call before using other methods (give_answer() in particular)
    void set_owner(const std::string & user);

	/// inherited from responder

	/// \note alsways shows the session table for the user
    virtual answer give_answer(const request & req) override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// whether user has requested to disconnect
    bool disconnection_requested() const { bool ret = disconnect_req; disconnect_req = false; return ret; };

private:
    std::string owner;           ///< list only sessions for that owner
    mutable bool disconnect_req; ///< whether user has asked for disconnection
    std::vector<html_form_input *> boxes; //< list of checkboxes in "page"
    std::vector<session::session_summary> sess; //< list of sessions in page

	/// listing session page and associated objects

    html_page page;          ///< page root for session listing
    html_disconnect disco;
    html_table table;
    html_div box_nouvelle;
    html_url nouvelle;
    html_form form;
    html_div div;
    html_yes_no_box confirmed;

    void regenerate_table_page();          ///< regenerate sess and boxes components (shown as session table) from currently existing sessions
    void release_boxes();                  ///< destroy checkboxes objects
    void kill_selected_sessions() const;   ///< kill checked session but does not change html components

    static const std::string css_class_normal_text;
    static const std::string url_new_session;
};

#endif
