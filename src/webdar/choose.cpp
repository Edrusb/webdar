    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "error_page.hpp"
#include "cookies.hpp"
#include "html_table.hpp"
#include "html_page.hpp"
#include "html_url.hpp"

    //
#include "choose.hpp"

using namespace std;

answer choose::give_answer(const request & req)
{
    answer ret;
    std::vector<session::session_summary> status = session::get_summary();
    std::vector<session::session_summary>::iterator it = status.begin();
    unsigned int count = 0;
    html_page page = html_page("Choose a session");
    html_table table = html_table(4);

    table.add_cell("Session ID");
    table.add_cell("Locked");
    table.add_cell("Running");
    table.add_cell("Closing");

    while(it != status.end())
    {
	if(it->owner == owner)
	{
	    table.add_cell(html_url(it->session_ID, it->session_ID).display());
	    table.add_cell(it->locked ? "locked" : "");
	    table.add_cell(it->libdar_running ? "running" : "");
	    table.add_cell(it->closing ? "closing" : "");
	    ++count;
	}
	++it;
    }

    table.set_border(1);
    page.add_to_body(table.display());
    page.add_to_body(html_url("new", "create a new session").display());

    if(count == 0) // no existing session for that user
	ret = create_new_session();
    else
    {
	ret.set_status(STATUS_CODE_OK);
	ret.set_reason("ok");
	ret.add_body(page.display());
    }

    return ret;
}

answer choose::create_new_session()
{
    answer ret;
    html_page page = html_page("rediction to newly created session page");
    error_page *obj = new (nothrow) error_page(STATUS_CODE_OK, "This is session temporary page");
    if(obj == NULL)
	throw exception_memory();

    string session_ID = session::create_new(owner, obj);
    string cookie;
    if(!session::get_session_cookie(session_ID, cookie))
	throw WEBDAR_BUG;
    obj->set_message_body(string("This is the main page of session ") + session_ID);

    page.set_refresh_redirection(0, session_ID);
    ret.set_status(STATUS_CODE_OK);
    ret.set_reason("new session created");
    ret.add_cookie(COOKIE_NAME_AUTH, cookie);
    ret.add_body(page.display());

    return ret;
}
