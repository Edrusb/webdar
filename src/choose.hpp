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

#ifndef CHOOSE_HPP
#define CHOOSE_HPP

    // C system header files
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

class choose : public responder
{
public:

    static answer give_answer_for(const std::string & user, const request & req);

    static void cleanup_memory();

private:

	// constructor
    choose(const std::string & user);
    ~choose() { release_boxes(); };

	// inherited from responder
    answer give_answer(const request & req);


    std::string owner;       //< list only sessions for that owner
    bool confirm_mode;       //< whether we display session list or kill confirmation
	/// listing session page and associated objects
    html_page page;         //< page root for session listing
    html_table table;
    html_url nouvelle;
    html_form form;
    html_div div;
    std::vector<html_form_input *> boxes; //< list of checkboxes in "page"
    std::vector<session::session_summary> sess; //< list of sessions in page

	/// confirmation page and associated objects
    html_page confirm;      //< page root for kill confirmation
    html_table ctable;
    html_yes_no_box confirmed;

    answer create_new_session(const request & req);
    void regenerate_table_page();
    void release_boxes();
    void regenerate_confirm_page();
    void kill_selected_sessions() const;

	// class structure

    struct record
    {
	choose *obj;
	libthreadar::mutex lock;
	record() { obj = nullptr; };
	~record() { if(obj != nullptr) delete obj; };
    };

    static std::map<std::string, record *> per_user;
};

#endif
