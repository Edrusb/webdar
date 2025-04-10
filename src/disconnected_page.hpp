/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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

#ifndef DISCONNECTED_PAGE_HPP
#define DISCONNECTED_PAGE_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "responder.hpp"
#include "html_page.hpp"
#include "html_text.hpp"

    /// html component used for the user to define a has algorithm

class disconnected_page : public responder
{
public:
    static const std::string changed;

    disconnected_page();
    disconnected_page(const disconnected_page & ref) = delete;
    disconnected_page(disconnected_page && ref) noexcept = delete;
    disconnected_page & operator = (const disconnected_page & ref) = delete;
    disconnected_page & operator = (disconnected_page && ref) noexcept = delete;
    ~disconnected_page() = default;

	/// wether to redirect to the uri with trimmed path
    void set_redirect(bool mode) { redir = mode; };

    	/// inherited from responder
    virtual answer give_answer(const request & req) override;

protected:
    	/// inherited from responder
    virtual void prefix_has_changed() override;

private:
    bool redir;
    html_page page;
    html_text msg;
};

#endif
