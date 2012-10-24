#include "session.hpp"
#include "cookies.hpp"
#include "webdar_tools.hpp"

using namespace std;

const unsigned int SESSION_ID_WIDTH = 20;


    //////////////////////////
    //  class method implementation
    //

mutex session::lock_running;
map<string, session::table> session::running_session;

unsigned int session::get_num_session()
{
    unsigned int ret = 0;

    lock_running.lock();
    try
    {
	ret = running_session.size();
    }
    catch(...)
    {
        lock_running.unlock();
	throw;
    }
    lock_running.unlock();

    return ret;
}

unsigned int session::get_num_session(const string & user)
{
    unsigned int ret = 0;
    map<string, table>::iterator it;

    lock_running.lock();
    try
    {
	for(it = running_session.begin(); it != running_session.end(); ++it)
	{
	    if(it->second.owner == user)
		++ret;
	}
    }
    catch(...)
    {
        lock_running.unlock();
	throw;
    }
    lock_running.unlock();

    return ret;
}

string session::create_new(const string & owner,
			   responder *resp)
{
    table entry;
    string sessID;
    string cookie = webdar_tools_generate_random_string(COOKIE_WIDTH);

    entry.clear();
    try
    {
	lock_running.lock();
	try
	{
		// looking whether the new session_ID is not already used
	    do
	    {
		sessID = webdar_tools_generate_random_string(SESSION_ID_WIDTH);
	    }
	    while(running_session.find(sessID) != running_session.end());

	    entry.owner = owner;
	    entry.reference = new (nothrow) session(sessID, cookie, resp);
	    if(entry.reference == NULL)
		throw exception_memory();

	    running_session[sessID] = entry;
	}
	catch(...)
	{
	    lock_running.unlock();
	    throw;
	}
	lock_running.unlock();
    }
    catch(...)
    {
	if(entry.reference != NULL)
	    delete entry.reference;
	throw;
    }

    return sessID;
}

vector<session::session_summary> session::get_summary()
{
    vector<session_summary> ret;
    map<string, table>::iterator it;
    session_summary tmp;

    lock_running.lock();
    try
    {
	it = running_session.begin();
	while(it != running_session.end())
	{
	    if(it->second.reference == NULL)
		throw WEBDAR_BUG;
	    tmp = publish(it);
	    ret.push_back(tmp);
	    ++it;
	}
    }
    catch(...)
    {
	lock_running.unlock();
	throw;
    }
    lock_running.unlock();
}

bool session::get_session_info(const std::string & session_ID, session_summary & val)
{
    bool ret = false;
    map<string, table>::iterator it;

    lock_running.lock();
    try
    {
	it = running_session.find(session_ID);
	if(it != running_session.end())
	{
	    val = publish(it);
	    ret = true;
	}
	if(val.session_ID != session_ID)
	    throw WEBDAR_BUG;
    }
    catch(...)
    {
	lock_running.unlock();
	throw;
    }
    lock_running.unlock();

    return ret;
}

session *session::acquire_session(const std::string & session_ID)
{
    session *ret = NULL;
    map<string,table>::iterator it;

    lock_running.lock();

    try
    {
	it = running_session.find(session_ID);
	if(it != running_session.end() && !(it->second.closing))
	{
	    if(it->second.reference == NULL)
		throw WEBDAR_BUG;
	    ret = it->second.reference;
	    ++(it->second.ref_given);
	}
    }
    catch(...)
    {
	lock_running.unlock();
	throw;
    }
    lock_running.unlock();

    if(ret != NULL)
    {
	ret->lock_gui.lock();
	ret->tid = pthread_self();
    }

    return ret;
}


void session::release_session(session *sess)
{
    map<string,table>::iterator it;

    if(sess == NULL)
	throw WEBDAR_BUG;

    lock_running.lock();
    try
    {
	it = running_session.begin();

	while(it != running_session.end() && it->second.reference != sess)
	    ++it;

	    // checks
	if(it == running_session.end())
	    throw WEBDAR_BUG;
	sess->check_caller();

	    // all check passed, we can proceed
	--(it->second.ref_given);
	sess->lock_gui.unlock();
	if(it->second.ref_given == 0 && it->second.closing)
	{
	    if(it->second.reference == NULL)
		throw WEBDAR_BUG;
	    if(it->second.reference->has_working_server())
		throw WEBDAR_BUG;
	    delete it->second.reference;
	    running_session.erase(it);
	}
    }
    catch(...)
    {
	lock_running.unlock();
	throw;
    }
    lock_running.unlock();
}


bool session::get_session_cookie(const std::string & session_ID, string & sesscook)
{
    bool ret = false;
    map<string, table>::const_iterator it;

    lock_running.lock();
    try
    {
	it = running_session.find(session_ID);

	if(it != running_session.end())
	{
	    if(it->second.reference == NULL)
		throw WEBDAR_BUG;
	    sesscook = it->second.reference->cookie;
	    ret = true;
	}
    }
    catch(...)
    {
	lock_running.unlock();
	throw;
    }
    lock_running.unlock();

    return ret;
}


    //////////////////////////
    //  object method implementation
    //



session::session(const string & sess_ID,
		 const string & x_cookie,
		 responder *x_resp)
{
    cookie = x_cookie;
    gui = x_resp;
    if(gui == NULL)
	throw WEBDAR_BUG;
    libdar_running = false;
    session_ID = sess_ID;
}

session::~session()
{
    if(gui != NULL)
	delete gui;
}

void session::check_caller() const
{
    if(pthread_self() != tid)
	throw WEBDAR_BUG;
}

answer session::give_answer(const request & req)
{
    check_caller();
    return gui->give_answer(req);
}


session::session_summary session::publish(std::map<std::string, table>::iterator it)
{
    session_summary ret;

    if(it->second.reference == NULL)
	throw WEBDAR_BUG;
    ret.clear();
    ret.owner = it->second.owner;
    ret.session_ID = it->first;
    ret.locked = it->second.reference->has_working_server();
    ret.libdar_running = it->second.reference->libdar_running;
    ret.closing = it->second.closing;

    return ret;
}
