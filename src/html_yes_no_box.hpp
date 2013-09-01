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

#ifndef HTML_YES_NO_BOX_HPP
#define HTML_YES_NO_BOX_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "exceptions.hpp"
#include "body_builder.hpp"
#include "html_form.hpp"
#include "html_form_radio.hpp"
#include "html_form_fieldset.hpp"

class html_yes_no_box : public body_builder
{
public:
    html_yes_no_box(const std::string & message, bool default_value);

    bool get_value() const { return rd.get_selected_num() != 0; };

    void set_value(bool val) { rd.set_selected(val ? 1 : 0); };

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req)
    { return get_body_part_from_all_children(path, req); };

protected:
	/// inherited from ancestor class css
    void css_updated(bool inherited);

private:
    html_form form;
    html_form_fieldset form_fs;
    html_form_radio rd;

};

#endif
