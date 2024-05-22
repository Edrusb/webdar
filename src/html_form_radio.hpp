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

    /// class html_form_radio implements HTML "input" of type "radio"

    /// these html_form_field objects are set calling add_choice(), clear()
    /// set_selected() methods and provide in return the menu selected by
    /// the user using get_selected_id() or get_selected_num()
    /// These object provide an event "changed" that is triggered when user
    /// change the selection of the radio button set. This change is only
    /// visible to the application once the user validated the change by clicking
    /// the button of the parent or grand-parent html_form.

class html_form_radio : public body_builder, public events
{
public:
    static const std::string changed; /// common event name for all object of this class

    html_form_radio();
    html_form_radio(const html_form_radio & ref) = default;
    html_form_radio(html_form_radio && ref) noexcept = default;
    html_form_radio & operator = (const html_form_radio & ref) = default;
    html_form_radio & operator = (html_form_radio && ref) noexcept = default;
    ~html_form_radio() = default;

    void add_choice(const std::string & id, const std::string & label);
    void clear() { choices.clear(); selected = 0; my_body_part_has_changed(); };

    void set_selected(unsigned int x); /// \note index zero is the first choice
    const std::string & get_selected_id() const { return choices[selected].id; };
    unsigned int get_selected_num() const { return selected; };


protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// used to record parameters of each option of the radio button
    struct record
    {
	std::string id;
	std::string label;
    };

    const std::vector<record> & get_choices() const { return choices; };
    void update_field_from_request(const request & req);

	/// unlock update_field_from_request()

	/// \note when a set_select() call is invoked to avoid
	/// a POST request to overwrite at the time of inherited_get_body_part(),
	/// update_field_from_request() is ignored and the POST
	/// request is not taken into account, until an call to
	/// unlock_update_field_from_request() is done. This should
	/// be done at the end of inherited_get_body_part()
    void unlock_update_field_from_request();

private:
    std::vector<record> choices;
    unsigned int selected;
    bool value_set; ///< avoid POST request to overwrite set_seleected*()
};


#endif
