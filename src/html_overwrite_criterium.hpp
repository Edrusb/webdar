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

#ifndef HTML_OVERWRITE_CRITERIUM_HPP
#define HTML_OVERWRITE_CRITERIUM_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <memory>

    // webdar headers
#include "body_builder.hpp"

    /// html ancestor class used for the criterium in html_form_overwrite_conditional_action class

    /// \note this class is a pure virtual class and has no link with html_form class unlike
    /// its inherited classes (html_form_overwrite_base_criterium and
    /// html_form_overwrite_combining_criterium, for example).

class html_overwrite_criterium : public body_builder
{
public:
    html_overwrite_criterium() {};
    html_overwrite_criterium(const html_overwrite_criterium & ref) = default;
    html_overwrite_criterium(html_overwrite_criterium && ref) noexcept = default;
    html_overwrite_criterium & operator = (const html_overwrite_criterium & ref) = default;
    html_overwrite_criterium & operator = (html_overwrite_criterium && ref) noexcept = default;
    ~html_overwrite_criterium() = default;

	/// obtain the crit_criterium object for libdar option
    virtual std::unique_ptr<libdar::criterium> get_overwriting_criterium() const = 0;

};

#endif
