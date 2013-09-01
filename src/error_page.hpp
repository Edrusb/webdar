/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013 Denis Corbin
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

#ifndef ERROR_PAGE_HPP
#define ERROR_PAGE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "responder.hpp"
#include "html_page.hpp"
#include "html_text.hpp"
#include "html_url.hpp"
#include "html_form_input.hpp"
#include "html_form.hpp"
#include "html_div.hpp"

class error_page : public responder
{
public:
    error_page(unsigned int status_code,
	       const std::string & reason);

    void set_message_body(const std::string & message);
    void set_return_uri(const uri &ref, const std::string & label);

	/// inherited from responder
    answer give_answer(const request & req);

protected:
    virtual void prefix_has_changed();

private:
    unsigned int status;  //< return code of the answer (seen by browser, rarely visibile by the user)
    std::string msg;      //< returned reason of the answer ( seen by browser, rarely visibile by the user)

	// body_builder objects
    html_page page;      //< html construction primitive
    html_div div;        //< construction object containing both body and text
    html_text body;      //< message displayed to the user set by 'set_message_body()'
    html_text text;      //< used to build the return path in HTML body

    void field_delete();
};


#endif
