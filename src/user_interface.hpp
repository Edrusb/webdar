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

#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "responder.hpp"
#include "saisie.hpp"
#include "html_statistics.hpp"
#include "events.hpp"
#include "html_libdar_running.hpp"
#include "html_error.hpp"
#include "html_listing_page.hpp"
#include "archive_test.hpp"
#include "archive_restore.hpp"
#include "archive_compare.hpp"
#include "archive_create.hpp"
#include "archive_isolate.hpp"
#include "archive_merge.hpp"
#include "archive_init_list.hpp"

class user_interface : public responder, public events, public actor
{
public:
	/// available event for that class
    static const std::string closing;      //< signal end of user session

    user_interface();
    user_interface(const user_interface & ref) = delete;
    user_interface(user_interface && ref) noexcept = delete;
    user_interface & operator = (const user_interface & ref) = delete;
    user_interface & operator = (user_interface && ref) noexcept = delete;
    ~user_interface();

	/// inherited from responder
    virtual answer give_answer(const request & req);

	/// inherited from actor
    virtual void on_event(const std::string & event_name);

	/// tells whether libdar is running
    bool is_libdar_running() const { return libdar_running; };

	/// provide the name of the session (given by the user);
    std::string get_session_name() const;

	/// set the session name (customizable by user)
    void set_session_name(const std::string & name);


protected:

	/// inherited from responder
    virtual void prefix_has_changed();

private:
    enum mode_type
    {
	config,        //< should display parameter selection
	listing_open,  //< should display the web_user_interface, progressive_report an cancellation button
	listing,       //< should display listing navigation window
	running,       //< should display web_user_interface, progressive_report and cancellation button
	error,         //< should display the error
    };

    std::string sessname;  //< customized name of that session (empty string by default)
    libthreadar::mutex mut_sessname; //< drives the access to sessname, by set and get session name methods
    mode_type mode;        //< current mode
    mode_type return_mode; //< mode in which to return from error status
    bool close_requested;  //< whether session close has been asked
    bool mode_changed;     //< whether mode has changed

    saisie parametrage;            //< page issued in mode == config
    html_libdar_running in_action; //< page issued when a libdar thread is running (mode == running)
    html_error in_error;           //< page issued when an exception has been caught (mode == error)
    html_listing_page in_list;     //< page issued when proceeding to archive listing

    bool libdar_running;      //< whether a libdar child thread is running
    libthreadar::thread *current_thread; //< points to the running thread (either arch_test, arch_merge, ....)
    archive_test arch_test;   //< holds thread created for testing archives
    archive_restore arch_rest;//< holds thread created for extracting archives
    archive_compare arch_diff;//< holds thread created for comparing archives
    archive_create arch_create; //< holds thread created for archive creation
    archive_isolate arch_isolate; //< holds thread created for archive isolation
    archive_merge arch_merge; //< holds thread created for archive merging
    archive_init_list arch_init_list; //< holds thread created to open an archive for listing

	/// available parameters for libdar execution
    const saisie & get_parametrage() const { return parametrage; };

	/// provides the libdar::user_interaction for libdar execution
    web_user_interaction & get_user_interaction() { return in_action.get_user_interaction(); };

	/// provides the a libdar::statistics datastructure for progressive report in libdar execution
    html_statistics & get_statistics() { return in_action.get_statistics(); };

    void go_restore();
    void go_diff();
    void go_test();
    void go_create();
    void go_isolate();
    void go_merge();
    void go_init_list();
};

#endif
