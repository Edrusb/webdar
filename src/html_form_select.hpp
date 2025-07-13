/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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
#include "my_config.h"
extern "C"
{

}

    // C++ system header files

    /// class html_form_select implements HTML input of type "select"

    /// It should be adopted directly or not by an html_form
    /// object. It shows a dropdown list of choices the user
    /// has to select. To setup the available choices use the
    /// add_choice(), clear() methods. To obtain the value selected
    /// by the user, use the get_selected_id() or get_selected_num()
    /// methods. Note that the change only occurs when the form is
    /// submited by the browser. As result of this POST action
    /// if the user changed the field, the object triggers the 'changed'
    /// event other objects can register on to be notified of change

    // webdar headers
#include "html_form_radio.hpp"
#include "html_label.hpp"

class html_form_select : public html_form_radio, public actor
{
public:
    static const std::string changed;  /// default event name for all object of this class

	/// constructor

	/// \param[in] label is what name shows to the user
	/// \param[in] x_event_name is the event to register upon change on this object
	/// \note if x_event_name is not given or is an empty string the even to register is
	/// given by the value of html_form_select::changed
    html_form_select(const std::string & label, const std::string & x_event_name = "");
    html_form_select(const html_form_select & ref);
    html_form_select(html_form_select && ref) noexcept = delete;
    html_form_select & operator = (const html_form_select & ref) = delete;
    html_form_select & operator = (html_form_select && ref) noexcept = delete;
    ~html_form_select() = default;

    void change_label(const std::string & label) { x_label.set_label(label); my_body_part_has_changed(); };

	// inherit from html_form_radio the following methods to feed contents
	// . add_choice
	// . clear
	// . set_selected
	// . get_selected_id
	// . get_selected_num


	/// inherited from actor
    virtual void on_event(const std::string & x_event_name) override { act(event_name); };

	/// number of available choices provided to the web user by this object
    unsigned int size() const { return get_choices().size(); };

	/// provides recorded item id

	/// \note index should be equal or greater than zero and strictly less than size()
    const std::string & get_id(unsigned int index) const { return get_choices()[index].id; };

	/// provides recorded item label

	/// \note index should be equal or greater than zero and strictly less than size()
    const std::string & get_label(unsigned int index) const { return get_choices()[index].label.get_label(); };

	/// set whether the HTML control is enable or disabled
    void set_enabled(bool val);

	/// set tooltip for the html label of the input form
    void set_tooltip(const std::string & msg) { x_label.set_tooltip(msg); };

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;


private:
    bool enabled;        ///< whether the control is enabled or disabled
    html_label x_label;
    std::string event_name;

};



#endif
