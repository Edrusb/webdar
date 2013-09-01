/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013 Denis Corbin
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

#ifndef HTML_OPTIONS_COMPARE_HPP
#define HTML_OPTIONS_COMPARE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"
#include "html_form_input.hpp"
#include "html_form_fieldset.hpp"
#include "html_comparison_fields.hpp"
#include "html_form.hpp"

class html_options_compare : public body_builder
{
public:
    html_options_compare();

    libdar::archive_options_diff get_options() const;

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req);


private:
    html_form form;
    html_form_fieldset fs;
    html_form_input info_details;
    html_comparison_fields what_to_check;
    html_form_input alter_atime;
    html_form_input furtive_read_mode;
    html_form_input display_skipped;
    html_form_input hourshift;
    html_form_input compare_symlink_date;

};


#endif
