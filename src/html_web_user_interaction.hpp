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

#ifndef HTML_WEB_USER_INTERACTION_HPP
#define HTML_WEB_USER_INTERACTION_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"
#include "html_form_input.hpp"
#include "html_form.hpp"
#include "html_form_radio.hpp"
#include "html_form_fieldset.hpp"
#include "html_text.hpp"
#include "events.hpp"
#include "web_user_interaction.hpp"


class html_web_user_interaction: public body_builder, public actor
{
public:
    html_web_user_interaction(unsigned int x_warn_size = 25);
    html_web_user_interaction(const html_web_user_interaction & ref) = delete;
    html_web_user_interaction(html_web_user_interaction && ref) noexcept = delete;
    html_web_user_interaction & operator = (const html_web_user_interaction & ref) = delete;
    html_web_user_interaction & operator = (html_web_user_interaction && ref) noexcept = delete;
    ~html_web_user_interaction() = default;

	/// change the number of last warnings to display
    void set_warning_list_size(unsigned int size) { lib_data.set_warning_list_size(size); };

	/// inherited from body_builder, called by the webdar thread
    virtual std::string get_body_part(const chemin & path,
				      const request & req);

	/// inherited from actor
    virtual void on_event(const std::string & event_name);

	/// clear logs and reset html interface
    void clear();

	/// true if no input is requested from libdar, thus HTML refresh
	/// can take place
    bool can_refresh() const { return !h_form.get_visible(); };

    web_user_interaction & get_user_interaction() { return lib_data; };

private:
	// fields for exchange with libdar thread
    web_user_interaction lib_data;

	// body_builder fields
    html_form_radio h_pause;
    html_form_fieldset h_inter;
    html_text h_inter_text;
    html_form_input h_get_string;
    html_form h_form;
    html_text h_warnings;
    html_form_fieldset h_logs;
    html_form_fieldset h_global;
    bool rebuild_body_part; //< set to true if changes occured while building body part, needing a whole rebuild
    bool ignore_event;      //< if true the on_event() method does not take action
    bool just_set;          //< true when the control have just been activated and no answer has been provided by the user

    void adjust_visibility();
};


#endif
