    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "error_page.hpp"
#include "cookies.hpp"

    //
#include "choose.hpp"

using namespace std;

answer choose::give_answer(const request & req)
{
    answer ret;
    uri base_uri = string("choose");
    uri asked = req.get_uri();
    std::vector<session::session_summary> status = session::get_summary();
    std::vector<session::session_summary>::iterator it = status.begin();
    string body = "";
    unsigned int count = 0;
    string last_session_ID;

    if(asked.size() == 2 && asked[1] == "new")
	ret = create_new_session();
    else
    {
	body += "<html><head><title>Choose a session</title></head><body>";
	body += "<table>";
	body += "<tr><td>Session ID</td><td>Locked</td><td>Running</td><td>Closing</td></tr>";
	while(it != status.end())
	{
	    if(it->owner == owner)
	    {
		body += string("<tr><td>")
		    + "<a href=\"" + (base_uri + it->session_ID).get_string() +"\">"
		    + it->session_ID + "</a></td><td>"
		    + (it->locked ? "locked" : "") + "</td><td>"
		    + (it->libdar_running ? "running" : "") + "</td><td>"
		    + (it->closing ? "closing" : "") + "</td></tr>";

		last_session_ID = it->session_ID;
		++it;
		++count;
	    }
	}
	body += "</table><p>";
	body += string("<a href=\"") + "/choose/new" + "\"> New session</a>";
	body += "</html>";

	if(count == 0)
	    ret = create_new_session();
	else
	{
	    ret.set_status(STATUS_CODE_OK);
	    ret.set_reason("ok");
	    ret.add_body(body);
	}
    }

    return ret;
}

answer choose::create_new_session()
{
    answer ret;
    uri target;
    responder *obj = new (nothrow) error_page(STATUS_CODE_OK, "This is a new session");
    if(obj == NULL)
	throw exception_memory();

    string session_ID = session::create_new(owner, obj);
    string cookie;
    if(!session::get_session_cookie(session_ID, cookie))
	throw WEBDAR_BUG;
    target += session_ID;

    ret.set_status(STATUS_CODE_MOVED_PERMANENTLY);
    ret.set_reason("new session created");
    ret.add_cookie(COOKIE_NAME_AUTH, cookie);
    ret.set_attribute(HDR_LOCATION, target.get_string());

    return ret;
}
