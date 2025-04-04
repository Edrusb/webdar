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

#ifndef HTML_COMPARISON_FIELDS_HPP
#define HTML_COMPARISON_FIELDS_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <vector>
#include <string>
#include <dar/libdar.hpp>

    // webdar headers
#include "html_form_select.hpp"

    /// html component in the form of a drop-down list to let the user define a libdar comparison field value

class html_comparison_fields : public html_form_select
{
public:
    html_comparison_fields();
    html_comparison_fields(const html_comparison_fields & ref) = default;
    html_comparison_fields(html_comparison_fields && ref) noexcept = default;
    html_comparison_fields & operator = (const html_comparison_fields & ref) = default;
    html_comparison_fields & operator = (html_comparison_fields && ref) noexcept = default;
    ~html_comparison_fields() = default;

    libdar::comparison_fields get_value() const;
    void set_value(libdar::comparison_fields val);
};




#endif
