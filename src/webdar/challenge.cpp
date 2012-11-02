    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "base64.hpp"
#include "session.hpp"
#include "error_page.hpp"
    //
#include "challenge.hpp"

using namespace std;

bool challenge::is_an_authoritative_request(const request & req, string & user)
{
    bool ret = false;
    string val;

    if(req.find_attribute(HDR_AUTHORIZATION, val))
    {
	string sp1, sp2;

	webdar_tools_split_in_two(' ', val, sp1, sp2);
	if(webdar_tools_to_canonical_case(sp1) == webdar_tools_to_canonical_case("Basic"))
	{
	    string clear = base64(sp2).decode();
	    webdar_tools_split_in_two(':', clear, sp1, sp2);

	    if(database->valid_credentials(sp1, sp2))
	    {
		user = sp1;
		ret = true;
	    }
	}
    }

    return ret;
}


answer challenge::give_answer(const request & req)
{
    answer ret;

	// Request a login/password
    ret.set_status(STATUS_CODE_UNAUTHORIZED);
    ret.set_reason("login/password requested");
    ret.set_attribute(HDR_WWW_AUTHENTICATE, "Basic realm=\"/\"");

    return ret;
}


