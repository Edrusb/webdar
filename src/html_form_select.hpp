/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2023 Denis Corbin
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

#ifndef HTML_FORM_SELECT_HPP
#define HTML_FORM_SELECT_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_form_radio.hpp"

class html_form_select : public html_form_radio, public actor
{
public:
    static const std::string changed;  /// common event name for all object of this class

    html_form_select(const std::string & label) { x_label = label; register_name(changed); record_actor_on_event(this, html_form_radio::changed); };
    html_form_select(const html_form_select & ref) = default;
    html_form_select(html_form_select && ref) noexcept = default;
    html_form_select & operator = (const html_form_select & ref) = default;
    html_form_select & operator = (html_form_select && ref) noexcept = default;
    ~html_form_select() = default;

    void change_label(const std::string & label) { x_label = label; };

	// inherit from html_form_radio the following methods to feed contents
	// . add_choice
	// . clear
	// . set_selected
	// . get_selected_id
	// . get_selected_num


	/// inherited from actor
    virtual void on_event(const std::string & event_name) override { act(changed); };

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;


private:
    std::string x_label;

};



#endif
