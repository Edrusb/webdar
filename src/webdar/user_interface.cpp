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
    case running:
	throw exception_feature("libdar running mode in user_interface");
    default:
	throw WEBDAR_BUG;
    }

    return ret;
}

void user_interface::prefix_has_changed()
{
    parametrage.set_prefix(get_prefix());
}
