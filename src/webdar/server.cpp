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
    while(src.get_status() == connexion::connected)
    {
	request req = src.get_next_request();
	answer ans = answer(201, "test");
	    // pass the request to object from the session table
	    // and update "ans"
	src.send_answer(ans);
    }
}
