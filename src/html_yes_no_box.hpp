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

#ifndef HTML_YES_NO_BOX_HPP
#define HTML_YES_NO_BOX_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "exceptions.hpp"
#include "body_builder.hpp"
#include "events.hpp"
#include "actor.hpp"
#include "html_form.hpp"
#include "html_form_radio.hpp"
#include "html_form_fieldset.hpp"
#include "html_popup.hpp"
#include "html_text.hpp"

    /// html component for user to answer by yes or no to a provided question

class html_yes_no_box : public html_popup, public events, public actor
{
public:
    static const std::string answer_yes;
    static const std::string answer_no;

    html_yes_no_box();
    html_yes_no_box(const html_yes_no_box & ref) = delete;
    html_yes_no_box(html_yes_no_box && ref) noexcept = delete;
    html_yes_no_box & operator = (const html_yes_no_box & ref) = delete;
    html_yes_no_box & operator = (html_yes_no_box && ref) noexcept = delete;
    ~html_yes_no_box() = default;

	/// make the question to appear to the user
    void ask_question(const std::string & message, bool default_value);

	/// read the answer returned by the user

	/// \note the use of answer_yes and answer_no events
	/// is more flexible than reading the value (no pooling)
    bool get_value() const { return rd.get_selected_num() != 0; };

    void set_value(bool val) { rd.set_selected_num(val ? 1 : 0); };

	/// inherited from actor parent class
    virtual void on_event(const std::string & event_name) override;

protected:

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    static constexpr const char* css_center = "html_ynb_center";

    bool ignore_events;

    html_form form;
    html_form_fieldset form_fs;
    html_form_radio rd;
    html_text question;

};

#endif
