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

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "webdar_css_style.hpp"
    //
#include "html_libdar_running.hpp"

using namespace std;

const string html_libdar_running::ask_end_libdar = "html_libdar_running_ask_end_libdar";
const string html_libdar_running::force_end_libdar = "html_libdar_running_force_end_libdar";
const string html_libdar_running::kill_libdar_thread = "html_libdar_running_kill_libdar_thread";
const string html_libdar_running::close_libdar_screen = "html_libdar_running_close_libdar_screen";

html_libdar_running::html_libdar_running():
    html_page("THIS IS A BUG"),
    ask_close("Gracefully stop libdar", ask_end_libdar),
    force_close("Immediately stop libdar", force_end_libdar),
    kill_close("Kill libdar thread", kill_libdar_thread),
    finish("Close", close_libdar_screen)
{
    sessname = "";

    global.css_margin("1em");
    global.css_padding("1em");
    global.css_border_style(css::bd_all, css::bd_inset);
    global.css_border_width(css::bd_all, css::bd_medium);

    web_ui.css_width("90%", true);
    stats.css_width("90%", true);

    webdar_style_normal_button(ask_close);
    ask_close.css_float(css::fl_right);
    ask_close.css_float_clear(css::fc_both);
    ask_close.css_margin_right("1em");

    webdar_style_active_button(force_close);
    force_close.css_float(css::fl_right);
    force_close.css_float_clear(css::fc_both);
    force_close.css_margin_right("1em");

    webdar_style_active_button(kill_close);
    kill_close.css_float(css::fl_right);
    kill_close.css_float_clear(css::fc_both);
    kill_close.css_margin_right("1em");

    webdar_style_normal_button(finish);
    finish.css_float(css::fl_right);
    finish.css_float_clear(css::fc_both);
    finish.css_margin_right("1em");

    global.adopt(&web_ui);
    global.adopt(&stats);
    adopt(&global);
    adopt(&ask_close);
    adopt(&force_close);
    adopt(&kill_close);
    adopt(&finish);

    register_name(ask_end_libdar);
    register_name(force_end_libdar);
    register_name(kill_libdar_thread);
    register_name(close_libdar_screen);

    visibility_has_changed = false;
    set_mode(normal);

    ask_close.record_actor_on_event(this, ask_end_libdar);
    force_close.record_actor_on_event(this, force_end_libdar);
    kill_close.record_actor_on_event(this, kill_libdar_thread);
    finish.record_actor_on_event(this, close_libdar_screen);
}

string html_libdar_running::get_body_part(const chemin & path,
					  const request & req)
{
    string body;

    visibility_has_changed = false;
    body = get_body_part_from_children_as_a_block(path, req);
    if(visibility_has_changed)
	body = get_body_part_from_children_as_a_block(path, req);

    if((web_ui.can_refresh() && mode != finished) || mode == kill_forced)
	set_refresh_redirection(1, req.get_uri().get_path().display(false));
    else
	set_refresh_redirection(0, ""); // disable refresh

    return get_body_part_given_the_body(path, req, body);
}

void html_libdar_running::on_event(const std::string & event_name)
{
    if(!visibility_has_changed) // avoid sending twice the event in case of visibility change
    {
	visibility_has_changed = true;
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
	    break; // mode already changed by the libdar_has_finished() method
	default:
	    throw WEBDAR_BUG;
	}
	act(event_name);   // propagate the event
    }
} // propagate the event from inner buttons

void html_libdar_running::clear()
{
    set_mode(normal);
    web_ui.clear();
    stats.clear_counters();
    stats.clear_labels();
}


void html_libdar_running::set_mode(mode_type m)
{
    switch(m)
    {
    case normal:
	ask_close.set_visible(true);
	force_close.set_visible(false);
	kill_close.set_visible(false);
	finish.set_visible(false);
	set_title(webdar_tools_get_title(sessname, "Libdar is running"));
	break;
    case end_asked:
	ask_close.set_visible(false);
	force_close.set_visible(true);
	kill_close.set_visible(false);
	finish.set_visible(false);
	set_title(webdar_tools_get_title(sessname, "Libdar is gracefully ending"));
	break;
    case end_forced:
	ask_close.set_visible(false);
	force_close.set_visible(false);
	kill_close.set_visible(true);
	finish.set_visible(false);
	set_title(webdar_tools_get_title(sessname, "Libdar is immediately ending"));
	break;
    case kill_forced:
	ask_close.set_visible(false);
	force_close.set_visible(false);
	kill_close.set_visible(false);
	finish.set_visible(false);
	set_title(webdar_tools_get_title(sessname, "Libdar thread is being killed"));
	break;
    case finished:
	ask_close.set_visible(false);
	force_close.set_visible(false);
	kill_close.set_visible(false);
	finish.set_visible(true);
	set_title(webdar_tools_get_title(sessname, "Libdar has finished"));
	break;
    default:
	throw WEBDAR_BUG;
    }

    mode = m;
}
