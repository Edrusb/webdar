    // C system header files
extern "C"
{
#include <errno.h>
}

    // C++ system header files
#include <new>

    // webdar headers
#include "exceptions.hpp"
#include "central_report.hpp"
#include "responder.hpp"
#include "server.hpp"
#include "webdar_tools.hpp"
#include "cookies.hpp"
#include "error_page.hpp"

using namespace std;

static const string WEBDAR_COOKIE="webdar-session-id";

static string get_session_ID_from(const request & req);

mutex server::lock_counter;
unsigned int server::max_server = 0;
list<server *> server::instances;

bool server::run_new_server(central_report *log, authentication *auth, connexion *source)
{
    bool ret = false;

    lock_counter.lock();

    try
    {
	    ///////////
	    // cleaning up the instances list

	list<server *>::iterator it = instances.begin();
	while(it != instances.end())
	{
	    if(*it == NULL)
		it = instances.erase(it);
	    else
	    {
		if(!((*it)->is_running()))
		{
		    (*it)->join(); // necessary to throw exception generated from within the thread
		    delete (*it);
		    (*it) = NULL;
		    it = instances.erase(it);
		}
		else
		    ++it;
	    }
	}

	    //////////
	    // creating a new server object if allowed
	    // and adding it to the list

	if(instances.size() >= max_server && max_server > 0)
	    ret = false;
	else
	{
	    sigset_t sigs;
	    server *tmp = new (nothrow) server(log, auth, source);

	    if(tmp == NULL)
		throw exception_memory();
	    instances.push_back(tmp);

	    if(sigfillset(&sigs) != 0)
		throw exception_system("failed creating a full signal set", errno);

	    tmp->set_signal_mask(sigs);
	    tmp->run();

	    ret = true;
	}
    }
    catch(...)
    {
	lock_counter.unlock();
	throw;
    }

    lock_counter.unlock();

    return ret;
}

void server::kill_all_servers()
{
    lock_counter.lock();

    try
    {
	list<server *>::iterator it = instances.begin();
	while(it != instances.end())
	{
	    if(*it != NULL)
	    {
		delete (*it);
		(*it) = NULL;
	    }
	    it = instances.erase(it);
	}

	if(!instances.empty())
	    throw WEBDAR_BUG;
    }
    catch(...)
    {
	lock_counter.unlock();
	throw;
    }

    lock_counter.unlock();
}

server::server(central_report *log, authentication *auth, connexion *source) : src(source, log)
{
    if(log == NULL)
	throw WEBDAR_BUG;
    rep = log;
    if(auth == NULL)
	throw WEBDAR_BUG;
    authsrc = auth;
    can_keep_session = true;
}

