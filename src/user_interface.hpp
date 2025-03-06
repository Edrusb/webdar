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

#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

    // C system header files
#include "my_config.h"
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
#include "html_libdar_running_page.hpp"
#include "html_error.hpp"
#include "html_listing_page.hpp"
#include "html_summary_page.hpp"
#include "archive_test.hpp"
#include "archive_restore.hpp"
#include "archive_compare.hpp"
#include "archive_create.hpp"
#include "archive_isolate.hpp"
#include "archive_merge.hpp"
#include "archive_init_list.hpp"
#include "archive_repair.hpp"
#include "html_fichier.hpp"

    /// main webdar html components that defines for a given session the type of output (config pages, libdar output, error, etc.)

    /// \note the configuration downloading process is the following: at construction time, the parametrage field (saisie class)
    /// is set with an html_fichier object using saisie::set_data_place() method. When the user clicks on the download button
    /// inside class saisie (html_bibliotheque field) this triggers the browser to save to file the body of the next http response,
    /// and the saisie object saves into the html_fichier the json configuration of the bibiliotheque and triggers the event
    /// event_download. The user_interface class has registered to this event which leads to change to mode to "download", but
    /// the body_builder evaluation of field parametrage continues to its end. When it has finished if the mode has been changed to
    /// download the obtained http body is just dropped and replaced by the content of the html_fichier we provided to the
    /// parametrage object and the mode is set back to config before returning the http response.

class user_interface : public responder, public events, public actor
{
public:
	/// available event for that class
    static const std::string closing;      ///< signal end of user session

    user_interface();
    user_interface(const user_interface & ref) = delete;
    user_interface(user_interface && ref) noexcept = delete;
    user_interface & operator = (const user_interface & ref) = delete;
    user_interface & operator = (user_interface && ref) noexcept = delete;
    ~user_interface();

	/// inherited from responder
    virtual answer give_answer(const request & req) override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// provide the name of the session (given by the user);
    std::string get_session_name() const;

	/// set the session name (customizable by user)
    void set_session_name(const std::string & name);

	/// define the username that the current session is owned by
    void set_username(const std::string & username) { parametrage.set_username(username); };

    	/// whether a libdar thread is running
    bool is_libdar_running() const { return in_action.is_libdar_running(); };

	/// whether user has requested to disconnect
    bool disconnection_requested() const { bool ret = disconnect_req; disconnect_req = false; return ret; };

protected:

	/// inherited from responder
    virtual void prefix_has_changed() override;

private:
    enum mode_type
    {
	config,        ///< should display parameter selection
	listing_open,  ///< should display the web_user_interface, progressive_report an cancellation button
	listing,       ///< should display listing navigation window
	summary_open,  ///< should display the web_user_interface, progressive_report and cancellation button
	summary,       ///< should display the summary window
	running,       ///< should display web_user_interface, progressive_report and cancellation button
	error,         ///< should display the error
	download       ///< should provide content of fichier for download or display (depending on the link used to get to this status)
    };

    std::string sessname;  ///< customized name of that session (empty string by default)
    mutable libthreadar::mutex mut_sessname; ///< drives the access to sessname, by set and get session name methods
    mode_type mode;        ///< current mode
    mode_type return_mode; ///< mode in which to return from error status
    bool close_requested;  ///< whether session close has been asked
    bool mode_changed;     ///< whether mode has changed
    mutable bool disconnect_req; ///< whether user has requested a session disconnection

    saisie parametrage;            ///< page issued in mode == config
    html_libdar_running_page in_action; ///< page issued when a libdar thread is running (mode == running)
    html_error in_error;           ///< page issued when an exception has been caught (mode == error)
    html_listing_page in_list;     ///< page issued when proceeding to archive listing
    html_summary_page in_summ;     ///< page issued when proceeding to archive summary
    std::shared_ptr<html_fichier> data; ///< content to send in download mode

    archive_test arch_test;        ///< holds thread created for testing archives
    archive_restore arch_rest;     ///< holds thread created for extracting archives
    archive_compare arch_diff;     ///< holds thread created for comparing archives
    archive_create arch_create;    ///< holds thread created for archive creation
    archive_isolate arch_isolate;  ///< holds thread created for archive isolation
    archive_merge arch_merge;      ///< holds thread created for archive merging
    archive_repair arch_repair;    ///< holds thread created for archive repairing
    archive_init_list arch_init_list; ///< holds thread created to open an archive for listing
    libthreadar::thread *current_thread; ///< points to the running thread (either arch_test, arch_merge, ....)

	/// available parameters for libdar execution
    const saisie & get_parametrage() const { return parametrage; };

	/// provides the libdar::user_interaction for libdar execution
    std::shared_ptr<html_web_user_interaction> get_html_user_interaction() { return in_action.get_html_user_interaction(); };

    void go_restore();
    void go_diff();
    void go_test();
    void go_create();
    void go_isolate();
    void go_merge();
    void go_init_list();
    void go_repair();
};

#endif
