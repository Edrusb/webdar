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

#ifndef HTML_SELECT_FILE_HPP
#define HTML_SELECT_FILE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <map>
#include <dar/libdar.hpp>
#include <memory>

    // webdar headers
#include "events.hpp"
#include "actor.hpp"
#include "html_div.hpp"
#include "html_text.hpp"
#include "html_form_fieldset.hpp"
#include "html_button.hpp"
#include "html_table.hpp"
#include "html_popup.hpp"
#include "html_form.hpp"
#include "html_form_input.hpp"

    /// \file html_select_file.hpp defines html_select_file class
    ///
    /// given a libdar::entrepot object, the class html_select_file
    /// shows a popup for the user to navigate in the directory tree
    /// and select a file or a directory depending on the parameter set
    /// to this object
    ///
    /// usage for the caller is to:
    ///- register for entry_selected and op_cancelled events
    /// on this object, events which will be triggered once the user will
    /// have selected a file or cancelled choice selection
    ///- call the different set_* methods
    ///- then call run() giving a libdar::entrepot
    ///- and upon event entry_selected fetch the path selected by the user
    ///  calling get_selected_path()

class html_select_file: public html_popup, public events, public actor
{
public:
	/// event when user selected a file
    static const std::string entry_selected;

	/// event when the user cancelled file selection
    static const std::string op_cancelled;

	/// constructor

	/// \param[in] message is show to the user and should tell what it is expected to select for
    html_select_file(const std::string & message);

    html_select_file(const html_select_file & ref) = delete;
    html_select_file(html_select_file && ref) noexcept(false) = delete;
    html_select_file & operator = (const html_select_file & ref) = delete;
    html_select_file & operator = (html_select_file && ref) noexcept(false) = delete;
    virtual ~html_select_file() { clear_content(); };

	/// ask the user to select a file path (false) or a directory path (true)

	/// \note should be setup before calling run()
    void set_select_dir(bool val) { if(status != st_init) throw WEBDAR_BUG; select_dir = val; };

	/// whether to show the button allowing the user to create a subdirectory

	/// \note should be setup before calling run()
    void set_can_create_dir(bool val) { if(status != st_init) throw WEBDAR_BUG; btn_createdir.set_visible(val); };

	/// only show directories and files matching the given glob expression (shell wildcards)
	/// \note giving an empty string disable filtering, all entries are shown
	/// which is the default behavior
    void set_filter(const std::string & mask) { filter = mask; };

	/// start the user interaction for a path selection

	/// \param[in] x_entr entrepot to scan directory from
	/// \param[in] absolute path in x_entr to take as starting directory
	/// \note if start_dir is not a directory not is an absolute path
	/// an exception is thrown
    void run(const std::shared_ptr<libdar::entrepot> & x_entr,
	     const std::string & start_dir);

	/// obtain the path selected by the user (mandatory after entry_selected event, in order to reuse this object)
    std::string get_selected_path() const { if(status != st_completed) throw WEBDAR_BUG; status = st_init; return fieldset.get_label(); };

	/// action triggered from html_buttons internally used in this class implementation (do not use, even if it must stay a public method)
    virtual void on_event(const std::string & event_name) override;

protected:

	/// inherited from body_builder (via html_div)
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder (via html_div)
    virtual void new_css_library_available() override;

private:
	// event nam for the 'change to parent directory' button (parentdir)
    static const std::string op_chdir_parent;
    static const std::string op_createdir;

	// internal css class names
    static const std::string css_float_button_right;
    static const std::string css_float_button_left;
    static const std::string css_warning;
    static const std::string css_sticky_top;
    static const std::string css_sticky_bot;

	// popup size of browser windows
    static constexpr const unsigned int width_pct = 80;
    static constexpr const unsigned int height_pct = 80;


	// internal datastructure

    struct item
    {
	html_button* btn;
	bool isdir;
	item() { btn = nullptr; isdir = false; };
	item(html_button* ptr) { btn = ptr; isdir = false; };
    };


	// object status

    mutable enum { st_init, st_run, st_completed } status;


	// settings

    bool select_dir;
    std::string filter;
    std::shared_ptr<libdar::entrepot> entr;


	// html components

    html_text title;                  ///< shows the message request for the user (provided at constructor time)
    html_text warning;                ///< shows warnings when needed (permission pb, etc.)
    html_div title_box;               ///< contains title and warning and stay visible as a sticky box
    html_form_fieldset fieldset;      ///< shows the current directory path
    html_button parentdir;            ///< change to parent dir
    html_table content;               ///< parent of content objects
    std::map<std::string, item> listed; ///< associate a event message to each listed items
    html_div btn_box;                 ///< box containing the bottom buttons
    html_button btn_cancel;           ///< triggers the entry_cancelled event
    html_button btn_validate;         ///< trigger the entry_selected event
    html_button btn_createdir;        ///< leads to create a new directory
    html_form createdir_form;         ///< form for new directory
    html_form_input createdir_input;  ///< field to let user provide new dir name


	// status field about html components

    bool ignore_events;               ///< in inherited_get_body_part() we trigger twice the adopted objects but must take action on events only once
    bool fieldset_isdir;              ///< whether fieldset points to a directory or not


	// internal routines

	/// fills content and listed from entry located in fieldset.get_label() directory of the entr libdar::entrepot
    void fill_content();

	/// free up and clean all html_button of listed and clear content too in coherence
    void clear_content();
};

#endif
