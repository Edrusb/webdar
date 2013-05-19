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
const string user_interface::end_libdar = "user_interface_end_libdar";
const string user_interface::start_restore = "user_interface_start_restore";
const string user_interface::start_compare = "user_interface_start_compare";
const string user_interface::start_test = "user_interface_start_test";
const string user_interface::start_create = "user_interface_start_create";
const string user_interface::start_isolate = "user_interface_start_isolate";
const string user_interface::start_merge = "user_interface_start_merge";

user_interface::user_interface():
    close_click("Close", end_libdar)
{
    mode = config;
    mode_changed = false;
    parametrage.record_actor_on_event(this, saisie::event_closing);
    parametrage.record_actor_on_event(this, saisie::event_restore);
    parametrage.record_actor_on_event(this, saisie::event_compare);
    parametrage.record_actor_on_event(this, saisie::event_test);
    parametrage.record_actor_on_event(this, saisie::event_list);
    parametrage.record_actor_on_event(this, saisie::event_create);
    parametrage.record_actor_on_event(this, saisie::event_isolate);
    parametrage.record_actor_on_event(this, saisie::event_merge);
    close_click.record_actor_on_event(this, end_libdar);

    stats.set_treated_label("Treated inode(s)");
    stats.set_skipped_label("Skipped inode(s)");
    stats.set_errored_label("Error(s) met");
    run_div.adopt(&web_ui);
    run_div.adopt(&stats);
    run_div.adopt(&close_click);

    register_name(closing);
    register_name(end_libdar);
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
	switch(mode)
	{
	case config:
	    ret.add_body(parametrage.get_body_part(req.get_uri().get_path(), req));
	    break;
	case listing:
	    throw exception_feature("libdar listing mode reached in user_interface");
	case running:
	    ret.add_body(run_div.get_body_part(req.get_uri().get_path(), req));
	    break;
	default:
	    throw WEBDAR_BUG;
	}
    }
    while(mode_changed);

    return ret;
}

void user_interface::on_event(const std::string & event_name)
{
    if(event_name == saisie::event_closing)
	act(closing);
    else if(event_name == end_libdar)
    {
	mode = config;
	mode_changed = true;
	act(end_libdar);
    }
    else if(event_name == saisie::event_restore
	    || event_name == saisie::event_compare
	    || event_name == saisie::event_test
	    || event_name == saisie::event_create
	    || event_name == saisie::event_isolate
	    || event_name == saisie::event_merge)
    {
	mode = running;
	mode_changed = true;

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
    else
	throw WEBDAR_BUG; // what's that event !?!
}

void user_interface::prefix_has_changed()
{
    parametrage.set_prefix(get_prefix());
    run_div.set_prefix(get_prefix());
}
