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

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "webdar_css_style.hpp"


    //
#include "html_web_user_interaction.hpp"

using namespace std;


const string html_web_user_interaction::ask_end_libdar = "html_web_user_interaction_ask_end_libdar";
const string html_web_user_interaction::force_end_libdar = "html_web_user_interaction_force_end_libdar";
const string html_web_user_interaction::kill_libdar_thread = "html_web_user_interaction_kill_libdar_thread";
const string html_web_user_interaction::close_libdar_screen = "html_web_user_interaction_close_libdar_screen";
const string html_web_user_interaction::can_refresh = "html_web_user_interaction_canrefresh";
const string html_web_user_interaction::dont_refresh = "html_web_user_interaction_dontrefresh";


const string html_web_user_interaction::class_button = "html_web_user_interaction_button";
const string html_web_user_interaction::class_web = "html_web_user_interaction_web";
const string html_web_user_interaction::class_inter = "html_web_user_interaction_inter";


html_web_user_interaction::html_web_user_interaction(unsigned int x_warn_size):
    h_inter(""),
    h_get_string("", html_form_input::text, "", 20),
    h_form("Update"),
    h_logs("Last logs"),
    h_global("Interaction with libdar"),
    ask_close("Gracefully stop libdar", ask_end_libdar),
    force_close("Immediately stop libdar", force_end_libdar),
    kill_close("Kill libdar thread", kill_libdar_thread),
    finish("Close", close_libdar_screen)
{
    lib_data.reset(new (nothrow) web_user_interaction(x_warn_size));
    if(!lib_data)
	throw exception_memory();

    h_pause.add_choice("undefined", "please answer yes or no");
    h_pause.add_choice("no", "No");
    h_pause.add_choice("yes", "Yes");

    h_inter.adopt(&h_inter_text);
    h_inter.adopt(&h_pause);
    h_form.adopt(&h_inter);
    h_form.adopt(&h_get_string);
    h_global.adopt(&h_form);
    h_logs.adopt(&h_warnings);
    h_global.adopt(&h_logs);
    h_global.adopt(&stats);
    h_global.adopt(&ask_close);
    h_global.adopt(&force_close);
    h_global.adopt(&kill_close);
    h_global.adopt(&finish);
    adopt(&h_global);

	// visibility

    h_inter.set_visible(false);
    h_get_string.set_visible(false);
    h_form.set_visible(false);
    set_mode(normal);

	// events
    register_name(ask_end_libdar);
    register_name(force_end_libdar);
    register_name(kill_libdar_thread);
    register_name(close_libdar_screen);
    register_name(can_refresh);
    register_name(dont_refresh);

    h_pause.record_actor_on_event(this, html_form_radio::changed);
    h_get_string.record_actor_on_event(this, html_form_input::changed);
    ask_close.record_actor_on_event(this, ask_end_libdar);
    force_close.record_actor_on_event(this, force_end_libdar);
    kill_close.record_actor_on_event(this, kill_libdar_thread);
    finish.record_actor_on_event(this, close_libdar_screen);

    rebuild_body_part = false;
    ignore_event = false;
    just_set = false;
    event_sent = false;

	// css
    h_inter_text.add_css_class(class_inter);
    stats.add_css_class(class_web);

    webdar_css_style::normal_button(ask_close);
    ask_close.add_css_class(class_button);

    webdar_css_style::active_button(force_close);
    force_close.add_css_class(class_button);

    webdar_css_style::active_button(kill_close);
    kill_close.add_css_class(class_button);

    webdar_css_style::normal_button(finish);
    finish.add_css_class(class_button);
}

