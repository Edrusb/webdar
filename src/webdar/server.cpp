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

pthread_mutex_t server::lock_counter = PTHREAD_MUTEX_INITIALIZER;
unsigned int server::max_server = 0;
list<server *> server::instances;

bool server::run_new_server(central_report *log, connexion *source)
{
    bool ret = false;

    if(pthread_mutex_lock(&lock_counter) != 0)
	throw WEBDAR_BUG;

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
	if(pthread_mutex_unlock(&lock_counter) != 0)
	    throw WEBDAR_BUG;
	throw;
    }

    if(pthread_mutex_unlock(&lock_counter) != 0)
	throw WEBDAR_BUG;

    return ret;
}

void server::kill_all_servers()
{
    if(pthread_mutex_lock(&lock_counter) != 0)
	throw WEBDAR_BUG;

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
	if(pthread_mutex_unlock(&lock_counter) != 0)
	    throw WEBDAR_BUG;
	throw;
    }

    if(pthread_mutex_unlock(&lock_counter) != 0)
	throw WEBDAR_BUG;
}

server::server(central_report *log, connexion *source) : src(source, log)
{
    rep = log;
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
