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
const string user_interface::close_libdar_screen = "user_interface_close_libdar_screen";
const string user_interface::start_restore = "user_interface_start_restore";
const string user_interface::start_compare = "user_interface_start_compare";
const string user_interface::start_test = "user_interface_start_test";
const string user_interface::start_create = "user_interface_start_create";
const string user_interface::start_isolate = "user_interface_start_isolate";
const string user_interface::start_merge = "user_interface_start_merge";

user_interface::user_interface():
    ask_close("Stop libdar thread", ask_end_libdar),
    force_close("Abord libdar thread", force_end_libdar),
    finish("Finish", close_libdar_screen)
{
    mode = config;
    mode_changed = false;
    close_requested = false;

    parametrage.record_actor_on_event(this, saisie::event_closing);
    parametrage.record_actor_on_event(this, saisie::event_restore);
    parametrage.record_actor_on_event(this, saisie::event_compare);
    parametrage.record_actor_on_event(this, saisie::event_test);
    parametrage.record_actor_on_event(this, saisie::event_list);
    parametrage.record_actor_on_event(this, saisie::event_create);
    parametrage.record_actor_on_event(this, saisie::event_isolate);
    parametrage.record_actor_on_event(this, saisie::event_merge);

    ask_close.record_actor_on_event(this, ask_end_libdar);
    force_close.record_actor_on_event(this, force_end_libdar);
    finish.record_actor_on_event(this, close_libdar_screen);

    errors.css_color("0xFF0000");
    errors.css_margin("1em");
    errors.css_font_weight_bold();
    errors.css_text_align(css::al_center);

    run_div.adopt(&web_ui);
    run_div.adopt(&stats);
    run_div.adopt(&ask_close);
    run_div.adopt(&force_close);
    run_div.adopt(&errors);
    run_div.adopt(&finish);


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
	switch(mode)
	{
	case config:
	    ret.add_body(parametrage.get_body_part(req.get_uri().get_path(), req));
	    break;
	case listing:
	    throw exception_feature("libdar listing mode reached in user_interface");
	case running:
	case end_asked:
	case end_forced:
	case finished:
	    ret.add_body(run_div.get_body_part(req.get_uri().get_path(), req));
	    break;
	case exceptions:
	    try
	    {
		act(clean_ended_libdar);
	    }
	    catch(libdar::Egeneric & e)
	    {
		errors.clear();
		errors.set_visible(true);
		errors.add_text(1, "LIBDAR error");
		errors.add_paragraph();
		errors.add_text(0, e.get_message());
	    }
	    mode = finished;
	    mode_changed = true;
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
    else if(event_name == ask_end_libdar)
    {
	switch(mode)
	{
	case config:
	    throw WEBDAR_BUG;
	case listing:
	    throw WEBDAR_BUG;
	case running:
	    mode = end_asked;
	    act(ask_end_libdar);
	    finish.set_visible(false);
	    force_close.set_visible(true);
	    ask_close.set_visible(false);
	    break;
	case end_asked:
	    break;
	case end_forced:
	    throw WEBDAR_BUG;
	case exceptions:
	case finished:
	    break;
	default:
	    throw WEBDAR_BUG;
	}

	mode_changed = true;
    }
    else if(event_name == force_end_libdar)
    {
	switch(mode)
	{
	case config:
	    throw WEBDAR_BUG;
	case listing:
	    throw WEBDAR_BUG;
	case running:
	    throw WEBDAR_BUG;
	case end_asked:
	    mode = end_forced;
	    act(force_end_libdar);
	    finish.set_visible(false);
	    force_close.set_visible(false);
	    ask_close.set_visible(false);
	    break;
	case end_forced:
	case exceptions:
	case finished:
	    break;
	default:
	    throw WEBDAR_BUG;
	}

	mode_changed = true;
    }
    else if(event_name == close_libdar_screen)
    {
	switch(mode)
	{
	case config:
	    throw WEBDAR_BUG;
	case listing:
	    throw WEBDAR_BUG;
	case running:
	    throw WEBDAR_BUG;
	case end_asked:
	    throw WEBDAR_BUG;
	case end_forced:
	    throw WEBDAR_BUG;
	case exceptions:
	    throw WEBDAR_BUG;
	case finished:
	    mode = config;
	    mode_changed = true;
	    break;
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
	finish.set_visible(false);
	force_close.set_visible(false);
	ask_close.set_visible(true);
	errors.set_visible(false);

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

void user_interface::libdar_has_finished()
{
    mode = exceptions;
    finish.set_visible(true);
    force_close.set_visible(false);
    ask_close.set_visible(false);
}

void user_interface::prefix_has_changed()
{
    parametrage.set_prefix(get_prefix());
    run_div.set_prefix(get_prefix());
}
