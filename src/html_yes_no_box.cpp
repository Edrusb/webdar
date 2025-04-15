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
    html_popup(50, 30), // parent class
    ignore_events(false),
    form("Confirm"),
    form_fs("")
{
    rd.add_choice("no", "No");
    rd.add_choice("yes", "Yes");

	// building the body_builder tree
    adopt(&question);
    form_fs.adopt(&rd);
    form.adopt(&form_fs);
    adopt(&form);

	// events
    rd.record_actor_on_event(this, html_form_radio::changed);
    register_name(answer_yes);
    register_name(answer_no);

	// visibility
    set_visible(false);

	// css
    question.add_css_class(css_center);
    form.add_css_class(css_center);
}

void html_yes_no_box::ask_question(const string & message, bool default_value)
{
    if(get_visible())
	return;
	// ignoring subsequent request, as this can be
	// the consequence of the changes done during the
	// first invocation

    ignore_events = true;
    try
    {
	question.clear();
	question.add_text(4, message);
	rd.set_emphase(default_value ? 1 : 0);
	rd.unset_selected();
    }
    catch(...)
    {
	ignore_events = false;
	throw;
    }
    ignore_events = false;

    set_visible(true);
}

void html_yes_no_box::on_event(const string & event_name)
{
    if(ignore_events)
	return;

    try
    {
	if(rd.get_selected_num() == 1)
	    act(answer_yes);
	else
	    act(answer_no);
    }
    catch(...)
    {
	set_visible(false);
	throw;
    }
    set_visible(false);
}

void html_yes_no_box::new_css_library_available()
{
    css tmp;
    unique_ptr<css_library> & csslib = lookup_css_library();

    if(!csslib)
	throw WEBDAR_BUG;

    if(!csslib->class_exists(css_center))
    {
	tmp.clear();
	tmp.css_width("80%", true);
	tmp.css_text_h_align(css::al_center);
	csslib->add(css_center, tmp);
    }

	// we also need css class definitions from the parent class
    html_popup::new_css_library_available();
}
