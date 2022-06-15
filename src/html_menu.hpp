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
#include "html_button.hpp"
#include "html_div.hpp"
#include "events.hpp"


    /// class html_menu provides a menu of choices to dynamically
    /// change another part of the html user interface
    /// upon change the event "changed" is generated

class html_menu : public html_div, protected events, protected actor
{
public:
	/// the available event for this class
    static const std::string changed;

	/// constructor
    html_menu();
    html_menu(const html_menu & ref) = delete;
    html_menu(html_menu && ref) noexcept = delete;
    html_menu & operator = (const html_menu & ref) = delete;
    html_menu & operator = (html_menu && ref) noexcept = delete;

	/// destructor
    ~html_menu();

	/// add an entry in the menu
	///
	/// \param[in] label is any text that will be showed to the user
	/// \note first added entry will get the index (or mode) zero, next
	/// added entry will have index 1, adding an empty string makes a non
	/// selectable space between buttons.
    void add_entry(const std::string & label);

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

	/// modified wrapper from class events for our inherited classes
    void record_actor_on_event(actor *ptr) { events::record_actor_on_event(ptr, changed); };

	/// inherited from class actor
    virtual void on_event(const std::string & event_name);

protected:

	/// inherited from body_builder, used to defines the css_class/css_selectors used by this class
    virtual void new_css_library_available() override;

private:

    unsigned int current_mode;  ///< which item is currently selected
    unsigned int previous_mode; ///< which item was previously selected
    std::vector<html_button *> item;  ///< items for choices

    static const std::string box_off_class;
    static const std::string box_on_class;
    static const std::string box_void_class;
    static const std::string url_selected_class;
    static const std::string url_normal_class;
};

#endif
