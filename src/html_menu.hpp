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

#ifndef HTML_MENU_HPP
#define HTML_MENU_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <map>
#include <deque>

    // webdar headers
#include "body_builder.hpp"
#include "html_button.hpp"
#include "html_div.hpp"
#include "events.hpp"

    /// class html_menu shows a menu and provides events to act on other objects

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
	/// \param[in] tag is to programmatically change the selected item (not translatable as the label)
	/// \note first added entry will get the index (or mode) zero, next
	/// added entry will have index 1, adding an empty string makes a non
	/// selectable space between buttons.
    void add_entry(const std::string & label, const std::string & tag);

	/// returns the reference of the current mode
	///
	/// \return the selected item in the order of addition using add_entry
	/// starting from 0: First add_entry gives item number 0, second gives 1, etc.
    unsigned int get_current_mode() const { return current_mode; };

	/// set html field accordingly to the given mode
	/// \param[in] mode is the mode to set, it start from 0
	/// \param[in] force is set to true, will never take into consideration the current mode and avoid changing if the new mode is the same
    void set_current_mode(unsigned int mode, bool force = false);

	/// returns the reference of the previous mode (the mode that was selected before current mode)
    unsigned int get_previous_mode() const { return previous_mode; };

	/// return the tage of the currently selected item
    std::string get_current_tag() const;

	/// set the current selected mode giving its tag
    void set_current_tag(const std::string & tag);

	/// get previous tag
    std::string get_previous_tag() const;

	/// get current label (what is shown to the user)
    std::string get_current_label() const;

	/// modified wrapper from class events for our inherited classes
    void record_actor_on_event(actor *ptr) { events::record_actor_on_event(ptr, changed); };

	/// inherited from class actor
    virtual void on_event(const std::string & event_name) override;

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder, used to defines the css_class/css_selectors used by this class
    virtual void new_css_library_available() override;

private:

	/// struct used to store the html components of the html_menu
    struct cell
    {
	std::string itag;  ///< used in implementation
	html_button* ibtn; ///< object containing the visible (translatable) name of the item

	cell(const std::string & label, const std::string & event_name, const std::string & tag)
	{
	    ibtn = new (std::nothrow) html_button(label, event_name);
	    if(ibtn == nullptr)
		throw exception_memory();
	    itag = tag;
	};
	cell(const cell &) = delete; // not implemented (not needed)
	cell(cell && ref) noexcept { ibtn = nullptr; std::swap(ref.ibtn, ibtn); std::swap(ref.itag, itag); };
	cell & operator = (const cell &) = delete; // not implemented (not needed)
	cell & operator = (cell && ref) noexcept { std::swap(ref.ibtn, ibtn); std::swap(ref.itag, itag); return *this; };
	~cell()
	{
	    if(ibtn != nullptr)
	    {
		delete ibtn;
		ibtn = nullptr;
	    }
	};
    };

    unsigned int current_mode;  ///< which item is currently selected
    unsigned int previous_mode; ///< which item was previously selected
    std::deque<cell> item;      ///< items for choices

    static const std::string box_off_class;
    static const std::string box_on_class;
    static const std::string box_void_class;
    static const std::string url_selected_class;
    static const std::string url_normal_class;
};

#endif
