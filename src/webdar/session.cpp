#include "session.hpp"

using namespace std;

    //////////////////////////
    //  class method implementation
    //

static unsigned int session::get_num_session()
{
    unsigned int ret = 0;

    lock_class();

    try
    {
	ret = running_session.size();
    }
    catch(...)
    {
	unlock_class();
	throw;
    }
    unlock_class();

    return ret;
}

static void session::create_new(const string & session_ID,
				const string & owner,
				responder *resp)
{
    lock_class();

    try
    {
	session *obj = NULL;

	try
	{
	    string session_ID = webdar_tools_generate_random_strong();
	    obj = new session(owner, resp);
	    running_session[session_ID] = obj;
	}
	catch(...)
	{
	    if(obj != NULL)
		delete obj;
	    throw;
	}
    }
    catch(...)
    {
	unlock_class();
	throw;
    }
    unlock_class();
}


static vector<session_summary> get_summary()
{
	// A IMPLEMENTER
}

static answer get_answer_for(const string & session_ID,        //< the session to interact with
				 const request & req,          //< the request to be answered
				 const string new_cookie = "") //< if set, modifies the session cookie to this new value (session holdover)
{
    answer ret;
    session *obj = NULL;

    lock_class();

    try
    {
	map<string, session *>::iterator it = running_session.find(session_ID);

	if(it != running_session.end())
	{
	    obj = it->second;
	    if(obj == NULL)
		throw WEBDAR_BUG;
	}
	    // else obj stays equal to NULL

	if(obj != NULL)
	{
	    obj->add_pending_request();
	    if(new_cookie != "")
	    {
		obj->change_cookie(new_cookie);
		    // ! MUST ALSO ADD NEW COOKIE IN THE REQUEST
	    }
	}
    }
    catch(...)
    {
	unlock_class();
	throw;
    }
    unlock_class();

    if(obj != NULL)
	ret = obj->give_answer(req);
    else // no such session found
    {
	error_responder tmp = error_responder("unknown session", STATUS_CODE_NOT_FOUND);
	ret = tmp.give_answer(req);
    }

   return ret;
}



    //////////////////////////
    //  object method implementation
    //



session::session(const string & x_user,
		 responder *x_resp): lock_this(PTHREAD_MUTEX_INITIALIZE)
{
    running_job = false;

    owner = user;
    cookie = ""; // the cookie will be defined at the time of the first answer
    gui = x_resp;
    if(gui == NULL)
	throw WEBDAR_BUG;
    closing = false;
    pending_requests = 0;
}

session::~session()
{
    if(gui != NULL)
	delete gui;
}


void lock_obj() const
{
    if(pthread_mutex_lock(&lock_this) != 0)
	throw WEBDAR_BUG;
}

void unlock_obj() const
{
    if(pthread_mutex_unlock(&lock_this) != 0)
	throw WEBDAR_BUG;
}


answer session::give_answer(const request & req)
{
    answer ret;

    lock_obj();

    try
    {
	if(pending_requests > 0)
	    --pending_requests;
	else
	    throw SRC_BUG;
	if(gui == NULL)
	    throw WEBDAR_BUG;

	if(cookie != "")
	{
		// we must check that the cookie is present in the request
	}

	ret = gui->give_answer(req);

	if(cookie == "")
	{
		// we must define a cookie, record in "this" and add it to the response
	}
    }
    catch(...)
    {
	unlock_obj();
	throw;
    }
    unlock_obj();

    return ret;
}
