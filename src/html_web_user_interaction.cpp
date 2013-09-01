/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013 Denis Corbin
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
extern "C"
{

}

    // C++ system header files


    // webdar headers



    //
#include "html_web_user_interaction.hpp"


using namespace std;

html_web_user_interaction::html_web_user_interaction(unsigned int x_warn_size):
    h_inter(""),
    h_get_string("", html_form_input::text, "", 20),
    h_form("Update"),
    h_logs("Last logs"),
    h_global("Interaction with libdar"),
    lib_data(x_warn_size)
{
    h_pause2.add_choice("undefined", "please answer yes or no");
    h_pause2.add_choice("no", "No");
    h_pause2.add_choice("yes", "Yes");

    h_inter_text.css_font_weight_bold();
    h_inter_text.css_color("blue");

    h_inter.adopt(&h_inter_text);
    h_inter.adopt(&h_pause2);
    h_form.adopt(&h_inter);
    h_form.adopt(&h_get_string);
    h_global.adopt(&h_form);
    h_logs.adopt(&h_warnings);
    h_global.adopt(&h_logs);
    adopt(&h_global);

    h_inter.set_visible(false);
    h_get_string.set_visible(false);
    h_form.set_visible(false);

    h_pause2.record_actor_on_event(this, html_form_radio::changed);
    h_get_string.record_actor_on_event(this, html_form_input::changed);

    rebuild_body_part = false;
    ignore_event = false;
    just_set = false;
}

html_web_user_interaction::html_web_user_interaction(const html_web_user_interaction & ref):
    h_inter(""),
    h_get_string("", html_form_input::text, "", 1),
    h_form(""),
    h_logs(""),
    h_global(""),
    lib_data(1)
{
    throw WEBDAR_BUG;
}


string html_web_user_interaction::get_body_part(const chemin & path,
					   const request & req)
{
    string ret;

	// in the following we need to ignore on_event() calls to be able to
	// se the html_interface without considering it as
	// an interaction from the user
    ignore_event = true;
    just_set = false;
    try
    {
	list<string> logs = lib_data.get_warnings();
	string msg;
	bool echo;

	h_warnings.clear();
	for(list<string>::iterator it = logs.begin();
	    it != logs.end();
	    ++it)
	{
	    h_warnings.add_text(0, *it);
	    h_warnings.add_nl();
	}

	if(lib_data.pending_pause2(msg))
	{
	    if(!h_inter.get_next_visible())
	    {
		h_inter.set_visible(true);
		h_inter_text.clear();
		h_inter_text.add_text(0, msg);
		h_pause2.set_selected(0);
		just_set = true;
	    }
	}

	if(lib_data.pending_get_string(msg, echo))
	{
	    if(!h_get_string.get_next_visible())
	    {
		h_get_string.set_visible(true);
		h_get_string.change_label(msg);
		if(echo)
		    h_get_string.change_type(html_form_input::text);
		else
		    h_get_string.change_type(html_form_input::password);
		h_get_string.set_value("");
		just_set = true;
	    }
	}

	if(lib_data.pending_get_secu_string(msg, echo))
	{
	    if(!h_get_string.get_next_visible())
	    {
		h_get_string.set_visible(true);
		h_get_string.change_label(msg);
		if(echo)
		    h_get_string.change_type(html_form_input::text);
		else
		    h_get_string.change_type(html_form_input::password);
		h_get_string.set_value("");
		just_set = true;
	    }
	}
    }
    catch(...)
    {
	ignore_event = false;
	throw;
    }
    ignore_event = false;


	// now we return to the user the updated html interface
	// any event is a user event and may need further display

    rebuild_body_part = false;
    adjust_visibility();
    ret = get_body_part_from_all_children(path, req);
    if(rebuild_body_part)
    {
	adjust_visibility();
	ret = get_body_part_from_all_children(path, req);
    }

    return ret;
}

void html_web_user_interaction::on_event(const std::string & event_name)
{
    if(!ignore_event)
    {
	if(h_inter.get_visible() && !just_set)
	{
	    if(h_pause2.get_selected_num() != 0)
	    {
		lib_data.set_pause2_answer(h_pause2.get_selected_num() == 2);
		h_inter.set_visible(false);
		rebuild_body_part = true;
	    }
		// else we do nothing here
	}

	if(h_get_string.get_visible() && !just_set)
	{
	    string tmpm;
	    bool tmpe;
	    if(lib_data.pending_get_string(tmpm, tmpe))
	    {
		lib_data.set_get_string_answer(h_get_string.get_value());
		h_get_string.set_visible(false);
		rebuild_body_part = true;
	    }
	    else
	    {
		if(lib_data.pending_get_secu_string(tmpm, tmpe))
		{
		    lib_data.set_get_secu_string_answer(libdar::secu_string(h_get_string.get_value().c_str(), h_get_string.get_value().size()));
		    h_get_string.set_visible(false);
		    rebuild_body_part = true;
		}
		else
		    throw WEBDAR_BUG;
	    }
	}
    }
}

void html_web_user_interaction::clear()
{
    lib_data.clear();
    h_inter.set_visible(false);
    h_get_string.set_visible(false);
    adjust_visibility();
}

void html_web_user_interaction::adjust_visibility()
{
    if(h_get_string.get_next_visible() || h_inter.get_next_visible() || just_set)
	h_form.set_visible(true);
    else
	h_form.set_visible(false);
}
