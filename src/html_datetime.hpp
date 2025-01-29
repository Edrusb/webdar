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

#ifndef HTML_DATETIME_HPP
#define HTML_DATETIME_HPP

    // C system header files
#include "my_config.h"
extern "C"
{
#if HAVE_TIME_H
#include <time.h>
#endif
}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "html_form_input.hpp"
#include "html_form_select.hpp"
#include "events.hpp"
#include "actor.hpp"

    /// class html_datetime is a html_form_field object

    /// like html_form_input it is expected to be adopted directly
    /// or thrown an html_form_fieldset by an html_form object
    /// and let the user provide a date information

class html_datetime : public body_builder,
		      public events,
		      public actor
{
public:
    static const std::string changed;

    html_datetime(const std::string & title);
    html_datetime(const html_datetime & ref) = delete;
    html_datetime(html_datetime && ref) noexcept = delete;
    html_datetime & operator = (const html_datetime & ref) = delete;
    html_datetime & operator = (html_datetime && ref) noexcept = delete;
    ~html_datetime() = default;

	/// return the provided date in the form a number of seconds since 1969
    libdar::infinint get_value() const;

	/// setup the initial value displayed to the user
    void set_value(const libdar::infinint & val);

	/// inherited from actor parent class
    virtual void on_event(const std::string & event_name) override;

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
