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

#ifndef WEB_USER_INTERACTION_HPP
#define WEB_USER_INTERACTION_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>
#include <list>
#include <dar/libdar.hpp>
#include <libthreadar/libthreadar.hpp>

    // webdar headers


    /// class web_user_interaction provides a libdar::user_interaction interface to libdar

    /// this class is not an html_ body_builder class but rather holds information fields and status
    /// provided by libdar through the libdar::user_interaction interface. The main role of this
    /// class is to allow any thread to get access to this libdar running thread provided information,
    /// by mean of mutex on the internal/private data structure.

    /// \note web_user_interaction can has two sides, the libdar::user_interaction interface which
    /// is implemented by the inherited_* protected methods and the web_user_interaction interface
    /// that is provided by the public methods of the class. This object is given to libdar
    /// as is for user_interaction. And this object is consulted/interacted inside webdar possibly
    /// from another thread for html_ interaction (see html_web_user_interaction class for example)

class web_user_interaction : public libdar::user_interaction
{
public:
    web_user_interaction(unsigned int x_warn_size = 10); ///< the argument is the number latest message to retain from libdar
    web_user_interaction(const web_user_interaction & ref) = delete;
    web_user_interaction(web_user_interaction && ref) noexcept = delete;
    web_user_interaction & operator = (const web_user_interaction & ref) = delete;
    web_user_interaction & operator = (web_user_interaction && ref) noexcept = delete;
    ~web_user_interaction();

	/// change the number of last warnings to display
    void set_warning_list_size(unsigned int size);

	/// clear logs and reset the object
    void clear();

	/// obtain a copy of the current log buffer
    std::list<std::string> get_warnings();

	/// wether libdar is pending for pause answer
    bool pending_pause(std::string & msg) const;

	/// wether libdar is pending for get_string answer
    bool pending_get_string(std::string & msg, bool & echo) const;

	/// wether libdar is pending for a get_secu_string answer
    bool pending_get_secu_string(std::string & msg, bool & echo) const;

	/// provide the answer to libdar for pause() request
    void set_pause_answer(bool val);

	/// provide the answer to libdar for get_string() request
    void set_get_string_answer(const std::string & val);

    	/// provide the answer to libdar for get_secu_string() request
    void set_get_secu_string_answer(const libdar::secu_string & val);

	/// whether libdar is waiting for an answer
    bool has_libdar_pending() const;

protected:

	// inherited from libdar::user_interaction

    virtual void inherited_message(const std::string & message) override;
    virtual bool inherited_pause(const std::string & message) override;
    virtual std::string inherited_get_string(const std::string & message, bool echo) override;
    virtual libdar::secu_string inherited_get_secu_string(const std::string & message, bool echo) override;

private:
    mutable libthreadar::mutex control; ///< control access to any value this object has
    libthreadar::semaphore libdar_sem;  ///< libdar thread wait on it for an answer
    bool answered;                      ///< true if the pending pause(), get_string() or get_secu_string() has been answered, so the question has not to be shown twice

	// pause() fields
    bool pause_pending;    ///< true if a pause() is pending for a response
    std::string pause_msg; ///< the request to answser to
    bool pause_ans;        ///< the answer to the request

	// get_string() fields
    bool get_string_pending;    ///< true if a get_string() is pending for a response
    std::string get_string_msg; ///< the libdar message for get_string()
    bool get_string_echo;       ///< whether answer has to be echoed
    std::string get_string_ans; ///< the user provided answer

	// get_secu_string() fields
    bool get_secu_string_pending;    ///< true if a get_secu_string() is pending for a response
    std::string get_secu_string_msg; ///< the libdar message for get_secu_string()
    bool get_secu_string_echo;       ///< whether the answer has to be echoed
    libdar::secu_string get_secu_string_ans; ///< the user provided secu_string

	// libdar warnings (= logs)
    std::list<std::string> warnings;  ///< lines (message/warning) sent by libdar
    unsigned warn_size;               ///< max number of line to record from libdar
};

#endif

