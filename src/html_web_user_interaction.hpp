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

    /// body_builder component, providing an html interface to libdar::user_interaction

    /// adoption tree:
    /// +-h_global----------------------------------+
    /// |                                           |
    /// |+-h_form----------------------------------+|
    /// ||+-h_inter-------------------------------+||
    /// ||| h_inter_text (question from libdar)   |||
    /// ||| h_pause (radio button yes/no)         |||
    /// ||+---------------------------------------+||
    /// ||+---------------------------------------+||
    /// ||| h_get_string(string asked from libdar)|||
    /// ||+---------------------------------------+||
    /// |+-----------------------------------------+|
    /// |                                           |
    /// |+-h_logs----------------------------------+|
    /// || h_warnings (libdar messages/warnings    ||
    /// |+-----------------------------------------+|
    /// |                                           |
    /// +-------------------------------------------+

    /// \note to communicate with a libdar thread, this class relies on a web_user_interaction
    /// objects that manages exclusive access (using mutex) to data structures provided and
    /// requested by libdar.

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
    void set_warning_list_size(unsigned int size) { check_libdata(); lib_data->set_warning_list_size(size); };

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// clear logs and reset html interface
    void clear();

	/// true if no input is requested from libdar, thus HTML refresh
	/// can take place
    bool can_refresh() const { return !h_form.get_visible(); };

    std::shared_ptr<web_user_interaction> get_user_interaction() { check_libdata(); return lib_data; };

protected:
	/// inherited from body_builder, called by the webdar thread
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;
	// inherited from body_builder
    virtual void new_css_library_available() override;

private:
	// fields for exchange with libdar thread
    std::shared_ptr<web_user_interaction> lib_data;

	// body_builder fields
    html_form_radio h_pause;      ///< shows when libdar request a yes/no decision from the user
    html_form_fieldset h_inter;   ///< wraps h_pause and h_inter_text and is any visible when libdar request an information from the user
    html_text h_inter_text;       ///< displays the question/request from libdar
    html_form_input h_get_string; ///< shows when libdar request a string answer from the user
    html_form h_form;             ///< html_form for the previous/above html fields
    html_text h_warnings;         ///< shows the list of warnings/message from libdar
    html_form_fieldset h_logs;    ///< wraps the h_warnings
    html_form_fieldset h_global;  ///< wraps the whole output from libdar

    bool rebuild_body_part; //< set to true if changes occured while building body part, needing a whole rebuild
    bool ignore_event;      //< if true the on_event() method does not take action
    bool just_set;          //< true when the control have just been activated and no answer has been provided by the user

    void adjust_visibility();
    void check_libdata() { if(!lib_data) throw WEBDAR_BUG; };
};


#endif
