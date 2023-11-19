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

#ifndef HTML_FORM_INPUT_FILE_HPP
#define HTML_FORM_INPUT_FILE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <memory>

    // webdar headers
#include "html_div.hpp"
#include "events.hpp"
#include "html_form_input.hpp"
#include "html_button.hpp"
#include "html_select_file.hpp"
#include "html_text.hpp"

    /// class html_form_input_file provides a mean to the user to navigate to filesystem to select a file or directory

    /// this is an html_form_input with an added button
    /// that triggers a popup for the user to select a file
    /// or a directory in a given libdar::entrepot (or by
    /// default an libdar::entrepot_local objeect).
    /// user can also manually modify the entry in the <INPUT>
    /// as a normal html_form_input object adopted by an
    /// html_form

class html_form_input_file: public html_div, public events, public actor
{
public:

    static const std::string changed_event; ///< name of the change event unless changed

    html_form_input_file(const std::string & label,
			 const std::string & initial_value,
			 unsigned int size,
			 const std::string & popup_message);

    html_form_input_file(const html_form_input_file & ref) = default;
    html_form_input_file(html_form_input_file && ref) noexcept = default;
    html_form_input_file & operator = (const html_form_input_file & ref) = default;
    html_form_input_file & operator = (html_form_input_file && ref) noexcept = default;
    ~html_form_input_file() = default;

    	// relayed from html_form_input

    void change_label(const std::string & label) { input.change_label(label); };
    const std::string & get_value() const { return input.get_value(); };
    void set_value(const std::string & val) { input.set_value(val); };

	/// change the name of the "changed" event
    void set_change_event_name(const std::string & name);

	// relayed from html_select_file

    void set_select_dir(bool val) { user_select.set_select_dir(val); };
    void set_can_create_dir(bool val) { user_select.set_can_create_dir(val); };

	/// change the entrepot to search into (by default this is local filesystem
    void set_entrepot(const std::shared_ptr<libdar::entrepot> & entrepot);

	// inherited from events

    virtual void on_event(const std::string & event_name) override;

protected:

	// inherited from body_builder
    virtual void new_css_library_available() override;
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req);


private:
	// adoption tree:
	//
	//  html_div (this)
        //   |
        //   +--- input_div
	//   | |
	//   | +- input
	//   +--- trigger
        //   +--- user_select
	//   +--- empty_text
	//

    html_form_input input;    ///< input field editable and/or filled by the popup window
    html_div input_div;       ///< wrapping the html_form_input for CSS class assignment
    html_button trigger;      ///< the button that triggers the popup window
    html_select_file user_select;  ///< the popup window letting the user select a file or dir
    html_text empty_text;    ///< to end the float:left CSS property applied to input and trigger

    std::string changed_event_name;
    std::shared_ptr<libdar::entrepot> entrep;
    bool refresh_get_body;

    static const std::string triggered_event;

    static const std::string css_input;
    static const std::string css_button_box;
    static const std::string css_button_link;
    static const std::string css_empty_text;

};


#endif
