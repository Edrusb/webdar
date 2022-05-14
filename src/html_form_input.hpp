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

#ifndef HTML_FORM_INPUT_HPP
#define HTML_FORM_INPUT_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <vector>

    // webdar headers
#include "body_builder.hpp"
#include "events.hpp"

class html_form_input : public body_builder, public events
{
public:
    static const std::string changed;

    enum input_type { text, password, number, range, check };

    html_form_input(const std::string & label,
		    input_type type,
		    const std::string & initial_value, //< for check set empty string for unchecked, anything else for checked
		    unsigned int size);

    void set_range(int min, int max);
    void change_label(const std::string & label) { x_label = label; };
    void change_type(input_type type) { x_type = string_for_type(type); };

    const std::string & get_value() const { return x_init; };
    void set_value(const std::string & val) { x_init = val; act(changed); };
    const bool get_value_as_bool() const { return !x_init.empty(); }; //< for checkbox empty string means unchecked, anything else checked
    void set_value_as_bool(bool val) { x_init = val ? "x" : ""; act(changed); };

	/// whether the HTML control is enable or disabled
    void set_enabled(bool val) { enabled = val; };

    virtual std::string get_body_part(const chemin & path,
				      const request & req);

	/// change the name of the event generated when this control changes
    void set_change_event_name(const std::string & name);

private:
    bool enabled;        //< whether the control is enabled or disabled
    std::string x_label; //< field text shown to the user
    std::string x_type;  //< type of HTML input field
    std::string x_init;  //< current value / initial value of the field
    std::string x_size;  //< width of the field in number of characterds
    std::string x_min;   //< minimum value of the field (for type = number)
    std::string x_max;   //< maximum value of the field (for type = number)

    std::string modif_change; //< name of the event "changed" to use, if set to an empty string default behavior is used

    static std::string string_for_type(input_type type);
};

#endif
