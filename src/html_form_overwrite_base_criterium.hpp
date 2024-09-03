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

#ifndef HTML_FORM_OVERWRITE_BASE_CRITERIUM_HPP
#define HTML_FORM_OVERWRITE_BASE_CRITERIUM_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_overwrite_criterium.hpp"
#include "actor.hpp"
#include "body_builder.hpp"
#include "html_form_input.hpp"
#include "html_datetime.hpp"
#include "html_form_select.hpp"
#include "html_form_fieldset.hpp"

    /// html component to be adopted by an html_form that implements atomic/base actions for overwriting policies

class html_form_overwrite_base_criterium: public html_overwrite_criterium, public actor
{
public:
    html_form_overwrite_base_criterium();
    html_form_overwrite_base_criterium(const html_form_overwrite_base_criterium & ref) = default;
    html_form_overwrite_base_criterium(html_form_overwrite_base_criterium && ref) noexcept = default;
    html_form_overwrite_base_criterium & operator = (const html_form_overwrite_base_criterium & ref) = default;
    html_form_overwrite_base_criterium & operator = (html_form_overwrite_base_criterium && ref) noexcept = default;
    ~html_form_overwrite_base_criterium() = default;

	/// obtain the crit_base_criterium object for libdar option
    virtual std::unique_ptr<libdar::criterium> get_overwriting_criterium() const override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// inherited from body_builder
    MAKE_BROTHER_MACRO;

protected:

	/// inherited from body_builder from html_overwrite_criterium
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    html_form_fieldset crit_fs; ///< wrapps all html form components of this class
    html_form_input negate; ///< checkbox
    html_form_input invert; ///< checkbox
    html_form_select base;  ///< selection of the base criterium
    html_datetime date; ///< for date related criterium
    html_form_input hourshift; ///< for date related criterium

};

#endif
