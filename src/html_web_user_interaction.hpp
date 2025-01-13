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

#ifndef HTML_WEB_USER_INTERACTION_HPP
#define HTML_WEB_USER_INTERACTION_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <libthreadar/libthreadar.hpp>
#include <memory>
#include <list>

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

    /// adoption tree (= how the component show on the browser)
    ///

    /** \verbatim
	+-h_global----------------------------------+
	|                                           |
	|                                           |
	|+-h_logs----------------------------------+|
	|| h_warnings (libdar messages/warnings    ||
	|+-----------------------------------------+|
	|+-h_form----------------------------------+|
	||+-h_inter-------------------------------+||
	||| h_inter_text (question from libdar)   |||
	||| h_pause (radio button yes/no)         |||
	||+---------------------------------------+||
	||+---------------------------------------+||
	||| h_get_string(string asked from libdar)|||
	||+---------------------------------------+||
	|+-----------------------------------------+|
	|                                           |
	|+-----------------------------------------+|
	|| stats (libdar::statistics)              ||
	|+-----------------------------------------+|
	|                                           |
	| +-------+  +-------+             +-------+|
	| | ask   |  | force |             |finish ||
	| | close |  | close |             |       ||
	| +-------+  +-------+             +-------+|
	+-------------------------------------------+

	\endverbatim **/

    /// usage: once created, this component can be adopted from another body_builder
    /// The caller should use get_user_interaction() to provide a user interaction to a libthreadar::thread object
    /// for it can interact with the user thanks to this html_web_user_interaction but the thread should not be
    /// run by the caller (though the caller will still have to join() to cleanup/propagate its exception status),
    /// but passed to the run_and_control_thread() method for that.
    /// the caller (code) can still access the thread. While the user (Web UI/browser) can also stop the thread
    /// using the button that show in turn at the bottom of the component. Once the thread has finished, the component
    /// can automatically hide from the UI (auto_hide(true)) or stay visible until the user press the "finish"
    /// html_button.
    /// the caller (code) can also register to the libdar_has_finished event to be notified when the thread
    /// has ended as detected by this html_web_user_interaction (for that its get_body_part() must be regularly
    /// invoked. This can be done by mean of page refresh, though, this is not always good to use, in particular
    /// when the user (web UI side) has to fill some html fields. For that reasons two additional events
    ///- can_refresh
    ///- dont_refresh
    /// are availabe and can be registered from the caller to avoid freshing the HTML page when it conflicts with
    /// the expected HTML behavior.
    /// \note to communicate with a libdar thread, this class relies on a web_user_interaction
    /// objects that manages exclusive access (using mutex) to data structures provided and
    /// requested by libdar.

class html_web_user_interaction: public body_builder, public actor, public events
{
public:
    	// this class generates the following events:
    static const std::string display_started;     ///< when the object has been set visible
    static const std::string can_refresh;         ///< last changes makes the object html refreshable
    static const std::string dont_refresh;        ///< last changes forbid html refresh
    static const std::string libdar_has_finished; ///< inform the caller that libdar has finished and user asked to close the "window" (or autohide was set)

	// class constructor assignment ops and destructors
    html_web_user_interaction(unsigned int x_warn_size = 25);
    html_web_user_interaction(const html_web_user_interaction & ref) = delete;
    html_web_user_interaction(html_web_user_interaction && ref) noexcept = delete;
    html_web_user_interaction & operator = (const html_web_user_interaction & ref) = delete;
    html_web_user_interaction & operator = (html_web_user_interaction && ref) noexcept = delete;
    ~html_web_user_interaction();

	/// change the number of last warnings to display
    void set_warning_list_size(unsigned int size) { check_libdata(); lib_data->set_warning_list_size(size); };

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// when the control thread has finished, this html_web_user_interaction object becomes invisible

	/// by default the object stays visible and user has to click on the "close" button to go forward
	/// \param[in] mode if set to true, no "close" button shows once the thread has complete
	/// \param[in] unless_interrupted if set to true and the thread has ended due to user request
	/// the autohide feature is disabled and a "close" button shows.
    void auto_hide(bool mode, bool unless_interrupted) { autohide = mode; hide_unless_interrupted = unless_interrupted; };

	/// clear counters, logs and reset html interface and make the component become visible if it was not already

	/// \note this implies hide_statistics, but counters labels stay untouched
    void clear();

	/// libdar strcture provided for user interaction
    std::shared_ptr<web_user_interaction> get_user_interaction() { check_libdata(); return lib_data; };

	/// libdar structure provided for user information
    html_statistics & get_statistics() { stats.set_visible(true); return stats; };

	/// hide statistics fields in the web interface

	/// \note get_statistics() unhides the field and it is hidden again, when calling clear()
    void hide_statistics() { stats.set_visible(false); };

	/// provide a libdar thread to be managed by this html_web_component (stop actions)

