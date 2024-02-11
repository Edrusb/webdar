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

	/// messages receved from saisie object named parametrage
    parametrage.record_actor_on_event(this, saisie::event_closing);
    parametrage.record_actor_on_event(this, saisie::event_restore);
    parametrage.record_actor_on_event(this, saisie::event_compare);
    parametrage.record_actor_on_event(this, saisie::event_test);
    parametrage.record_actor_on_event(this, saisie::event_list);
    parametrage.record_actor_on_event(this, saisie::event_create);
    parametrage.record_actor_on_event(this, saisie::event_isolate);
    parametrage.record_actor_on_event(this, saisie::event_merge);
    parametrage.record_actor_on_event(this, saisie::changed_session_name);

	/// messages received from html_libdar_running object named in_action
    in_action.record_actor_on_event(this, html_libdar_running::libdar_has_finished);

	/// messages received from html_error object named in_error;
    in_error.record_actor_on_event(this, html_error::acknowledged);

	/// messages received from html_listring_page object named in_list
    in_list.record_actor_on_event(this, html_listing_page::event_close);

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
		current_thread->kill();
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
		break;
	    case listing:
		ret.add_body(in_list.get_body_part(req.get_uri().get_path(), req));
		break;
	    case listing_open:
	    case running:
		ret.add_body(in_action.get_body_part(req.get_uri().get_path(), req));
		break;
	    case error:
		ret.add_body(in_error.get_body_part(req.get_uri().get_path(), req));
		break;
	    default:
		throw WEBDAR_BUG;
	    }
	}
	catch(exception_bug & e)
	{
	    throw;
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

void user_interface::on_event(const std::string & event_name)
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
	case listing:
	    throw WEBDAR_BUG;
	case running:
	    throw WEBDAR_BUG;
	case error:
	    throw WEBDAR_BUG;
	default:
	    throw WEBDAR_BUG;
	}
    }
    else if(event_name == html_libdar_running::libdar_has_finished)
    {
	current_thread = nullptr;

	switch(mode)
	{
	case config:
	    throw WEBDAR_BUG;
	case listing_open:
	    mode_changed = true;
	    mode = listing;
	    in_list.set_source(&arch_init_list);
	    break;
	case listing:
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
	    || event_name == saisie::event_merge)
    {
	if(mode != config)
	    throw WEBDAR_BUG;
	if(event_name != saisie::event_list)
	    mode = running;
	else
	    mode = listing_open;
	in_action.clear();
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
	    else if(event_name == saisie::event_list)
		go_init_list();
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
	if(return_mode != listing)
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


void user_interface::set_session_name(const std::string & name)
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
	// note, parametrage is already set with the new session name
	// the session name is filled by the user using the saisie class (parametrage)
	// which update itslef and triggers the event "saisie::changed_session_name"
	// for which the session object is registered and call this method to update
	// other components
}

void user_interface::prefix_has_changed()
{
    parametrage.set_prefix(get_prefix());
    in_action.set_prefix(get_prefix());
    in_error.set_prefix(get_prefix());
    in_list.set_prefix(get_prefix());

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
    arch_rest.set_archive_path(get_parametrage().get_archive_path());
    arch_rest.set_archive_basename(get_parametrage().get_archive_basename());
    arch_rest.set_archive_options_read(get_parametrage().get_read_options(get_html_user_interaction()));
    arch_rest.set_fs_root(get_parametrage().get_fs_root());
    arch_rest.set_archive_options_restore(get_parametrage().get_extraction_options());
    arch_rest.set_progressive_report(get_statistics().get_libdar_statistics());


	// restting counters and logs
    get_html_user_interaction()->clear();
    get_statistics().clear_counters();
    get_statistics().clear_labels();
    get_statistics().set_treated_label("item(s) restored");
    get_statistics().set_skipped_label("item(s) not restored (not saved in archive)");
    get_statistics().set_tooold_label("item(s) not restored (overwriting policy decision)");
    get_statistics().set_errored_label("item(s) failed to restore (filesystem error)");
    get_statistics().set_ignored_label("item(s) ignored (excluded by filters)");
    get_statistics().set_hard_links_label("hard link(s) restored");
    get_statistics().set_ea_treated_label("item(s) having their EA restored");
    get_statistics().set_total_label("item(s) considered");

	// launching libdar in a separated thread
    current_thread = & arch_rest;
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
    arch_diff.set_archive_path(get_parametrage().get_archive_path());
    arch_diff.set_archive_basename(get_parametrage().get_archive_basename());
    arch_diff.set_archive_options_read(get_parametrage().get_read_options(get_html_user_interaction()));
    arch_diff.set_fs_root(get_parametrage().get_fs_root());
    arch_diff.set_archive_options_compare(get_parametrage().get_comparison_options());
    arch_diff.set_progressive_report(get_statistics().get_libdar_statistics());


	// restting counters and logs
    get_statistics().clear_labels();
    get_statistics().set_treated_label("item(s) identical");
    get_statistics().set_errored_label("item(s) do not match those on filesystem");
    get_statistics().set_ignored_label("item(s) ignored (excluded by filters)");
    get_statistics().set_total_label("inode(s) considered");

	// launching libdar in a separated thread
    current_thread = & arch_diff;
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
    arch_test.set_archive_path(get_parametrage().get_archive_path());
    arch_test.set_archive_basename(get_parametrage().get_archive_basename());
    arch_test.set_archive_options_read(get_parametrage().get_read_options(get_html_user_interaction()));
    arch_test.set_archive_options_test(get_parametrage().get_testing_options());
    arch_test.set_progressive_report(get_statistics().get_libdar_statistics());

	// resetting counters and logs
    get_statistics().clear_counters();
    get_statistics().clear_labels();
    get_statistics().set_treated_label("item(s) treated");
    get_statistics().set_skipped_label("item(s) excluded by filters");
    get_statistics().set_errored_label("items(s) with error");

	// launching libdar in a separated thread
    current_thread = & arch_test;
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
    arch_create.set_archive_path(get_parametrage().get_archive_path());
    arch_create.set_archive_basename(get_parametrage().get_archive_basename());
    arch_create.set_archive_extension(EXTENSION);
    arch_create.set_fs_root(get_parametrage().get_fs_root());
    arch_create.set_archive_options_create(get_parametrage().get_creating_options(get_html_user_interaction()));
    arch_create.set_progressive_report(get_statistics().get_libdar_statistics());

	// resetting counters and logs
    get_statistics().clear_counters();
    get_statistics().clear_labels();
    get_statistics().set_treated_label("item(s) treated");
    get_statistics().set_hard_links_label("hard link(s) treated");
    get_statistics().set_tooold_label("item(s) modified while read for backup (dirty files)");
    get_statistics().set_byte_amount_label("byte(s) wasted due to changing files at the time they were read");
    get_statistics().set_skipped_label("item(s) not saved (no inode/file change)");
    get_statistics().set_errored_label("items(s) with error (filesystem error)");
    get_statistics().set_ignored_label("item(s) ignored (excluded by filters)");
    get_statistics().set_deleted_label("item(s) recorded as deleted");
    get_statistics().set_ea_treated_label("item(s) with Extended Attributes");

	// launching libdar in a separated thread
    current_thread = & arch_create;
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
    arch_isolate.set_archive_path(get_parametrage().get_archive_path());
    arch_isolate.set_archive_basename(get_parametrage().get_archive_basename());
    arch_isolate.set_archive_extension(EXTENSION);
    arch_isolate.set_archive_options_isolate(get_parametrage().get_isolating_options());
    arch_isolate.set_archive_reference(
	get_parametrage().get_isolating_reference().get_archive_path(),
	get_parametrage().get_isolating_reference().get_archive_basename(),
	EXTENSION,
	get_parametrage().get_isolating_reference().get_read_options(get_html_user_interaction()));

	// resetting counters and logs
    get_statistics().clear_counters();
    get_statistics().clear_labels();

	// launching libdar in a separated thread
    current_thread = & arch_isolate;
    in_action.run_and_control_thread(current_thread);
}

void user_interface::go_merge()
{
    if(is_libdar_running())
	throw WEBDAR_BUG;

    if(current_thread != nullptr)
	throw WEBDAR_BUG;

    arch_merge.set_user_interaction(get_html_user_interaction());
    arch_merge.set_archive_path(get_parametrage().get_archive_path());
    arch_merge.set_archive_basename(get_parametrage().get_archive_basename());
    arch_merge.set_archive_extension(EXTENSION);
    arch_merge.set_archive_options_merge(get_parametrage().get_merging_options().get_options());
    arch_merge.set_archive_reference(
	get_parametrage().get_merging_reference().get_archive_path(),
	get_parametrage().get_merging_reference().get_archive_basename(),
	EXTENSION,
	get_parametrage().get_merging_reference().get_read_options(get_html_user_interaction()));
    if(get_parametrage().get_merging_options().has_auxiliary())
    {
	arch_merge.set_archive_options_auxiliary(
	    get_parametrage().get_merging_options().get_auxiliary().get_archive_path(),
	    get_parametrage().get_merging_options().get_auxiliary().get_archive_basename(),
	    EXTENSION,
	    get_parametrage().get_merging_options().get_auxiliary().get_read_options(get_html_user_interaction()));
    }
    else
	arch_merge.clear_archive_options_auxiliary();

    arch_merge.set_progressive_report(get_statistics().get_libdar_statistics());

	// resetting counters and logs
    get_statistics().clear_labels();
    get_statistics().set_treated_label("item(s) treated");
    get_statistics().set_hard_links_label("hard link(s) treated");
    get_statistics().set_ignored_label("item(s) ignored (excluded by filters)");
    get_statistics().set_deleted_label("item(s) recorded as deleted");
    get_statistics().set_ea_treated_label("item(s) with Extended Attributes");
    get_statistics().set_total_label("item(s) considered");

	// launching libdar in a separated thread
    current_thread = & arch_merge;
    in_action.run_and_control_thread(current_thread);
}

void user_interface::go_init_list()
{
    if(is_libdar_running())
	throw WEBDAR_BUG;

    if(current_thread != nullptr)
	throw WEBDAR_BUG;

    arch_init_list.set_user_interaction(get_html_user_interaction());
    arch_init_list.set_archive_path(get_parametrage().get_archive_path());
    arch_init_list.set_archive_basename(get_parametrage().get_archive_basename());
    arch_init_list.set_archive_options_read(get_parametrage().get_read_options(get_html_user_interaction()));

    	// launching libdar in a separated thread
    current_thread = & arch_init_list;
    in_action.run_and_control_thread(current_thread);
}

