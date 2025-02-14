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
#include "tokens.hpp"
#include "exceptions.hpp"

    //
#include "user_interface.hpp"

using namespace std;

const string user_interface::closing = "user_interface_closing";

user_interface::user_interface()
{
    sessname = "";
    mode = config;
    mode_changed = false;
    close_requested = false;
    disconnect_req = false;

    data.reset(new (nothrow) html_fichier());
    if(!data)
	throw exception_memory();

    parametrage.set_data_place(data);

	/// messages receved from saisie object named parametrage
    parametrage.record_actor_on_event(this, saisie::event_closing);
    parametrage.record_actor_on_event(this, saisie::event_restore);
    parametrage.record_actor_on_event(this, saisie::event_compare);
    parametrage.record_actor_on_event(this, saisie::event_test);
    parametrage.record_actor_on_event(this, saisie::event_list);
    parametrage.record_actor_on_event(this, saisie::event_summary);
    parametrage.record_actor_on_event(this, saisie::event_create);
    parametrage.record_actor_on_event(this, saisie::event_isolate);
    parametrage.record_actor_on_event(this, saisie::event_merge);
    parametrage.record_actor_on_event(this, saisie::event_repair);
    parametrage.record_actor_on_event(this, saisie::changed_session_name);
    parametrage.record_actor_on_event(this, saisie::event_disconn);
    parametrage.record_actor_on_event(this, saisie::event_download);

	/// messages received from html_libdar_running object named in_action
    in_action.record_actor_on_event(this, html_libdar_running_page::libdar_has_finished);

	/// messages received from html_error object named in_error;
    in_error.record_actor_on_event(this, html_error::acknowledged);

	/// messages received from html_listring_page object named in_list
    in_list.record_actor_on_event(this, html_listing_page::event_close);
    in_summ.record_actor_on_event(this, html_summary_page::event_close);

    current_thread = nullptr;

	/// create the events that this object is willing to generate
    register_name(closing);
}

user_interface::~user_interface()
{
    if(is_libdar_running())
    {
	if(current_thread != nullptr)
	{
	    if(current_thread->is_running())
		current_thread->cancel();
	    current_thread->join(); // this later call may propagate exceptions
	    current_thread = nullptr;
	}
    }
}

answer user_interface::give_answer(const request & req)
{
    answer ret;

    ret.set_status(STATUS_CODE_OK);
    ret.set_reason("ok");

    do
    {
	mode_changed = false;
	try
	{
	    switch(mode)
	    {
	    case config:
		ret.add_body(parametrage.get_body_part(req.get_uri().get_path(), req));
		if(mode == download)
		{
		    try
		    {
			ret.clear();
			ret.set_status(STATUS_CODE_OK);
			ret.set_reason("ok");
			if(!data)
			    throw WEBDAR_BUG;
			ret.add_body(data->get_body_part(req.get_uri().get_path(), req));
			data->clear();
		    }
		    catch(...)
		    {
			mode = config;
			throw;
		    }
		    mode = config;
		}
		break;
	    case listing:
		ret.add_body(in_list.get_body_part(req.get_uri().get_path(), req));
		break;
	    case summary:
		ret.add_body(in_summ.get_body_part(req.get_uri().get_path(), req));
		break;
	    case listing_open:
	    case summary_open:
	    case running:
		ret.add_body(in_action.get_body_part(req.get_uri().get_path(), req));
		break;
	    case error:
		ret.add_body(in_error.get_body_part(req.get_uri().get_path(), req));
		break;
	    case download:
		throw WEBDAR_BUG; // this mode should be transitional from config
	    default:
		throw WEBDAR_BUG;
	    }
	}
	catch(exception_bug & e)
	{
	    throw;
	}
	catch(libthreadar::exception_base & e)
	{
	    if(mode == error)
		throw; // cannot handle an exception in error mode
	    return_mode = mode;
	    mode = error;
	    mode_changed = true;
	    in_error.set_message(e.get_message(": "));
	}
	catch(exception_base & e)
	{
	    if(mode == error)
		throw; // cannot handle an exception in error mode
	    return_mode = mode;
	    mode = error;
	    mode_changed = true;
	    in_error.set_message(e.get_message());
	}
    }
    while(mode_changed);

    return ret;
}

