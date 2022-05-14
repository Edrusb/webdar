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

#ifndef WEB_INTERACTION_HPP
#define WEB_INTERACTION_HPP

    // C++ system header files


    // webdar headers
#include "mutex.hpp"

class web_interaction : public user_interaction, public req_ans
{
public:
    web_interaction(unsigned int num_line_buffer); //< max number of line of output to buffer
    ~web_interaction();

	// inherited from user_interaction
    virtual bool pause2();
    virtual std::string get_string(const std::string & message, bool echo);
    virtual void listing(const std::string & flag,
			 const std::string & perm,
			 const std::string & uid,
			 const std::string & gid,
			 const std::string & size,
			 const std::string & date,
			 const std::string & filename,
			 bool is_dir,
			 bool has_children);

    virtual web_interaction *clone() const;


	// interface for the web
    answer get_answer(const std::string & request_prefix, const request & req, const std::vector<av> & request_attributs);
	// this method provides consistent output with a field for last line of output
	// and a field for user interaction.

protected:
    virtual void inherited_warning(const std::string & message);

private:
    mutex pending_libdar; //< used to suspend libdar thread, waiting for user answer
    pthread_mutex_t output_buffer_lock; //< exclusive access to the output buffer
    std::deque<std::string> output_buffer; //< list of line to display
    unsigned int index_of_last_input; //< gives in output_buffer the number of new lines of output (or the offset of output already returned to the user)

};

#endif
