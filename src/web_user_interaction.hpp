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


class web_user_interaction : public libdar::user_interaction
{
public:
    web_user_interaction(unsigned int x_warn_size = 10);
    web_user_interaction(const web_user_interaction & ref) { copy_from(ref); };
    const web_user_interaction & operator = (const web_user_interaction & ref) { detruit(); copy_from(ref); return *this; };
    ~web_user_interaction() { detruit(); };

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
    struct shared_data
    {
	libthreadar::mutex control;    //< control access to any value this object has
	unsigned int instances;        //< number of reference existing toward this struct object, this object data get destroyed when this number drops to zero
	libthreadar::semaphore libdar_sem;  //< libdar thread wait on it for an answer
	bool answered;                 //< true if the pending pause(), get_string() or get_secu_string() has been answered, so the question has not to be shown twice

	    // pause() fields
	bool pause_pending;    //< true if a pause() is pending for a response
	std::string pause_msg; //< the request to answser to
	bool pause_ans;        //< the answer to the request

	    // get_string() fields
	bool get_string_pending;//< true if a get_string() is pending for a response
	std::string get_string_msg;
	bool get_string_echo;
	std::string get_string_ans;

	    // get_secu_string() fields
	bool get_secu_string_pending; //< true if a get_secu_string() is pending for a response
	std::string get_secu_string_msg;
	bool get_secu_string_echo;
	libdar::secu_string get_secu_string_ans;

	    // libdar warnings (= logs)
	std::list<std::string> warnings;
	unsigned warn_size;

	shared_data(unsigned int size);
	~shared_data() noexcept(false);
	void clear(); // reset fields to default values
	void incr();
	bool decr_and_can_delete(); // if true is returned the object can be deleted
    };

    shared_data *data;

    void check_data() const;
    void copy_from(const web_user_interaction & ref);
    void detruit();
};

#endif

