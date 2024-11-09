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

#ifndef HTML_COMPRESSION_HPP
#define HTML_COMPRESSION_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <string>

    // webdar headers

#include "html_form_select.hpp"

    /// html components to let the user define the compression algorithm

class html_compression : public html_form_select
{
public:
    static const std::string changed;  /// common event name for all object of this class

    html_compression(const std::string & title);
    html_compression(const html_compression & ref) = delete;
    html_compression(html_compression && ref) noexcept = delete;
    html_compression & operator = (const html_compression & ref) = delete;
    html_compression & operator = (html_compression && ref) noexcept = delete;
    ~html_compression() = default;

    libdar::compression get_value() const;
    void set_value(libdar::compression val);

	/// actor indirect inheritance
    virtual void on_event(const std::string & event_name) override { act(changed); };
};

#endif
