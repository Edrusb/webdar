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

#ifndef SESSION_HPP
#define SESSION_HPP

    // C system header files
extern "C"
{
}

    // C++ system header files
#include <list>
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "user_interface.hpp"
#include "events.hpp"

    /// class session - holds information about a current user session

    /// a session object stores a work under progress. An internal mutex
    /// avoid two concurrent requests to be addressed a given session object.
    /// Session object are sollicitated by parser and chooser
    /// objects.

class session : public responder, public actor
{
public:

	//////////////////////////
	// Object methods
	//

	// constructor of the class are left private intentionnaly
	// this class provides a global table of session one can create, lookup or destroy objects in/from this table

	/// inherited from responder
    virtual answer give_answer(const request & req) override;

    bool has_waiting_threads() const { return lock_wui.waiting_thread(); };
    bool has_working_server() const { return lock_wui.working_thread(); };
    std::string get_session_ID() const { return session_ID; };


	/// inherited from actor parent class
    virtual void on_event(const std::string & event_name) override;


	//////////////////////////
	// Class types and methods
	//

	/// summary information of sessions
    struct session_summary
    {
	std::string owner;        //< owner of the session
	std::string session_ID;   //< session ID: internal unique identifier for that session
	std::string session_name; //< name given to that session by the user
	bool locked;              //< whether the session is locked by a thread
	bool libdar_running;      //< whether a libdar job is running
	bool closing;             //< whether the session is pending for closure
	void clear() { owner = session_ID = ""; locked = libdar_running = closing = false; };
    };

    static unsigned int get_num_session();
    static unsigned int get_num_session(const std::string & user);
    static std::vector<session_summary> get_summary();
    static bool get_session_info(const std::string & session_ID, session_summary & val);
    static std::string create_new(const std::string & owner); /// returns the session_ID of the newly created session
    static session *acquire_session(const std::string & session_ID);
    static void release_session(session *sess);
    static bool close_session(const std::string & session_ID); //< return true if the session exists and has been flagged for destruction

private:
	/// constructor
    session();
    session(const session & ref) = delete;
    session(session && ref) = delete;
    session & operator = (const session & ref) = delete;
    session & operator = (session && ref) = delete;
    ~session() = default;

	/// set session_ID
    void set_session_id(const std::string & sessid);

    libthreadar::semaphore lock_wui; //< required locking before accessing wui field
    user_interface wui;       //< object containing the current Web User Interface; is managed by the session object and should never be nullptr
    pthread_t tid;            //< tid of the thread that acquired lock on that object
    std::string session_ID;   //< session_ID info (duplicated info to avoid table lookup and mutex lock)

    void check_caller() const; //< test whether the caller has properly acquired the lock on this object


	////////////////
	// static types, variables and methods
	//

	/// wraps a session object with some metadata
    struct table
    {
	std::string owner;      //< to whom the session is
	session *reference;     //< object reference
	unsigned int ref_given; //< number of time the reference to that object has been given
	bool closing;           //< if true the reference must not be given any longer
	void clear() { owner = ""; reference = nullptr; ref_given = 0; closing = false; };
    };

    static libthreadar::mutex lock_running;       //< control access to runnng_session static table
    static std::map<std::string, table> running_session;     //< list of existing sessions
    static session_summary publish(std::map<std::string, table>::iterator it);
};

#endif