void server::inherited_run()
{
    answer ans;
    string session_ID = "";
    string input_cookie = "";
    string expected_cookie = "";
    uri url;
    session *sess = NULL;
    bool set_cookie_to_expected_one;
    bool authenticated;


    while(src.get_status() == connexion::connected)
    {
	try
	{
	    while(sess == NULL || (src.get_next_request_uri(url) && webdar_tools_get_session_ID_from_URI(url) == sess->get_session_ID()))
	    {
		    ///////////////////////////////////////////////////////
		    // obtain request info from the parser
		    //

		const request & req = src.get_request();
		ans.clear();
		set_cookie_to_expected_one = false;
		authenticated = false;

		    // extract session info if any
		session_ID = get_session_ID_from(req);

		    // extract authentication cookie from request
		if(req.extract_cookies().find(COOKIE_NAME_AUTH, input_cookie))
		    input_cookie = "";

		    // obtain the expected session cookie from the session table
		if(session_ID == "" || !session::get_session_cookie(session_ID, expected_cookie))
		{
			// no existing session to associate that request to
			// triggering authentication request as response to this request
			// except if this is the answer to an authentication request
		    if(input_cookie != COOKIE_VAL_AUTH_ANS)
			input_cookie = COOKIE_VAL_AUTH_REQ;
		    expected_cookie = COOKIE_VAL_AUTH_ANS;
		}


		    ///////////////////////////////////////////////////////
		    // authentication was requested in the previous answer
		    //

			// if this is the answer to an authentication request
		if(input_cookie == COOKIE_VAL_AUTH_ANS)
		{
		    string user = "";
		    string pass = "";

			// <<<< extract user/pass from the received answer
			// <<<< A IMPLEMENTER
			//
		    if(!authsrc->valid_credentials(user, pass))
		    {
			error_page tmp = error_page(STATUS_CODE_UNAUTHORIZED, "wrong login or password");
			ans = tmp.give_answer(req);
		    }
		    else // authentication is correct
		    {
			if(session_ID != "")
			{
			    if(!session::get_session_cookie(session_ID, expected_cookie))
			    {
				error_page tmp = error_page(STATUS_CODE_NOT_FOUND, "unknown session");
				ans = tmp.give_answer(req);
			    }
			    else
			    {
				set_cookie_to_expected_one = true;
				authenticated = true;
			    }
			}
			else // new session requested
			{
			    if(session::get_num_session(user) > 0)
			    {
				    // before creating a new session we must check whether
				    // a chooser session does not
				    // already exist!!!!!!! <<<<<<<<<<<<<<<<<<<<<< A FAIRE
				responder *resp = new (nothrow) error_page(STATUS_CODE_OK, "Faked session chooser page");
				if(resp == NULL)
				    throw exception_memory();
				try
				{
				    session_ID = session::create_new(user, resp);
				}
				catch(...)
				{
				    if(resp != NULL)
					delete resp;
				    throw;
				}

				if(!session::get_session_cookie(session_ID, expected_cookie))
				    throw WEBDAR_BUG;
				else
				{
				    set_cookie_to_expected_one = true;
				    authenticated = true;
				}
			    }
			    else // create a new session directly
			    {
				responder *resp = new (nothrow) error_page(STATUS_CODE_OK, "Faked session initial page");
				if(resp == NULL)
				    throw exception_memory();
				try
				{
				    session_ID = session::create_new(user, resp);
				}
				catch(...)
				{
				    if(resp != NULL)
					delete resp;
				    throw;
				}

				if(!session::get_session_cookie(session_ID, expected_cookie))
				    throw WEBDAR_BUG;
				else
				{
				    set_cookie_to_expected_one = true;
				    authenticated = true;
				}
			    }
			}
		    }
		}
		else // input cookie != COOKIE_VAL_AUTH_ANS
		{
		    if(input_cookie == expected_cookie)
			authenticated = true;
		    else
		    {
			    // << request a login / password
			ans.set_status(STATUS_CODE_UNAUTHORIZED);
			ans.set_reason("login/password requested");
			ans.add_attribute(HDR_WWW_AUTHENTICATE, "Basic realm=\"/\"");
			expected_cookie = COOKIE_VAL_AUTH_ANS;
			set_cookie_to_expected_one = true;
		    }
		}

		    ///////////////////////////////////////////////////////
		    // getting answer for authenticated requests only
		    // (non authenticated got their answer already)
		    // authenticated requests are those having
		    // a cookie different than COOKIE_VAL_AUTH_REQ
		    //

		if(authenticated)
		{
		    if(session_ID == "")
			throw WEBDAR_BUG; // cannot authenticate a user without session_ID
		    else
		    {
			if(sess != NULL && sess->get_session_ID() != session_ID)
			    throw WEBDAR_BUG;
			if(sess == NULL)
			{
			    sess = session::acquire_session(session_ID);
			    if(sess == NULL)
				throw WEBDAR_BUG;
			}

			    // obtaining the answer from the session
			ans = sess->give_answer(req);
		    }
		}

		    // add the session cookie
		if(set_cookie_to_expected_one)
		    ans.add_cookie(COOKIE_NAME_AUTH, expected_cookie);

		if(ans.is_empty())
		    throw WEBDAR_BUG;

		    // send back the anwser
		src.send_answer(ans);

	    } // end of the while loop

		// release the lock for the current session
	    if(sess != NULL)
	    {
		session::release_session(sess);
		sess = NULL;
	    }
	}
	catch(exception_input & e)
	{
		// nothing to do;
		// we just end normally the server thread
	}
	catch(...)
	{
		// release the lock for the current session
	    if(sess != NULL)
	    {
		session::release_session(sess);
		sess = NULL;
	    }
	    throw;
	}
    }
}


static string get_session_ID_from(const request & req)
{
    const uri chem = req.get_uri();
    return webdar_tools_get_session_ID_from_URI(req.get_uri());
}
