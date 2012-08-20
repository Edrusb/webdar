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
#include "parser.hpp"

using namespace std;

pthread_mutex_t parser::lock_counter = PTHREAD_MUTEX_INITIALIZER;
unsigned int parser::max_parser = 0;
list<parser *> parser::instances;

bool parser::run_new_parser(central_report *log, connexion *source)
{
    bool ret = false;

    if(pthread_mutex_lock(&lock_counter) != 0)
	throw WEBDAR_BUG;

    try
    {
	    ///////////
	    // cleaning up the instances list

	list<parser *>::iterator it = instances.begin();
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
	    // creating a new parser object if allowed
	    // and adding it to the list

	if(instances.size() >= max_parser && max_parser > 0)
	    ret = false;
	else
	{
	    sigset_t sigs;
	    parser *tmp = new (nothrow) parser(log, source);

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

void parser::kill_all_parsers()
{
    if(pthread_mutex_lock(&lock_counter) != 0)
	throw WEBDAR_BUG;

    try
    {
	list<parser *>::iterator it = instances.begin();
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

parser::parser(central_report *log, connexion *source)
{
    rep = log;
    src = source;
}

parser::~parser()
{
    if(src != NULL)
    {
	delete src;
	src = NULL;
    }
}

void parser::inherited_run()
{
    string tmp1 = "Tape quelque chose\n";
    string tmp2 = "HELLO WORD!";
    char buffer[100];
    unsigned int lu;

    if(src == NULL)
	throw WEBDAR_BUG;
    src->write(tmp1.c_str(), tmp1.size());
    lu = src->read(buffer, 100);
    src->write(tmp2.c_str(), tmp2.size());
    if(lu > 0)
	src->write(buffer, lu);
    else
	rep->report(warning, "could not read data from the socket");
    delete src;
    src = NULL;
}