void user_interface::on_event(const string & event_name)
{
    if(event_name == saisie::event_closing)
    {
	switch(mode)
	{
	case config:
	    act(closing); // transmetting the event
	    break;
	case listing_open:
	    throw WEBDAR_BUG;
	case summary_open:
	    throw WEBDAR_BUG;
	case listing:
	    throw WEBDAR_BUG;
	case summary:
	    throw WEBDAR_BUG;
	case running:
	    throw WEBDAR_BUG;
	case error:
	    throw WEBDAR_BUG;
	case download:
	    throw WEBDAR_BUG;
	default:
	    throw WEBDAR_BUG;
	}
    }
    else if(event_name == html_libdar_running_page::libdar_has_finished)
    {
	current_thread = nullptr;

	switch(mode)
	{
	case config:
	    throw WEBDAR_BUG;
	case listing_open:
	case summary_open:
	    mode_changed = true;
	    if(! in_action.has_libdar_been_aborted())
	    {
		switch(mode)
		{
		case listing_open:
		    mode = listing;
		    in_list.set_source(&arch_init_list);
		    break;
		case summary_open:
		    mode = summary;
		    in_summ.set_source(&arch_init_list);
		    break;
		default:
		    throw WEBDAR_BUG;
		}
	    }
	    else
	    {
		arch_init_list.close_archive();
		mode = config;
	    }
	    break;
	case listing:
	    throw WEBDAR_BUG;
	case summary:
	    throw WEBDAR_BUG;
	case running:
	    mode_changed = true;
	    mode = config;
	    break;
	case error:
	    throw WEBDAR_BUG;
	default:
	    throw WEBDAR_BUG;
	}
    }
    else if(event_name == saisie::event_restore
	    || event_name == saisie::event_compare
	    || event_name == saisie::event_test
	    || event_name == saisie::event_create
	    || event_name == saisie::event_isolate
	    || event_name == saisie::event_list
	    || event_name == saisie::event_summary
	    || event_name == saisie::event_merge
	    || event_name == saisie::event_repair)
    {
	if(mode != config)
	    throw WEBDAR_BUG;
	if(event_name == saisie::event_list)
	    mode = listing_open;
	else if(event_name == saisie::event_summary)
	    mode = summary_open;
	else
	    mode = running;

	in_action.get_html_user_interaction()->clear();
	mode_changed = true;

	try
	{
	    if(event_name == saisie::event_restore)
		go_restore();
	    else if(event_name == saisie::event_compare)
		go_diff();
	    else if(event_name == saisie::event_test)
		go_test();
	    else if(event_name == saisie::event_create)
		go_create();
	    else if(event_name == saisie::event_isolate)
		go_isolate();
	    else if(event_name == saisie::event_merge)
		go_merge();
	    else if(event_name == saisie::event_list
		    || event_name == saisie::event_summary)
		go_init_list();
	    else if(event_name == saisie::event_repair)
		go_repair();
	    else
		throw WEBDAR_BUG;
	}
	catch(exception_bug & e)
	{
	    throw;
	}
	catch(...)
	{
	    mode = config;
	    mode_changed = true;
	    throw;
	}
    }
    else if(event_name == html_error::acknowledged)
    {
	if(return_mode != listing && return_mode != summary)
	    mode = return_mode;
	else
	    mode = config;
	mode_changed = true;
    }
    else if(event_name == saisie::changed_session_name)
	set_session_name(parametrage.get_session_name());
    else if(event_name == html_listing_page::event_close)
    {
	if(mode != listing)
	    throw WEBDAR_BUG;
	if(!arch_init_list.opened())
	    throw WEBDAR_BUG;
	in_list.clear();
	arch_init_list.close_archive();
	mode = config;
	mode_changed = true;
    }
    else if(event_name == html_summary_page::event_close)
    {
	if(mode != summary)
	    throw WEBDAR_BUG;
	if(!arch_init_list.opened())
	    throw WEBDAR_BUG;
	in_summ.clear();
	arch_init_list.close_archive();
	mode = config;
	mode_changed = true;
    }
    else if(event_name == saisie::event_disconn)
    {
	disconnect_req = true;
    }
    else if(event_name == saisie::event_download)
    {
	mode = download;
    }
    else
	throw WEBDAR_BUG; // what's that event !?!
}

string user_interface::get_session_name() const
{
    string ret;

    mut_sessname.lock();
    try
    {
	ret = sessname;
    }
    catch(...)
    {
	mut_sessname.unlock();
	throw;
    }
    mut_sessname.unlock();

    return ret;
};


void user_interface::set_session_name(const string & name)
{
    mut_sessname.lock();
    try
    {
	sessname = name;
    }
    catch(...)
    {
	mut_sessname.unlock();
	throw;
    }
    mut_sessname.unlock();

	// no need to be protected by mutex for
	// the following operation as they only
	// concern private fields for that object
	// that are not visible by any other thread
    in_action.set_session_name(name);
    in_error.set_session_name(name);
    in_list.set_session_name(name);
    in_summ.set_session_name(name);
	// note, parametrage is already set with the new session name
	// the session name is filled by the user using the saisie class (parametrage)
	// which update itself and triggers the event "saisie::changed_session_name"
	// for which the session object is registered and call this method to update
	// other components
}

