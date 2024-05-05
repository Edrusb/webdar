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
    // public events:
const string html_web_user_interaction::display_started = "html_web_user_interaction_displaystarted";
const string html_web_user_interaction::can_refresh = "html_web_user_interaction_canrefresh";
const string html_web_user_interaction::dont_refresh = "html_web_user_interaction_dontrefresh";
const string html_web_user_interaction::libdar_has_finished = "html_web_user_interaction_libdarfinished";
    // local css class names:
const string html_web_user_interaction::class_button = "html_web_user_interaction_button";
const string html_web_user_interaction::class_web = "html_web_user_interaction_web";
const string html_web_user_interaction::class_inter = "html_web_user_interaction_inter";
    // internal events:
const string html_web_user_interaction::ask_end_libdar = "html_web_user_interaction_ask_end_libdar";
const string html_web_user_interaction::force_end_libdar = "html_web_user_interaction_force_end_libdar";
const string html_web_user_interaction::kill_libdar_thread = "html_web_user_interaction_kill_libdar_thread";
const string html_web_user_interaction::close_libdar_screen = "html_web_user_interaction_close_libdar_screen";



html_web_user_interaction::html_web_user_interaction(unsigned int x_warn_size):
    mode(closed), // to force set_mode(normal) to do something in constructor body below
    autohide(false),
    hide_unless_interrupted(false),
    was_interrupted(false),
    h_inter(""),
    h_get_string("", html_form_input::text, "", 20),
    h_form("Update"),
    h_logs("Last logs"),
    h_global("Interaction with libdar"),
    ask_close("Gracefully stop libdar", ask_end_libdar),
    force_close("Immediately stop libdar", force_end_libdar),
    kill_close("Kill libdar thread", kill_libdar_thread),
    finish("Close", close_libdar_screen),
    ignore_event(false)
{
    lib_data.reset(new (nothrow) web_user_interaction(x_warn_size));
    if(!lib_data)
	throw exception_memory();

	// status fields

    h_pause.add_choice("undefined", "please answer yes or no");
    h_pause.add_choice("no", "No");
    h_pause.add_choice("yes", "Yes");

    h_warnings.ignore_body_changed_from_my_children(true);

	// adoption tree
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

	// events we provide
    register_name(display_started);
    register_name(can_refresh);
    register_name(dont_refresh);
    register_name(libdar_has_finished);

	// events we act upon
    h_pause.record_actor_on_event(this, html_form_radio::changed);
    h_get_string.record_actor_on_event(this, html_form_input::changed);
    ask_close.record_actor_on_event(this, ask_end_libdar);
    force_close.record_actor_on_event(this, force_end_libdar);
    kill_close.record_actor_on_event(this, kill_libdar_thread);
    finish.record_actor_on_event(this, close_libdar_screen);

	// visibility and object status
    clear();

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

html_web_user_interaction::~html_web_user_interaction()
{
    try
    {
	kill_threads();
	join_all_threads();
    }
    catch(...)
    {
	    // do not propagate any exception
    }
}


void html_web_user_interaction::run_and_control_thread(libthreadar::thread* arg)
{
    if(arg == nullptr)
	throw WEBDAR_BUG;
    managed_threads.push_back(arg);
    set_visible(true);
    arg->run();
}

bool html_web_user_interaction::is_libdar_running() const
{
    bool ret = false;

    deque<libthreadar::thread*>::const_iterator it = managed_threads.begin();

    while(it != managed_threads.end() && !ret)
    {
	if(*it == nullptr)
	    throw WEBDAR_BUG;
	if((*it)->is_running())
	    ret = true;
	++it;
    }

    return ret;
}

string html_web_user_interaction::inherited_get_body_part(const chemin & path,
							  const request & req)
{
    string ret;

	// updating components from libdar status
    update_html_from_libdar_status();

	// monitoring libdar thread status
    check_thread_status();

	// now we return to the user the updated html interface
	// any event triggered during that first generation may
	// need further re-display (rebuild_body_part is set to true in that case)
    ret = get_body_part_from_all_children(path, req);

    return ret;
}

void html_web_user_interaction::on_event(const string & event_name)
{
    check_libdata();

    if(!ignore_event)
    {
	if(event_name == html_form_radio::changed)
	{
	    if(h_inter.get_visible())
	    {
		if(h_pause.get_selected_num() != 0)
		{
		    lib_data->set_pause_answer(h_pause.get_selected_num() == 2);
		    h_inter.set_visible(false);
		    my_body_part_has_changed();
		}
		    // else we do nothing here
	    }
	    else
		throw WEBDAR_BUG;
		// how could we get an event from h_pause
		// if its parent h_inter is hidden?
	}
	else if(event_name == html_form_input::changed)
	{
	    if(h_get_string.get_visible())
	    {
		string tmpm;
		bool tmpe;
		if(lib_data->pending_get_string(tmpm, tmpe))
		    lib_data->set_get_string_answer(h_get_string.get_value());
		else if(lib_data->pending_get_secu_string(tmpm, tmpe))
		    lib_data->set_get_secu_string_answer(libdar::secu_string(h_get_string.get_value().c_str(), h_get_string.get_value().size()));
		else
		    throw WEBDAR_BUG;

		h_get_string.set_visible(false);
		my_body_part_has_changed();
	    }
	    else
		throw WEBDAR_BUG;
		// how could we get an event from h_get_string
		// if it is not visible?
	}
	else if(event_name == ask_end_libdar)
	    set_mode(end_asked); // eventually calls my_body_part_has_changed()
	else if(event_name == force_end_libdar)
	    set_mode(end_forced); // eventually calls my_body_part_has_changed()
	else if(event_name == kill_libdar_thread)
	    set_mode(kill_forced); // eventually calls my_body_part_has_changed()
	else if(event_name == close_libdar_screen)
	    set_mode(closed); // eventually calls my_body_part_has_changed()
	else
	    throw WEBDAR_BUG;
    }
}

void html_web_user_interaction::clear()
{
    check_libdata();
    lib_data->clear();
    h_inter.set_visible(false);
    h_get_string.set_visible(false);
    adjust_visibility();
    stats.clear_counters();
    stats.clear_labels();
    hide_statistics();
    set_mode(normal);
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

void html_web_user_interaction::my_visibility_has_changed()
{
    if(get_visible())
    {
	act(display_started);
	trigger_refresh();
    }
}

void html_web_user_interaction::adjust_visibility()
{
    if(h_get_string.get_visible() || h_inter.get_visible())
    {
	if(!h_form.get_visible())
	{
	    h_form.set_visible(true);
	    act(dont_refresh);
	}
	    // else nothing new to perform
    }
    else
    {
	if(h_form.get_visible())
	{
	    h_form.set_visible(false);
	    if(mode != finished && mode != closed)
		act(can_refresh);
	}
	    // else nothing new to perform
    }
}

void html_web_user_interaction::set_mode(mode_type m)
{
    if(m == mode)
	return;

    switch(m)
    {
    case normal:
	ask_close.set_visible(true);
	force_close.set_visible(false);
	kill_close.set_visible(false);
	finish.set_visible(false);
	set_visible(true);
	was_interrupted = false;
	trigger_refresh();
	break;
    case end_asked:
	ask_close.set_visible(false);
	force_close.set_visible(true);
	kill_close.set_visible(false);
	finish.set_visible(false);
	clean_end_threads(false);
	break;
    case end_forced:
	ask_close.set_visible(false);
	force_close.set_visible(false);
	kill_close.set_visible(true);
	finish.set_visible(false);
	clean_end_threads(true);
	break;
    case kill_forced:
	ask_close.set_visible(false);
	force_close.set_visible(false);
	kill_close.set_visible(false);
	finish.set_visible(false);
	kill_threads();
	trigger_refresh();
	break;
    case finished:
	ask_close.set_visible(false);
	force_close.set_visible(false);
	kill_close.set_visible(false);
	finish.set_visible(true);
	if(!autohide || (was_interrupted && hide_unless_interrupted))
	{
	    act(dont_refresh);
	    break; // break for the 'case finished:' !!!
	}
	else // if auto hide is set, we go to the closed status, here below
	    m = closed;
	    // no break!
    case closed:
	set_visible(false); // we hide ourself
	act(libdar_has_finished);
	if(autohide && (!was_interrupted || !hide_unless_interrupted))
	    act(dont_refresh);
	break;
    default:
	throw WEBDAR_BUG;
    }

    mode = m;
}


void html_web_user_interaction::update_html_from_libdar_status()
{
    check_libdata();

    	// in the following we need to ignore on_event() calls to be able to
	// se the html_interface without considering it as
	// an interaction from the user

    ignore_event = true; // we're about to change our own component which may trigger events, we ignore them here
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
	    if(!h_inter.get_visible())
	    {
		h_inter.set_visible(true);
		h_inter_text.clear();
		h_inter_text.add_text(0, msg);
		h_pause.set_selected(0);
	    }
	}

	if(lib_data->pending_get_string(msg, echo)
	   || lib_data->pending_get_secu_string(msg, echo))
	{
	    if(!h_get_string.get_visible())
	    {
		h_get_string.set_visible(true);
		h_get_string.change_label(msg);
		if(echo)
		    h_get_string.change_type(html_form_input::text);
		else
		    h_get_string.change_type(html_form_input::password);
		h_get_string.set_value("");
	    }
	}

	adjust_visibility();
    }
    catch(...)
    {
	ignore_event = false;
	throw;
    }
    ignore_event = false;
}

