    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "error_page.hpp"
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
    html_url url = html_url("/choose/new", "create a new session");

    if(req.get_uri().get_path().size() > 2 && req.get_uri().get_path()[0] == "choose" && req.get_uri().get_path()[1] == "new")
	ret = create_new_session(req);
    else
    {
	table.give("Session ID");
	table.give("Locked");
	table.give("Running");
	table.give("Closing");

	while(it != status.end())
	{
	    if(it->owner == owner)
	    {
		table.give(html_url(string("/") + it->session_ID, it->session_ID).get_body_part());
		table.give(it->locked ? "locked" : "");
		table.give(it->libdar_running ? "running" : "");
		table.give(it->closing ? "closing" : "");
		++count;
	    }
	    ++it;
	}

	if(count == 0) // no existing session for that user
	    ret = create_new_session(req);
	else
	{
	    page.give(& table);
	    page.give(& url);

	    try
	    {
		table.set_border(1);

		ret.set_status(STATUS_CODE_OK);
		ret.set_reason("ok");
		ret.add_body(page.get_body_part(chemin("/"), req));
	    }
	    catch(...)
	    {
		page.take_back(& table);
		page.take_back(& url);
		throw;
	    }

	    page.take_back(& table);
	    page.take_back(& url);

	}
    }

    return ret;
}

answer choose::create_new_session(const request & req)
{
    answer ret;

    html_page page = html_page("rediction to newly created session page");
    error_page *obj = new (nothrow) error_page(STATUS_CODE_OK, "This is session temporary page");
    if(obj == NULL)
	throw exception_memory();

    string session_ID = session::create_new(owner, obj);
    obj->set_message_body(string("This is the main page of session ") + session_ID);
    obj->set_prefix(chemin(session_ID));
    obj->set_return_uri(uri("/"), "Root Page");

    page.set_refresh_redirection(0, session_ID);
    ret.set_status(STATUS_CODE_OK);
    ret.set_reason("new session created");
    ret.add_body(page.get_body_part(chemin("/"), req));

    return ret;
}
