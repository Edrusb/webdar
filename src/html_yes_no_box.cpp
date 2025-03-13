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

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_form_fieldset.hpp"

    //
#include "html_yes_no_box.hpp"

using namespace std;


const string html_yes_no_box::answer_yes = "html_yes_no_box_y";
const string html_yes_no_box::answer_no = "html_yes_no_box_n";

html_yes_no_box::html_yes_no_box():
    html_popup(60, 60), // parent class
    ignore_events(false),
    form("Confirm"),
    form_fs("&lt;lack of initialization!&gt;")
{
    rd.add_choice("undef", "--- make a choice ---");
    rd.add_choice("no", "No");
    rd.add_choice("yes", "Yes");

	// building the body_builder tree
    form_fs.adopt(&rd);
    form.adopt(&form_fs);
    adopt(&form);

	// events
    rd.record_actor_on_event(this, html_form_radio::changed);
    register_name(answer_yes);
    register_name(answer_no);

	// visibility
    set_visible(false);
}

void html_yes_no_box::ask_question(const string & message, bool default_value)
{
    form_fs.change_label(message);
    ignore_events = true;
    try
    {
	rd.set_emphase(default_value ? 2 : 1);
    }
    catch(...)
    {
	ignore_events = false;
	throw;
    }
    ignore_events = false;

    set_visible(true);
}

void html_yes_no_box::on_event(const std::string & event_name)
{
    if(ignore_events)
	return;

    if(rd.get_selected_num() == 2)
	act(answer_yes);
    else
	act(answer_no);
    set_visible(false);
}
