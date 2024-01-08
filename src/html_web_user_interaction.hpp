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
#include "events.hpp"
#include "actor.hpp"
#include "html_form_input.hpp"
#include "html_form.hpp"
#include "html_form_radio.hpp"
#include "html_form_fieldset.hpp"
#include "html_text.hpp"
#include "html_button.hpp"
#include "html_statistics.hpp"
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
    /// |+-----------------------------------------+|
    /// || stats (libdar::statistics)              ||
    /// |+-----------------------------------------+|
    /// |                                           |
    /// | +-------+  +-------+  +-------+  +-------+|
    /// | | ask   |  | force |  | kill  |  |finish ||
    /// | | close |  | close |  | close |  |       ||
    /// | +-------+  +-------+  +-------+  +-------+|
    /// +-------------------------------------------+

    /// \note to communicate with a libdar thread, this class relies on a web_user_interaction
    /// objects that manages exclusive access (using mutex) to data structures provided and
    /// requested by libdar.

class html_web_user_interaction: public body_builder, public actor, public events
{
public:
    	// this class generates the following events:
    static const std::string ask_end_libdar;      ///< ask_close button has been pressed (graceful ending requested)
    static const std::string force_end_libdar;    ///< force_close button has been pressed (immediate ending requested)
    static const std::string kill_libdar_thread;  ///< kill_close button has been pressed (kill thread requested)
    static const std::string close_libdar_screen; ///< finish button has been pressed
    static const std::string can_refresh;         ///< last changes makes the object html refreshable
    static const std::string dont_refresh;        ///< last changes forbid html refresh

	// class constructor assignment ops and destructors
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

	/// set the component in the finish status
    void do_finish() { set_mode(finished); };

	/// libdar strcture provided for user interaction
    std::shared_ptr<web_user_interaction> get_user_interaction() { check_libdata(); return lib_data; };

	/// libdar structure provided for user information
    html_statistics & get_statistics() { stats.set_visible(true); return stats; };

	/// hide statistics fields in the web interface

	/// \note get_statistics() unhides the field and it is hidden again, when calling clear()
    void hide_statistics() { stats.set_visible(false); };

protected:
	/// inherited from body_builder, called by the webdar thread
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;
	// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    static const std::string class_button;
    static const std::string class_web;
    static const std::string class_inter;

    enum mode_type
    {
	normal,        ///< should display web_user_interface, progressive_report and ask_close button
	end_asked,     ///< should display web_user_interface, progressive_report and force_close button
	end_forced,    ///< should display web_user_interface, progressive_report and kill_close button
	kill_forced,   ///< should display web_user_interface, progressive_report (no button)
	finished       ///< should display web_user_interface, progressive_report, close button
    };


	// fields for exchange with libdar thread
    std::shared_ptr<web_user_interaction> lib_data;

	// current display mode
    mode_type mode;

	// body_builder fields
    html_form_radio h_pause;      ///< shows when libdar request a yes/no decision from the user
    html_form_fieldset h_inter;   ///< wraps h_pause and h_inter_text and is any visible when libdar request an information from the user
    html_text h_inter_text;       ///< displays the question/request from libdar
    html_form_input h_get_string; ///< shows when libdar request a string answer from the user
    html_form h_form;             ///< html_form for the previous/above html fields
    html_text h_warnings;         ///< shows the list of warnings/message from libdar
    html_form_fieldset h_logs;    ///< wraps the h_warnings
    html_form_fieldset h_global;  ///< wraps the whole output from libdar (before stats ans buttons)

    html_statistics stats;
    html_button ask_close;        ///< button that shows to query libdar clean shutdown
    html_button force_close;      ///< button that shows to query libdar immediate shutdown
    html_button kill_close;       ///< button that shows to query libdar thread kill
    html_button finish;           ///< button that shows to let the user read last logs before closing

    bool rebuild_body_part; ///< set to true if changes/events occured while building body part, needing a whole rebuild
    bool ignore_event;      ///< if true the on_event() method does not take any action
    bool just_set;          ///< true when the control have just been activated by libdar and no answer has been provided by the user
    bool event_sent;        ///< true if button event haven been propagated for a given get_body_part()

    void adjust_visibility();
    void check_libdata() { if(!lib_data) throw WEBDAR_BUG; };
    void set_mode(mode_type m);
};


#endif
