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

#ifndef HTML_DEMO_HPP
#define HTML_DEMO_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "body_builder.hpp"
#include "actor.hpp"
#include "html_text.hpp"
#include "html_table.hpp"
#include "html_form_input.hpp"
#include "html_form_radio.hpp"
#include "html_form_fieldset.hpp"
#include "html_form.hpp"
#include "html_double_button.hpp"
#include "html_div.hpp"
#include "html_popup.hpp"

    /// class html_demo show how webdar interacts with the browser

    /** \verbatim
	+------------------------------------------------+
	| text explaing the left and right fields nature |
	+------------------------------------------------+
	+----- table -----------+------------------------+
	|+--- form ------------+| +-- right_fs ---------+|
	||+-- left_fs --------+|| |right_input          ||
	||| left_input        ||| |right_radio          ||
	||| left_radio        ||| +---------------------+|
	||+-------------------+||                        |
	|+-[update btn]--------+|                        |
	+-----------------------+------------------------+
	|+-- btn_div ----------+|                        |
	||+--------++---------+||                        |
	|||btn_incr||btn_clear||| counter                |
	||+--------++---------+||                        |
	|+---------------------+|                        |
	+-----------------------+------------------------+
    \endverbatim **/

class html_demo : public html_popup, public actor
{
public:
    html_demo();
    html_demo(const html_demo & ref) = delete;
    html_demo(html_demo && ref) noexcept = delete;
    html_demo & operator = (const html_demo & ref) = delete;
    html_demo & operator = (html_demo && ref) noexcept = delete;
    ~html_demo() = default;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:

	/// inherited from body_builder
    virtual void new_css_library_available() override;


private:
    static constexpr const char* event_incr = "increment";
    static constexpr const char* event_clear = "clear";
    static constexpr const char* event_close = "close";

    static constexpr const char* css_cells = "html_demo_cells";
    static constexpr const char* css_table = "html_demo_table";
    static constexpr const char* css_btn_close = "html_demo_button_close";


    html_div desc_div;
    html_text description;
    html_table table;
    html_form_input left_input;
    html_form_radio left_radio;
    html_form_fieldset left_fs;
    html_form form;
    html_double_button btn_incr;
    html_double_button btn_clear;
    html_div btn_div;
    html_form_input right_input;
    html_form_radio right_radio;
    html_form_fieldset right_fs;
    html_form_input counter;
    html_button close;
};

#endif
