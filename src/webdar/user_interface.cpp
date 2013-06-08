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
const string user_interface::ask_end_libdar = "user_interface_ask_end_libdar";
const string user_interface::force_end_libdar = "user_interface_force_end_libdar";
const string user_interface::clean_ended_libdar = "user_interface_clean_ended_libdar";
const string user_interface::start_restore = "user_interface_start_restore";
const string user_interface::start_compare = "user_interface_start_compare";
const string user_interface::start_test = "user_interface_start_test";
const string user_interface::start_create = "user_interface_start_create";
const string user_interface::start_isolate = "user_interface_start_isolate";
const string user_interface::start_merge = "user_interface_start_merge";

user_interface::user_interface()
{
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

	/// messages received from html_libdar_running object named in_action
    in_action.record_actor_on_event(this, html_libdar_running::ask_end_libdar);
    in_action.record_actor_on_event(this, html_libdar_running::force_end_libdar);
    in_action.record_actor_on_event(this, html_libdar_running::close_libdar_screen);

	/// messages received from html_error object named in_error;
    in_error.record_actor_on_event(this, html_error::acknowledged);

	/// create the events that this object is willing to generate
    register_name(closing);
    register_name(ask_end_libdar);
    register_name(force_end_libdar);
    register_name(clean_ended_libdar);
    register_name(start_restore);
    register_name(start_compare);
    register_name(start_test);
    register_name(start_create);
    register_name(start_isolate);
    register_name(start_merge);
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
		throw exception_feature("libdar listing mode reached in user_interface");
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
	case listing:
	    act(closing); // transmetting the event
	    break;
	case running:
	    throw WEBDAR_BUG;
	case error:
	    act(closing); // transmetting the event
	    break;
	default:
	    throw WEBDAR_BUG;
	}
    }
    else if(event_name == html_libdar_running::ask_end_libdar)
    {
	switch(mode)
	{
	case config:
	    throw WEBDAR_BUG;
	case listing:
	    throw WEBDAR_BUG;
	case running:
	case error:
	    act(ask_end_libdar);  // transmit the event
	    break;
	default:
	    throw WEBDAR_BUG;
	}
    }
    else if(event_name == html_libdar_running::force_end_libdar)
    {
	switch(mode)
	{
	case config:
	    throw WEBDAR_BUG;
	case listing:
	    throw WEBDAR_BUG;
	case running:
	case error:
	    act(force_end_libdar);// transmit the event
	    break;
	default:
	    throw WEBDAR_BUG;
	}
    }
    else if(event_name == html_libdar_running::close_libdar_screen)
    {
	switch(mode)
	{
	case config:
	    throw WEBDAR_BUG;
	case listing:
	    throw WEBDAR_BUG;
	case running:
	    try
	    {
		act(clean_ended_libdar); // trigger exception rethrowing from the dead libdar thread
	    }
	    catch(...)
	    {
		mode_changed = true;
		mode = config;
		throw;
	    }
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
	    || event_name == saisie::event_merge)
    {
	if(mode != config)
	    throw WEBDAR_BUG;
	mode = running;
	mode_changed = true;
	in_action.clear();

	if(event_name == saisie::event_restore)
	    act(start_restore);
	else if(event_name == saisie::event_compare)
	    act(start_compare);
	else if(event_name == saisie::event_test)
	    act(start_test);
	else if(event_name == saisie::event_create)
	    act(start_create);
	else if(event_name == saisie::event_isolate)
	    act(start_isolate);
	else if(event_name == saisie::event_merge)
	    act(start_merge);
	else
	    throw WEBDAR_BUG;
    }
    else if(event_name == saisie::event_list)
    {
	mode = listing;
	mode_changed = true;
    }
    else if(event_name == html_error::acknowledged)
    {
	mode = return_mode;
	mode_changed = true;
    }
    else
	throw WEBDAR_BUG; // what's that event !?!
}

void user_interface::libdar_has_finished()
{
    switch(mode)
    {
    case config:
	throw WEBDAR_BUG;
    case listing:
	throw WEBDAR_BUG;
    case running:
	in_action.libdar_has_finished();
	break;
    case error:
	throw WEBDAR_BUG;
    default:
	throw WEBDAR_BUG;
    }
}

void user_interface::prefix_has_changed()
{
    parametrage.set_prefix(get_prefix());
    in_action.set_prefix(get_prefix());
    in_error.set_prefix(get_prefix());
}
