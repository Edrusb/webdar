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

#ifndef HTML_MENU_HPP
#define HTML_MENU_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <map>

    // webdar headers
#include "body_builder.hpp"
#include "html_url.hpp"
#include "html_div.hpp"
#include "html_url_class.hpp"
#include "events.hpp"


    /// class html_menu provides a menu of choices to dynamically
    /// change another part of the html user interface
    /// upon change the event "changed" is generated

class html_menu : public body_builder, protected events
{
public:
	/// the available event for this class
    static const std::string changed;

	/// constructor
    html_menu();
    html_menu(const html_menu & ref) = default;
    html_menu(html_menu && ref) noexcept = default;
    html_menu & operator = (const html_menu & ref) = default;
    html_menu & operator = (html_menu && ref) noexcept = default;

	/// destructor
    ~html_menu();

	/// add an entry in the menu
	///
	/// \param[in] reference must be a single word, letters only,
	/// lower case and different from any other reference already give so far
	/// \param[in] label is any text that will be showed to the user
    void add_entry(const std::string & reference, const std::string & label);

	/// returns the reference of the current mode
	///
	/// \return the selected item in the order of addition using add_entry
	/// starting from 0: First add_entry gives item number 0, second gives 1, etc.
    unsigned int get_current_mode() const { return current_mode; };

	/// set html field accordingly to the given mode
	/// \param[in] mode is the mode to set, it start from 0
    void set_current_mode(unsigned int mode);

	/// returns the reference of the previous mode (the mode that was selected before current mode)
    unsigned int get_previous_mode() const { return previous_mode; };

	/// return the current selected message
    std::string get_current_label() const;

	/// set the current selected mode
    void set_current_label(const std::string & label);

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req) override;

	/// modified wrapper for inherited methods from events
    void record_actor_on_event(actor *ptr) { events::record_actor_on_event(ptr, changed); };


protected:
	/// inherited from body_builder
    virtual void path_has_changed() override;

	/// inherited from css (grand-parent class)
    virtual void css_updated(bool inherit) override;

private:
    struct boite
    {
	std::string value;
	html_div surround;
	html_url inside;
	boite(const std::string & url, const std::string & label): inside(url, label) { value = ""; };
    };

    unsigned int current_mode; // which item is currently selected
    unsigned int previous_mode; // which item was previously selected

    html_div global;           // first level object containing all others
    std::vector<boite *> item; // items for choices

    html_div box_off; //< used to assign CSS attributes: unselected item
    html_div box_on;  //< used to assign CSS attributes: selected item
    html_div box_void;//< used to assign CSS attributes: separators
    html_url_class url_selected; //< links when box selected
    html_url_class url_normal;   //< links when box no selected

};

#endif
