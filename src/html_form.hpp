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

#ifndef HTML_FORM_HPP
#define HTML_FORM_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"

    /// class html_form implements HTML form feature

    /// html_form object should adopt html_form_fieldset
    /// or directly html_form_input, html_datetime, html_form_radio
    /// or any more generally called html_form_field object
    /// Visually, html_form only shows a button to validate the change
    /// set by the user in the displayed form. The text of the button
    /// is given as argument of this class constructor

class html_form : public body_builder
{
public:
    html_form(const std::string & validate_msg = "send") { go_mesg = validate_msg; };
    html_form(const html_form & ref) = default;
    html_form(html_form && ref) noexcept = default;
    html_form & operator = (const html_form & ref) = default;
    html_form & operator = (html_form && ref) noexcept = default;
    ~html_form() = default;

protected:
	/// inherited methods from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    std::string go_mesg;

};

#endif