void user_interface::prefix_has_changed()
{
    parametrage.set_prefix(get_prefix());
    in_action.set_prefix(get_prefix());
    in_error.set_prefix(get_prefix());
    in_list.set_prefix(get_prefix());
    in_summ.set_prefix(get_prefix());

	// by the way the prefix is also the session ID used as initial session name
    if(parametrage.get_session_name() == "")
    {
	    // so if the session name has not been set
	    // we set it to the prefix
	parametrage.set_session_name(get_prefix().display(true));
	    // this has the effect to put the default
	    // value in the html_form_input field
	    // but does not modify the title of pages for
	    // the session.
    }

}

void user_interface::go_restore()
{
    if(in_action.is_libdar_running())
	throw WEBDAR_BUG;

    if(current_thread != nullptr)
	throw WEBDAR_BUG;

	// providing libdar::parameters
    arch_rest.set_user_interaction(get_html_user_interaction());
    arch_rest.set_parametrage(&get_parametrage());

	// launching libdar in a separated thread
    current_thread = & arch_rest;
    if(current_thread->is_running())
	throw WEBDAR_BUG;
    in_action.run_and_control_thread(current_thread);
}

void user_interface::go_diff()
{
    if(in_action.is_libdar_running())
	throw WEBDAR_BUG;

    if(current_thread != nullptr)
	throw WEBDAR_BUG;

	// providing libdar::parameters
    arch_diff.set_user_interaction(get_html_user_interaction());
    arch_diff.set_parametrage(&get_parametrage());

	// launching libdar in a separated thread
    current_thread = & arch_diff;
    if(current_thread->is_running())
	throw WEBDAR_BUG;
    in_action.run_and_control_thread(current_thread);
}


void user_interface::go_test()
{
    if(is_libdar_running())
	throw WEBDAR_BUG;

    if(current_thread != nullptr)
	throw WEBDAR_BUG;

	// providing libdar::parameters
    arch_test.set_user_interaction(get_html_user_interaction());
    arch_test.set_parametrage(&get_parametrage());

	// launching libdar in a separated thread
    current_thread = & arch_test;
    if(current_thread->is_running())
	throw WEBDAR_BUG;
    in_action.run_and_control_thread(current_thread);
}

void user_interface::go_create()
{
    if(is_libdar_running())
	throw WEBDAR_BUG;

    if(current_thread != nullptr)
	throw WEBDAR_BUG;

	// providing libdar::parameters
    arch_create.set_user_interaction(get_html_user_interaction());
    arch_create.set_parametrage(&get_parametrage());

	// launching libdar in a separated thread
    current_thread = & arch_create;
    if(current_thread->is_running())
	throw WEBDAR_BUG;
    in_action.run_and_control_thread(current_thread);
}

void user_interface::go_isolate()
{
    if(is_libdar_running())
	throw WEBDAR_BUG;

    if(current_thread != nullptr)
	throw WEBDAR_BUG;

	// providing libdar::parameters
    arch_isolate.set_user_interaction(get_html_user_interaction());
    arch_isolate.set_parametrage(&get_parametrage());

	// launching libdar in a separated thread
    current_thread = & arch_isolate;
    if(current_thread->is_running())
	throw WEBDAR_BUG;
    current_thread->join(); // in case a previous execution triggered an exception
    in_action.run_and_control_thread(current_thread);
}

void user_interface::go_merge()
{
    if(is_libdar_running())
	throw WEBDAR_BUG;

    if(current_thread != nullptr)
	throw WEBDAR_BUG;

	// providing libdar::parameters
    arch_merge.set_user_interaction(get_html_user_interaction());
    arch_merge.set_parametrage(&get_parametrage());

	// launching libdar in a separated thread
    current_thread = & arch_merge;
    if(current_thread->is_running())
	throw WEBDAR_BUG;
    in_action.run_and_control_thread(current_thread);
}

void user_interface::go_init_list()
{
    if(is_libdar_running())
	throw WEBDAR_BUG;

    if(current_thread != nullptr)
	throw WEBDAR_BUG;

	// providing libdar::parameters
    arch_init_list.set_user_interaction(get_html_user_interaction());
    arch_init_list.set_parametrage(&get_parametrage());

    	// launching libdar in a separated thread
    current_thread = & arch_init_list;
    if(current_thread->is_running())
	throw WEBDAR_BUG;
    in_action.run_and_control_thread(current_thread);
}

void user_interface::go_repair()
{
   if(is_libdar_running())
	throw WEBDAR_BUG;

    if(current_thread != nullptr)
	throw WEBDAR_BUG;

	// providing libdar::parameters
    arch_repair.set_user_interaction(get_html_user_interaction());
    arch_repair.set_parametrage(&get_parametrage());

	// launching libdar in a separated thread
    current_thread = & arch_repair;
    if(current_thread->is_running())
	throw WEBDAR_BUG;
    in_action.run_and_control_thread(current_thread);
}
