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
    mode = config;
    register_name(closing);
    parametrage.record_actor_on_event(this, saisie::event_closing);
}

answer user_interface::give_answer(const request & req)
{
    answer ret;

    ret.set_status(STATUS_CODE_OK);
    ret.set_reason("ok");

    switch(mode)
    {
    case config:
	ret.add_body(parametrage.get_body_part(req.get_uri().get_path(), req));
	break;
    case listing:
	throw exception_feature("libdar listing mode reached in user_interface");
    case running:
	throw exception_feature("libdar running mode reached in user_interface");
    default:
	throw WEBDAR_BUG;
    }

    return ret;
}

void user_interface::on_event(const std::string & event_name)
{
    if(event_name == saisie::event_closing)
	act(closing);
    else
	throw WEBDAR_BUG; // what's that event !?!
}

void user_interface::prefix_has_changed()
{
    parametrage.set_prefix(get_prefix());
}
