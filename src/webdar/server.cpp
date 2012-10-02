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
#include "req_ans.hpp"
#include "server.hpp"

using namespace std;

static const string WEBDAR_COOKIE="webdar-session-id";

static string get_session_ID_from(const request & req);

mutex server::lock_counter;
unsigned int server::max_server = 0;
list<server *> server::instances;

bool server::run_new_server(central_report *log, connexion *source)
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
	    server *tmp = new (nothrow) server(log, source);

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

server::server(central_report *log, connexion *source) : src(source, log)
{
    rep = log;
    can_keep_session = true;
}

void server::inherited_run()
{
    request req;
    answer ans;
    string session_ID = "";
    string input_cookie = "";
    string expected_cookie = "";
    session *sess = NULL;
    bool authenticated;

    while(src.get_status() == connexion::connected)
    {
	try
	{
	    while(sess == NULL || src.has_pending_request())
	    {

		ans.clear();
		req.clear();

		    ///////////////////////////////////////////////////////
		    // obtain request info from the parser
		    //

		req = src.get_next_request();

		    // extract session info if any
		session_ID = get_session_ID_from(req);

			// extract cookie from request
		if(!req.extract_cookies().find(COOKIE_NAME, input_cookie))
		    input_cookie = "";

		if(session_ID == "" || !session::get_session_cookie(session_ID, expected_cookie))
		{
		    if(input_cookie != COOKIE_VAL_AUTH_ANS)
			input_cookie = COOKIE_AUTH_REQ;
		}

		    ///////////////////////////////////////////////////////
		    // authentication verification
		    //

		if(input_cookie == COOKIE_AUTH_ANSW)
			// this is the answer to an authentication request
		{
		    string user = "";

			// check whether user authentication is correct
		    if(!authentication_is_correct)   //<<<< A DEFINIR
		    {
			ans = error_page("wrong login or password"); //<<<< A DEFINIR
			expected_cookie = "";
			    // input_cookie stays equal to COOKIE_AUTH_ANSW
		    }
		    else // authentication is correct
		    {
			if(session_ID != "")
			{
			    if(!session::get_session_cookie(session_ID, expected_cookie))
			    {
				ans = error_page("unknown session"); // <<< A DEFINIR
				exptected_cookie = "";
				    // input_cookie stays equal to COOKIE_AUTH_ANSW
			    }
			    else
				input_cookie = "";
			}
			else // new session requested
			{
			    if(session::get_num_session(user) > 0)
			    {
				ans = session_list_page(); // <<< A DEFINIR
				expected_cookie = COOKIE_CHOOSE_SESSION;
				    // cookie stays equal to COOKIE_AUTH_ANSW
			    }
			    else // create a new session directly
			    {
				session_ID = session::create_new(user, error_page("Faked session initial page")); //<<< A DEFINIR
				if(!session::get_session_cookie(session_ID, expected_cookie))
				    throw WEBDAR_BUG;
				cookie = "";
			    }
			}
		    }
		}


		    ///////////////////////////////////////////////////////
		    // authentication request
		    //

		if(input_cookie == COOKIE_AUTH_REQ)
		{
		    ans = ; /// A DEFINIR
		    expected_cookie = COOKIE_AUTH_ANSW;
		}

		    ///////////////////////////////////////////////////////
		    // getting answer for authenticated requests
		    // authenticated requests are those having a cookie different than COOKIE_AUTH
		    //

		if(input_cookie != COOKIE_AUTH_ANSW && input_cookie != COOKIE_AUTH_ANSW)
		{
		    if(session_ID == "")
		    {
			    // request user authentication
			ans = ; // <<<<<<<<<<<<<<< A DEFINIR
			    //
			cookie = ""; // this will set the COOKIE_AUTH in the answer later on
			expected_cookie = COOKIE_AUTH_ANSW;
		    }
		    else // asked for a particular session
		    {
			if(cookie != expected_cookie)
			{
			    ans = error_page("unauthorized access");
			    expected_cookie = "";
			}
			else // we are deemed authenticated for that session
			{
			    if(sess != NULL && sess->get_session_ID() != session_ID)
			    {
				session::release_session(sess);
				sess = NULL;
			    }
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
		}

		    // add the session cookie (expected_cookie, si cookie != expected_cookie)
		if(cookie != expected_cookie)
		    ans.add_cookie(WEDAR_COOKIE, expected_cookie);

		    // send back the anwser
		src.send_answer(ans);
	    }

		// release the lock for the current session
	    if(sess != NULL)
	    {
		session::release_session(sess);
		sess = NULL;
	    }


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
    string ret = "";

    const uri chem = req.get_uri();
    if(chem.size() > 2)
	ret = chem[3];

    return ret;
}
