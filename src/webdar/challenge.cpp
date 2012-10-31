    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "cookies.hpp"
#include "base64.hpp"
#include "session.hpp"
#include "error_page.hpp"
#include "choose.hpp"
    //
#include "challenge.hpp"

using namespace std;


answer challenge::give_answer(const request & req)
{
    answer ret;
    string val;

    if(req.find_attribute(HDR_AUTHORIZATION, val))
    {
	string sp1, sp2;

	webdar_tools_split_in_two(' ', val, sp1, sp2);
	if(webdar_tools_to_canonical_case(sp1) != webdar_tools_to_canonical_case("Basic"))
	    set_answer_need_authorization(ret);
	else
	{
	    string clear = base64(sp2).decode();
	    webdar_tools_split_in_two(':', clear, sp1, sp2);

	    if(database->valid_credentials(sp1, sp2))
	    {
		string session_ID = webdar_tools_get_session_ID_from_URI(req.get_uri());
		if(session_ID == "")
		    set_answer_redirect_to_choose(ret, sp1);
		else // session_ID present
		{
		    session::session_summary info;
		    string cookie;

		    if(session_ID == "choose"
		       || !session::get_session_info(session_ID, info)
		       || info.owner != sp1
		       || info.locked
		       || !session::get_session_cookie(session_ID, cookie))
			set_answer_from_chooser_object(ret, sp1, req);
		    else
		    {
			ret = error_page(STATUS_CODE_OK, "missing cookie").give_answer(req);
			string body = string("<html><head><title>MISSING COOKIE SUPPORT</title></head>")
			    + "<body><h1>Webdar need you to accept session cookies to work</h1>"
			    + "</body></html>";
			ret.add_body(body);
		    }
		}
	    }
	    else
		set_answer_need_authorization(ret);
	}
    }
    else
	set_answer_need_authorization(ret);

    return ret;
}

void challenge::set_answer_need_authorization(answer & ans)
{
	// Request a login/password
    ans.set_status(STATUS_CODE_UNAUTHORIZED);
    ans.set_reason("login/password requested");
    ans.set_attribute(HDR_WWW_AUTHENTICATE, "Basic realm=\"/\"");
}

void challenge::set_answer_redirect_to_choose(answer & ans, const std::string & user)
{
    ans.set_status(STATUS_CODE_MOVED_TEMPORARILY);
    ans.set_reason("Which session");
    ans.set_attribute(HDR_LOCATION, "/choose");
}

void challenge::set_answer_from_chooser_object(answer & ans, const std::string & user, const request & req)
{
    ans = choose(user).give_answer(req);
}
