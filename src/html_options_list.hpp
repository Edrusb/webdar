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

#ifndef HTML_OPTIONS_LIST_HPP
#define HTML_OPTIONS_LIST_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "html_form.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_radio.hpp"
#include "html_derouleur.hpp"

    /// html component for the user to provide parameters of libdar archive testing operation

class html_options_list : public body_builder
{
public:
    html_options_list();
    html_options_list(const html_options_list & ref) = delete;
    html_options_list(html_options_list && ref) noexcept = delete;
    html_options_list & operator = (const html_options_list & ref) = delete;
    html_options_list & operator = (html_options_list && ref) noexcept = delete;
    ~html_options_list() = default;

    bool do_we_list() const { return action.get_selected_num() == 0; };

protected:

	// inherited from bdy_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	// inherited from bdy_builder
    virtual void new_css_library_available() override;

private:
    html_derouleur deroule;
    html_form form;
    html_form_fieldset fs;
    html_form_radio action;
};

#endif
