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

#ifndef HTML_FORM_OVERWRITE_CONSTANT_ACTION_HPP
#define HTML_FORM_OVERWRITE_CONSTANT_ACTION_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_overwrite_action.hpp"
#include "body_builder.hpp"
#include "html_form_select.hpp"
#include "html_form_fieldset.hpp"


    /// html component to be adopted by an html_form that implements constant actions for overwriting policies

class html_form_overwrite_constant_action: public html_overwrite_action
{
public:
    html_form_overwrite_constant_action();
    html_form_overwrite_constant_action(const html_form_overwrite_constant_action & ref) = default;
    html_form_overwrite_constant_action(html_form_overwrite_constant_action && ref) noexcept = default;
    html_form_overwrite_constant_action & operator = (const html_form_overwrite_constant_action & ref) = default;
    html_form_overwrite_constant_action & operator = (html_form_overwrite_constant_action && ref) noexcept = default;
    ~html_form_overwrite_constant_action() = default;

	/// obtain the crit_constant_action object for libdar option
    virtual std::unique_ptr<libdar::crit_action> get_overwriting_action() const override;

protected:

	/// inherited from body_builder from html_overwrite_action
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    html_form_fieldset action_fs;
    html_form_select data_action;
    html_form_select ea_action;

};

#endif
