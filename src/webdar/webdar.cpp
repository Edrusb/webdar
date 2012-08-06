#include <iostream>
#include <signal.h>
#include <libdar/libdar.hpp>

#include "exceptions.hpp"
#include "central_report.hpp"

#define WEBDAR_EXIT_OK 0
#define WEBDAR_EXIT_SYNTAX 1
#define WEBDAR_EXIT_WEBDAR_BUG 2
#define WEBDAR_EXIT_LIBDAR_BUG 3
#define WEBDAR_EXIT_SIGT1 4
#define WEBDAR_EXIT_SIGT2 5
#define WEBDAR_EXIT_RESOURCE 6

struct interface_port
{
    std::string interface; //< if empty means any interface
    U_I port;
};

static void parse_cmd(int argc, char *argv[],
		      vector<interface_port> & ecoute, bool & verbose, bool & background, int & facility);

int main(int argc, char *argv[], char **env)
{
    vector<interface_port> ecoute;
    bool verbose;
    bool background;
    int facility;
    central_report *report = NULL;
    int ret = WEBDAR_EXIT_OK;
    bool quit = false;
    vector<listener *> taches;
    priority_t min;

    try
    {
	    /////////////////////////////////////////////////
	    // set signal handlers for type 1 and type 2



	    /////////////////////////////////////////////////
	    // analysing command-line arguments
	parse_cmd(argc, argv, ecoute, verbose, background, facility);


	    /////////////////////////////////////////////////
	    // creating central report object
	if(verbose)
	    min = notice;
	else
	    min = info;

	if(background)
	{
	    report = new central_report_syslog(min, "webdar", facility);
	    throw exception_feature("background as a daemon");
	}
	else
	    report = new central_report_stdout(min);

	if(report == NULL)
	    throw exception_memory();


	    /////////////////////////////////////////////////

	try
	{
	    vector<interface_port>::iterator it = ecoute.begin();
	    listener *tmp = NULL;
	    pthread_t unused_arg;

	    try
	    {

		    /////////////////////////////////////////////////
		    // creating and launching all threads

		while(it != ecoute.end())
		{
		    if(it->interface == "")
			tmp = new listener(it->port);
		    else
			tmp = new listener(it->interface, it->port);
		    if(tmp == NULL)
			throw exception_memory();
		    else
		    {
			taches.push_back(tmp);
			tmp->run();
		    }
		    ++it;
		}


		    /////////////////////////////////////////////////
		    // looping while not all thread have ended

		while(!taches.is_empty())
		{
		    if(taches.back() == NULL)
			taches.pop_back();
		    else
			if(!taches.back()->is_running(unused_arg))
			{
			    delete taches.back();
			    taches.back() = NULL;
			}
			else
			    taches.back()->join();
		}

	    }
	    catch(...)
	    {
		vector<listener *>::iterator ta = taches.begin();

		while(ta != taches.end())
		{
		    if(ta != NULL)
		    {
			delete *ta;
			*ta = NULL;
			++ta;
		    }
		}
		taches.clear();
		throw;
	    }

	}
	catch(...)
	{
	    if(report == NULL)
		throw WEBDAR_BUG;
	    delete report;
	    report = NULL;
	    throw;
	}

    }
    catch(exception_memory & e)
    {
	cerr << "Memory allocation failed, cannot get the necessary memory from the system in order to provided the expected service" << endl;
	ret = WEBDAR_EXIT_RESOURCE;
    }
    catch(exception_bug & e)
    {
	cerr << "Webdar met an internal condition flagged as a bug: please report the following output to maintainer: " << e.get_message() << endl;
	ret = WEBDAR_EXIT_WEBDAR_BUG;
    }
    catch(exception_thread & e)
    {
	cerr << "Uncaught exception_thread reached program boundary! " << e.get_message() << endl;
	ret = WEBDAR_EXIT_WEBDAR_BUG;
    }
    catch(exception_system & e)
    {
	cerr << "Cannot proceed to the requested action, operating system reported: " << endl;
	cerr << e.get_message() << endl;
	ret = WEBDAR_EXIT_RESOURCE;
    }
    catch(exception_range & e)
    {
	cerr << "Out of range value or parameter: " << e.get_message() << endl;
	ret = WEBDAR_EXIT_WEBDAR_BUG; // if this exception is not caught at this level, this is a bug
    }
    catch(execption_feautr & e)
    {
	cerr << e.get_message() << endl;
	    // ret is unchanged
    }
    catch(exception_base & e)
    {
	cerr << "Unknown and unhandled webdar exception: " << endl << e.get_message(); << endl;
	ret = WEBDAR_EXIT_WEBDAR_BUG;
    }
    catch(libdar::Ebug & e)
    {
	cerr << "Libdar bug met: " << endl;
	cerr << e.get_message() << endl;
	ret = WEBDAR_EXIT_LIBDAR_BUG;
    }
    catch(...)
    {
	cerr << "Unknown non webdar exception met: " << typeid(e) << endl;
	ret = WEBDAR_EXIT_WEBDAR_BUG;
    }

    return ret;
}

