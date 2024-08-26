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

#ifndef HTML_OVERWRITE_ACTION_HPP
#define HTML_OVERWRITE_ACTION_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "body_builder.hpp"

    /// html component used for the actions in overwriting policies (pure virtual class)

class html_overwrite_action : public body_builder
{
public:
    html_overwrite_action();
    html_overwrite_action(const html_overwrite_action & ref) = default;
    html_overwrite_action(html_overwrite_action && ref) noexcept = default;
    html_overwrite_action & operator = (const html_overwrite_action & ref) = default;
    html_overwrite_action & operator = (html_overwrite_action && ref) noexcept = default;
    ~html_overwrite_action() = default;

	/// obtain the crit_action object for libdar option
    virtual libdar::crit_ation get_overwriting_action() const = 0;

};

#endif
