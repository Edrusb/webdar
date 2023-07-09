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

#ifndef HTML_DATETIME_HPP
#define HTML_DATETIME_HPP

    // C system header files
extern "C"
{
#include <time.h>
}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "html_form_input.hpp"
#include "html_form_select.hpp"


class html_datetime : public body_builder
{
public:
    html_datetime(const std::string & title);
    html_datetime(const html_datetime & ref) = default;
    html_datetime(html_datetime && ref) noexcept = default;
    html_datetime & operator = (const html_datetime & ref) = default;
    html_datetime & operator = (html_datetime && ref) noexcept = default;
    ~html_datetime() = default;

    libdar::infinint get_value() const;
    void set_value(libdar::infinint val);

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    html_form_input year;
    html_form_select month;
    html_form_select day;
    html_form_select hour;
    html_form_select minute;

    void update_from(const struct tm & val);
};




#endif
