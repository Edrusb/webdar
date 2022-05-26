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

#ifndef HTML_SIZE_UNIT_HPP
#define HTML_SIZE_UNIT_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "html_form_select.hpp"

class html_size_unit : public body_builder, public actor
{
public:
    html_size_unit();
    html_size_unit(const html_size_unit & ref) = default;
    html_size_unit(html_size_unit && ref) noexcept = default;
    html_size_unit & operator = (const html_size_unit & ref) = default;
    html_size_unit & operator = (html_size_unit && ref) noexcept = default;
    ~html_size_unit() = default;

    libdar::infinint get_value() const;

	/// body_builder indirect inheritance
    virtual std::string get_body_part(const chemin & path,
				      const request & req) override;

	/// actor inheritance
    virtual void on_event(const std::string & event_name) override;

private:
    html_form_select unit;
    html_form_select SI_mode;
    bool changed;


    void set_fields();
};

#endif
