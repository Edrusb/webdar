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

#ifndef HTML_FORM_RADIO_HPP
#define HTML_FORM_RADIO_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <vector>

    // webdar headers
#include "body_builder.hpp"
#include "events.hpp"
#include "html_label.hpp"

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
    static const std::string changed; /// default event name for all object of this class

    html_form_radio();
    html_form_radio(const html_form_radio & ref) = default;
    html_form_radio(html_form_radio && ref) noexcept = delete;
    html_form_radio & operator = (const html_form_radio & ref) = default;
    html_form_radio & operator = (html_form_radio && ref) noexcept = delete;
    ~html_form_radio() = default;

    void add_choice(const std::string & id, const std::string & label);
    void clear() { choices.clear(); unset_selected(); emphase = -1; my_body_part_has_changed(); };

	/// set the radio buttons to item given in argument

	/// \note index zero is the first choice
    void set_selected_num(unsigned int x);

	/// set the radio buttons to the item id given in argument

	/// \note set_selected() and set_selected_id() do the same
	/// thing but using different way to design the radio button
	/// to select
	/// \note if provided id is not valid a bug is reported
    void set_selected_id(const std::string & id);

	/// same as set_selected_id but issue a friendly warning build with jlabel for user understanding
    void set_selected_id_with_warning(const std::string & id, const std::string & jlabel);

	/// unselect all radio buttons
    void unset_selected();

	/// returns whether a radio button is selected
    bool is_selected() const { return selected < choices.size(); };

	/// obtain the index (starts at zero) of the selected radio button

	/// \note if nothing is selected the index passed the last valid
	/// index is returned
    unsigned int get_selected_num() const { return selected; };

	/// obtain the id of the selected radio button

	/// \note if no item is selected exception is thrown (BUG).
	/// first check with is_selected() before calling this method
    const std::string & get_selected_id() const;

	/// index of the item to display in bold
    void set_emphase(unsigned int num);

	/// disable emphasing a particular item (default behavior)
    void set_emphase() { emphase = -1; my_body_part_has_changed(); };

	/// the number of currently available options
    const unsigned int num_choices() const { return choices.size(); };

	/// change the default value for the "changed" event
    void set_change_event_name(const std::string & name);

	/// set whether the HTML control is enable or disabled
    void set_enabled(bool val);

	/// set tooltip for item at the given index (index starts at zero)
	/// \note index must stay less than the value returned by num_choices()
    void set_tooltip(unsigned int index, const std::string & msg);

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void css_classes_have_changed() override;


	/// inherited from html_div/body_builder
    virtual void new_css_library_available() override;


	/// used to record parameters of each option of the radio button
    struct record
    {
	std::string id;
	html_label label;

	record(const std::string x_id,
	       const std::string & x_label): id(x_id), label(x_label) {};
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
    bool enabled;              ///< whether the control is enabled or disabled
    std::vector<record> choices;
    unsigned int selected;
    bool value_set;            ///< avoid POST request to overwrite set_seleected*()
    int emphase;               ///< item displayed in bold, unless emphase < 0

    std::string modif_changed; ///< name of the event "changed" to use, empty string for the default
    void my_act();
};


#endif