string html_web_user_interaction::inherited_get_body_part(const chemin & path,
							  const request & req)
{
    string ret;

	// in the following we need to ignore on_event() calls to be able to
	// se the html_interface without considering it as
	// an interaction from the user
    ignore_event = true; // we're about to change our own component which will trigger events, si we ignore them here
    just_set = false;   // keep trace of these libdar side component changes
    event_sent = false; // keep trace if an event is sent to avoid duplicate

	// fetching libdar status and updating our components from libdar
    check_libdata();
    try
    {
	list<string> logs = lib_data->get_warnings();
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

	if(lib_data->pending_pause(msg))
	{
	    if(!h_inter.get_next_visible())
	    {
		h_inter.set_visible(true);
		h_inter_text.clear();
		h_inter_text.add_text(0, msg);
		h_pause.set_selected(0);
		just_set = true;
	    }
	}

	if(lib_data->pending_get_string(msg, echo))
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

	if(lib_data->pending_get_secu_string(msg, echo))
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
	// libdar status have been updated

	// now we return to the user the updated html interface
	// any event is a user event and may need further display

    rebuild_body_part = false; // libdar change lead compoents to change
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
    check_libdata();

    if(!ignore_event)
    {
	if(event_name == html_form_radio::changed
	   || event_name == html_form_input::changed)
	{
	    if(h_inter.get_visible() && !just_set)
	    {
		if(h_pause.get_selected_num() != 0)
		{
		    lib_data->set_pause_answer(h_pause.get_selected_num() == 2);
		    h_inter.set_visible(false);
		    rebuild_body_part = true;
		}
		    // else we do nothing here
	    }

	    if(h_get_string.get_visible() && !just_set)
	    {
		string tmpm;
		bool tmpe;
		if(lib_data->pending_get_string(tmpm, tmpe))
		{
		    lib_data->set_get_string_answer(h_get_string.get_value());
		    h_get_string.set_visible(false);
		    rebuild_body_part = true;
		}
		else
		{
		    if(lib_data->pending_get_secu_string(tmpm, tmpe))
		    {
			lib_data->set_get_secu_string_answer(libdar::secu_string(h_get_string.get_value().c_str(), h_get_string.get_value().size()));
			h_get_string.set_visible(false);
			rebuild_body_part = true;
		    }
		    else
			throw WEBDAR_BUG;
		}
	    }
	}
	else
	{
	    if(! event_sent) // avoid sending twice the event in case of visibility change
	    {
		event_sent = true;

		switch(mode)
		{
		case normal:
		    if(event_name == ask_end_libdar)
			set_mode(end_asked);
		    break;
		case end_asked:
		    if(event_name == force_end_libdar)
			set_mode(end_forced);
		    break;
		case end_forced:
		    if(event_name == kill_libdar_thread)
			set_mode(kill_forced);
		    break;
		case kill_forced:
		    break; // nothing to do
		case finished:
		    break; // mode already changed by the finish() method
		default:
		    throw WEBDAR_BUG;
		}
		act(event_name);  // propagate the event from inner buttons
	    }
	}
    }
}

void html_web_user_interaction::clear()
{
    check_libdata();
    lib_data->clear();
    h_inter.set_visible(false);
    h_get_string.set_visible(false);
    adjust_visibility();
    set_mode(normal);
    stats.clear_counters();
    stats.clear_labels();
}

void html_web_user_interaction::new_css_library_available()
{
    css tmp;

    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    if(! csslib->class_exists(class_inter))
    {
	tmp.css_font_weight_bold();
	tmp.css_color("blue");
	define_css_class_in_library(class_inter, tmp);

	tmp.clear();
	tmp.css_width("90%", true);
	csslib->add(class_web, tmp);

	tmp.clear();
	tmp.css_float(css::fl_right);
	tmp.css_float_clear(css::fc_both);
	tmp.css_margin_right("1em");
	csslib->add(class_button, tmp);
    }
    else
    {
	if(! csslib->class_exists(class_web)
	   || ! csslib->class_exists(class_button))
	    throw WEBDAR_BUG;
    }

    webdar_css_style::update_library(*csslib);
}

void html_web_user_interaction::adjust_visibility()
{
    if(h_get_string.get_next_visible() || h_inter.get_next_visible() || just_set)
    {
	h_form.set_visible(true);
	act(dont_refresh);
    }
    else
    {
	h_form.set_visible(false);
	if(mode != finished)
	    act(can_refresh);
    }
}

void html_web_user_interaction::set_mode(mode_type m)
{
    switch(m)
    {
    case normal:
	ask_close.set_visible(true);
	force_close.set_visible(false);
	kill_close.set_visible(false);
	finish.set_visible(false);
	break;
    case end_asked:
	ask_close.set_visible(false);
	force_close.set_visible(true);
	kill_close.set_visible(false);
	finish.set_visible(false);
	break;
    case end_forced:
	ask_close.set_visible(false);
	force_close.set_visible(false);
	kill_close.set_visible(true);
	finish.set_visible(false);
	break;
    case kill_forced:
	ask_close.set_visible(false);
	force_close.set_visible(false);
	kill_close.set_visible(false);
	finish.set_visible(false);
	act(can_refresh);
	break;
    case finished:
	ask_close.set_visible(false);
	force_close.set_visible(false);
	kill_close.set_visible(false);
	finish.set_visible(true);
	break;
    default:
	throw WEBDAR_BUG;
    }

    mode = m;
}
