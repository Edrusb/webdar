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
#include "error_page.hpp"
#include "challenge.hpp"
#include "choose.hpp"

using namespace std;

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

void server::throw_a_pending_exception()
{
    lock_counter.lock();

    try
    {
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
    uri url;
    session *sess = NULL;
    challenge chal = authsrc;
    session::session_summary info;
    string user;


    try
    {
	while(src.get_status() == connexion::connected)
	{
	    try
	    {
		while(sess == NULL ||
		      (src.get_next_request_uri(url)
		       && webdar_tools_get_session_ID_from_URI(url) == sess->get_session_ID()))
		{
		    try
		    {
			const request & req = src.get_request();
			ans.clear();

			    // extract session info if any
			session_ID = get_session_ID_from(req);

			    // check validity of the request
			if(!chal.is_an_authoritative_request(req, user))
			    ans = chal.give_answer(req);
			else
			    if(!session::get_session_info(session_ID, info)
			       || info.locked
			       || info.owner != user)
				ans = choose(user).give_answer(req);
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

			    // send back the anwser
			src.send_answer(ans);
		    }
		    catch(exception_input & e)
		    {
			    // nothing to do;
			    // we just end normally the server thread
		    }

		} // end of the while loop

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
		src.close();
		throw;
	    }
	}
    }
    catch(exception_range & e)
    {
	rep->report(notice, string("Server thread ending: ") + e.get_message());
    }
}



static string get_session_ID_from(const request & req)
{
    return webdar_tools_get_session_ID_from_URI(req.get_uri());
}
