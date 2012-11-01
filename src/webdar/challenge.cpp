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
#include "html_page.hpp"
#include "html_text.hpp"
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
			html_page page = html_page("MISSING COOKIE SUPPORT");
			html_text text = html_text(1);

			text.add_text(true, false, true, "Webdar need you to accept session cookies to work");
			page.add_to_body(text.display());
			ret = error_page(STATUS_CODE_OK, "missing cookie").give_answer(req);
			ret.add_body(page.display());
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
    html_page page = html_page("Session Chooser");
    html_text text = html_text(1);

    page.set_background_color("rgb(221,221,221)");
    page.set_color("rgb(0,0,170)");
    text.add_text(true, false, false, "Welcome to Chooser page redirector");
    page.add_to_body(text.display());
    text.clear(0);
    text.add_text(false, false, false, "This is a temporary page");
    text.set_background_color("white");
    page.add_to_body(text.display());
    page.set_refresh_redirection(2, "/choose");

    ans.set_status(STATUS_CODE_OK);
    ans.set_reason("ok");
    ans.add_body(page.display());

}

void challenge::set_answer_from_chooser_object(answer & ans, const std::string & user, const request & req)
{
    ans = choose(user).give_answer(req);
}