	/// \param[in] arg is the thread to be managed, it must have been setup but not run() by the caller
	/// this method will run() the thread, monitor its liveness then join() it when it has completed.
	/// The caller must have registered to the event libdar_has_finished to be informed when the thread
	/// will have completed. The caller can also/instead join() the corresponding thread, but is then
	/// stuck until the thread ends.
	/// During the life of the thread, this body_builder component displays buttons to stop the
	/// provided thread, as well as the output of the get_user_interaction() returned component (which
	/// is a libdar::user_interaction object).
	///
	/// \note the run_and_control_thread() method can be invoked while a thread is already managed
	/// by this object. THe object monitors the liveness status of all managed thread and join() it
	/// but only triggers the event close_libdar_screen once all thread have ended and user has clicked
	/// on the "close" button (or all thread have ended and auto_hide() was set). This does not prevent
	/// the caller to join() any thread it need to wait for its termination. Of course if the user
	/// asks to abort the libdar process, first only the last provided thread is requested to stop or
	/// in a second time (upon user action) all threads are stopped in reverse order they have been
	/// given for management.

	///
	/// \note, this is the duty of the caller to give to the thread as libdar::user_interaction
	/// the web_user_interaction object returned by the get_user_interaction() method, for libdar
	/// to also interact with the user (display message, ask questions), using this
	/// html_web_user_interaction.
	///
	/// \note, the provided thread object is not managed by this object, it must exist during
	/// the whole life of the object until it ends or is aborted.
    void run_and_control_thread(libthreadar::thread* arg);

    	/// whether a libdar thread is running under "this" management
    bool is_libdar_running() const;

	/// whether libdar thread has been aborted (to be checked by the caller upon libdar_has_finished event)
    bool has_libdar_been_aborted() const { return was_interrupted; };


protected:
	/// inherited from body_builder, called by the webdar thread
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;
	// inherited from body_builder
    virtual void new_css_library_available() override;

	// inherited from body_builder
    virtual void my_visibility_has_changed() override;


private:
    static const std::string class_button;
    static const std::string class_web;
    static const std::string class_inter;

	// internal event names (assigned to control buttons and on which we registered)
    static const std::string ask_end_libdar;      ///< ask_close button has been pressed (graceful ending requested)
    static const std::string force_end_libdar;    ///< force_close button has been pressed (immediate ending requested)
    static const std::string close_libdar_screen; ///< finish button has been pressed

    enum mode_type
    {
	normal,        ///< managed thread may be running (then managed_threads.empty() == false)
	end_asked,     ///< thread pending graceful stop
	end_forced,    ///< thread pending graceful immediate stop
	finished,      ///< thread has finished, display kept for user to ack the status
	closed         ///< user has acked the status or auto_hide was set
    };


	// fields for exchange with libdar thread
    std::shared_ptr<web_user_interaction> lib_data;

	// current display mode
    mode_type mode;               ///< current mode of operation
    bool autohide;                ///< if true the finish status leads the html component to become invisible
    bool hide_unless_interrupted; ///< disable autohide when thread ended due to user request
    bool was_interrupted;         ///< whether user asked for thread cancellation or exception thrown within thread

	// body_builder fields
    html_form_radio h_pause;      ///< shows when libdar request a yes/no decision from the user
    html_form_fieldset h_inter;   ///< wraps h_pause and h_inter_text and is any visible when libdar request an information from the user
    html_text h_inter_text;       ///< displays the question/request from libdar
    html_form_fieldset h_gtstr_fs;///< box arount h_get_string
    html_form_input h_get_string; ///< shows when libdar request a string answer from the user
    html_form h_form;             ///< html_form for the previous/above html fields
    html_text h_warnings;         ///< shows the list of warnings/message from libdar
    html_form_fieldset h_logs;    ///< wraps the h_warnings
    html_form_fieldset h_global;  ///< wraps the whole output from libdar (before stats ans buttons)

    html_statistics stats;        ///< holds a libdar::statistics for progressive report of libdar operations on archives
    html_button ask_close;        ///< button that shows to query libdar clean shutdown
    html_button force_close;      ///< button that shows to query libdar immediate shutdown
    html_text helper_text;        ///< shows when abortion requested but a question from libdar need an answer first
    html_button finish;           ///< button that shows to let the user read last logs before closing

    bool ignore_event;      ///< if true the on_event() method does not take any action

    std::list<libthreadar::thread*> managed_threads;


    void adjust_visibility();
    void check_libdata() { if(!lib_data) throw WEBDAR_BUG; };
    void set_mode(mode_type m);
    void update_html_from_libdar_status();
    void check_thread_status();           ///< trigger the finished mode if no thread still runs
    void clean_end_threads(bool force);
    void trigger_refresh();
    void remove_nullptr_from_managed_threads();

};


#endif
