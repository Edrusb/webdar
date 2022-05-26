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

#ifndef HTML_FORM_RADIO_HPP
#define HTML_FORM_RADIO_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <vector>

    // webdar headers
#include "body_builder.hpp"
#include "events.hpp"

class html_form_radio : public body_builder, public events
{
public:
    static const std::string changed;

    html_form_radio() { selected = 0; register_name(changed); };
    html_form_radio(const html_form_radio & ref) = default;
    html_form_radio(html_form_radio && ref) noexcept = default;
    html_form_radio & operator = (const html_form_radio & ref) = default;
    html_form_radio & operator = (html_form_radio && ref) noexcept = default;
    ~html_form_radio() = default;

    void add_choice(const std::string & id, const std::string & label);
    void clear() { choices.clear(); selected = 0; };

    void set_selected(unsigned int x);
    const std::string & get_selected_id() const { return choices[selected].id; };
    unsigned int get_selected_num() const { return selected; };

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req) override;

protected:
    struct record
    {
	std::string id;
	std::string label;
    };

    const std::vector<record> & get_choices() const { return choices; };
    void update_field_from_request(const request & req);

private:
    std::vector<record> choices;
    unsigned int selected;
};


#endif
