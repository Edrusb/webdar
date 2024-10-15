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
#include "html_page.hpp"
#include "html_page.hpp"
#include "html_url.hpp"
#include "html_form.hpp"
#include "html_div.hpp"
#include "html_table.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_input.hpp"
#include "html_form_radio.hpp"
#include "html_yes_no_box.hpp"

    /// display current existing user sessions and let user kill or change of session

    ///  \note chooser object is a per user and list all sessions owned by that user
    ///  but the creation if chooser object is managed by the class chooser, which exposes
    ///  the static method give_answer_for() for that purpose

class choose : public responder
{
public:

    static answer give_answer_for(const std::string & user, const request & req);

    static void cleanup_memory();

private:

	// constructor
    choose(const std::string & user);
    choose(const choose & ref) = delete;
    choose(choose && ref) noexcept = delete;
    choose & operator = (const choose & ref) = delete;
    choose & operator = (choose && ref) noexcept = delete;
    ~choose() { release_boxes(); };

	// inherited from responder, but made private
    virtual answer give_answer(const request & req) override;


    std::string owner;       ///< list only sessions for that owner
    bool confirm_mode;       ///< whether we display session list or kill confirmation
	/// listing session page and associated objects
    html_page page;          ///< page root for session listing
    html_table table;
    html_url nouvelle;
    html_form form;
    html_div div;
    std::vector<html_form_input *> boxes; //< list of checkboxes in "page"
    std::vector<session::session_summary> sess; //< list of sessions in page

	/// confirmation page and associated objects
    html_page confirm;       ///< page root for kill confirmation
    html_table ctable;
    html_yes_no_box confirmed;

    answer create_new_session(const request & req);
    void regenerate_table_page();
    void release_boxes();
    void regenerate_confirm_page();
    void kill_selected_sessions() const;

	/// class level structure holding a chooser for each user

    struct record
    {
	choose *obj;
	libthreadar::mutex lock;

	record() { obj = nullptr; };
	record(const record & ref) = delete;
	record(record && ref) noexcept = delete;
	record & operator = (const record & ref) = delete;
	record & operator = (record && ref) noexcept = delete;
	~record() { if(obj != nullptr) delete obj; };
    };

	/// the list of chooser and associated attributes

	/// maps username to a record structure (which contained the chooser)
    static std::map<std::string, record *> per_user;
    static const std::string css_class_error_msg;
    static const std::string css_class_normal_text;
};

#endif