void html_web_user_interaction::check_thread_status()
{
    while( ! managed_threads.empty()
	  && managed_threads.back() != nullptr
	  && ! managed_threads.back()->is_running())
    {
	libthreadar::thread* tmp = managed_threads.back();
	managed_threads.pop_back(); // doing that before join in case join triggers an exception

	try
	{
	    tmp->join();
	}
	catch(exception_base & e)
	{
	    e.change_message(string("Error reported from libdar: ") + e.get_message());
	    was_interrupted = true;
	    throw;
	}
	catch(libdar::Egeneric & e)
	{
	    was_interrupted = true;
	    throw exception_libcall(e);
	}
    }

    if(! managed_threads.empty()
       && managed_threads.back() == nullptr)
	throw WEBDAR_BUG;

    if(managed_threads.empty())
	set_mode(finished);
}

void html_web_user_interaction::clean_end_threads(bool force)
{
    if( ! managed_threads.empty()
	&& managed_threads.back() != nullptr)
    {
	pthread_t libdar_tid;
	if(managed_threads.back()->is_running(libdar_tid))
	{
	    libdar::thread_cancellation th;
	    th.cancel(libdar_tid, force, 0);
	    was_interrupted = true;
	}
    }

    if(! managed_threads.empty()
       && managed_threads.back() == nullptr)
	throw WEBDAR_BUG;
}

void html_web_user_interaction::kill_threads()
{
    bool bug = false;
    deque<libthreadar::thread*>::reverse_iterator rit = managed_threads.rbegin();

    while(rit != managed_threads.rend())
    {
	if(*rit == nullptr)
	    bug = true;
	else
	{
	    if((*rit)->is_running())
		(*rit)->kill();
	}

	rit++;
    }

    if(bug)
	throw WEBDAR_BUG;
}


void html_web_user_interaction::join_all_threads()
{
    deque<libthreadar::thread*>::reverse_iterator rit = managed_threads.rbegin();

    while(rit != managed_threads.rend())
    {
	if(*rit != nullptr)
	{
	    if((*rit)->is_running())
	    {
		try
		{
		    (*rit)->join();
		}
		catch(...)
		{
			// we do not propagate any exception
		}
	    }
	}

	rit++;
    }
}

void html_web_user_interaction::trigger_refresh()
{
    if(h_form.get_visible())
	act(dont_refresh);
    else
	act(can_refresh);
}
