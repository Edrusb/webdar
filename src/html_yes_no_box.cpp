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

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_form_fieldset.hpp"

    //
#include "html_yes_no_box.hpp"


using namespace std;

html_yes_no_box::html_yes_no_box(const string & message, bool default_value)
    : form("Confirm"),
      form_fs(message)
{
    rd.add_choice("no", "No");
    rd.add_choice("yes", "Yes");
    rd.set_selected_num(default_value ? 1 : 0);

	/// building the body_builder tree
    form_fs.adopt(&rd);
    form.adopt(&form_fs);
    adopt(&form);
}
