    // C system header files
extern "C"
{
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
}

    // C++ system header files
#include <iostream>
#include <vector>
#include <new>


    // libraries header files
#include <libdar/libdar.hpp>


    // webdar headers
#include "exceptions.hpp"
#include "central_report.hpp"
#include "listener.hpp"
#include "webdar_tools.hpp"
#include "server.hpp"

#define WEBDAR_EXIT_OK 0
#define WEBDAR_EXIT_SYNTAX 1
#define WEBDAR_EXIT_WEBDAR_BUG 2
#define WEBDAR_EXIT_LIBDAR_BUG 3
#define WEBDAR_EXIT_SIGT1 4
#define WEBDAR_EXIT_SIGT2 5
#define WEBDAR_EXIT_RESOURCE 6

using namespace std;

struct interface_port
{
    std::string interface; //< if empty means any interface
    unsigned int port;
};

static void parse_cmd(int argc, char *argv[],
		      vector<interface_port> & ecoute, bool & verbose, bool & background, int & facility);
static void add_item_to_list(const char *optarg, vector<interface_port> & ecoute);
static void close_all_listeners(int sig);

    // yes, this will point to a global object, this class handle concurrent access,
    // no problem in this multi-threaded program.
    // it is necessary to have this global for signal handler able to report what they do
central_report *creport = NULL;

int main(int argc, char *argv[], char **env)
{
    vector<interface_port> ecoute;
    bool verbose;
    bool background;
    int facility;
    int ret = WEBDAR_EXIT_OK;
    bool quit = false;
    vector<listener *> taches;
    priority_t min;

    try
    {
	    /////////////////////////////////////////////////
	    // set signal handlers for type 1 and type 2

	set<int> signals_list;
	set<int>::iterator sl_it;
	struct sigaction sg;

	sg.sa_handler = close_all_listeners;
	if(sigemptyset(&sg.sa_mask) < 0)
	    throw exception_system("Cannot define the set of blocked signals", errno);
	sg.sa_flags = SA_RESETHAND;

	signals_list.insert(SIGHUP);
	signals_list.insert(SIGINT);
	signals_list.insert(SIGALRM);
	signals_list.insert(SIGTERM);
	signals_list.insert(SIGUSR1);
	signals_list.insert(SIGUSR2);
	sl_it = signals_list.begin();

	while(sl_it != signals_list.end())
	{
	    const char *signal_name = strsignal(*sl_it);
	    if(signal_name != NULL)
		cout << "Adding hanlder for signal " << strsignal(*sl_it) << endl;
	    else
		cout << "Adding hanlder for signal " << *sl_it << endl;
	    if(sigaction(*sl_it, &sg, NULL) < 0)
		if(signal_name != NULL)
		    throw exception_system(libdar::tools_printf("Cannot set signal handle for %s", strsignal(*sl_it)), errno);
		else
		    throw exception_system(libdar::tools_printf("Cannot set signal handle for %d", *sl_it), errno);
	    ++sl_it;
	}


	    /////////////////////////////////////////////////
	    // analysing command-line arguments
	parse_cmd(argc, argv, ecoute, verbose, background, facility);


	    /////////////////////////////////////////////////
	    // creating central report object
	if(verbose)
	    min = debug;
	else
	    min = info;

	if(background)
	{
	    creport = new (nothrow) central_report_syslog(min, "webdar", facility);
	    throw exception_feature("background as a daemon");
	}
	else
	    creport = new (nothrow) central_report_stdout(min);

	if(creport == NULL)
	    throw exception_memory();
	creport->report(debug, "central report object has been created");


	    /////////////////////////////////////////////////

	try
	{
	    vector<interface_port>::iterator it = ecoute.begin();
	    listener *tmp = NULL;
	    pthread_t unused_arg;

	    server::set_max_server(10);

	    try
	    {

		    /////////////////////////////////////////////////
		    // creating and launching all threads

		while(it != ecoute.end())
		{
		    if(it->interface == "")
			tmp = new (nothrow) listener(creport, it->port);
		    else
			tmp = new (nothrow) listener(creport, it->interface, it->port);
		    if(tmp == NULL)
			throw exception_memory();
		    else
		    {
			taches.push_back(tmp);
			tmp->run();
		    }
		    ++it;
		}
		creport->report(debug, "all listener threads have been launched, main thread waiting for all of them to complete");

		    /////////////////////////////////////////////////
		    // looping while not all thread have ended

		while(!taches.empty())
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

		creport->report(info, "all listener threads have ended, waiting for existing sessions to end");

		    /////////////////////////////////////////////////
		    // killing remaining server threads

		server::kill_all_servers();

		creport->report(info, "all listener threads have ended");
	    }
	    catch(...)
	    {
		vector<listener *>::iterator ta = taches.begin();

		while(ta != taches.end())
		{
		    if(*ta != NULL)
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
	    if(creport == NULL)
		throw WEBDAR_BUG;
	    delete creport;
	    creport = NULL;
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
    catch(exception_feature & e)
    {
	cerr << e.get_message() << endl;
	    // ret is unchanged
    }
    catch(exception_base & e)
    {
	cerr << "Unknown and unhandled webdar exception: " << endl << e.get_message() << endl;
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
	cerr << "Unknown non webdar exception met" << endl;
	ret = WEBDAR_EXIT_WEBDAR_BUG;
    }

    return ret;
}

static void parse_cmd(int argc, char *argv[],
		      vector<interface_port> & ecoute, bool & verbose, bool & background, int & facility)
{
    int lu;
	// prevents getopt to show a message when unknown option is met
	// we will report that ourselfves:
    opterr = 0;
    verbose = false;
    background = false;
    facility = LOG_USER;
    ecoute.clear();

    while((lu = getopt(argc, argv, "vl:b" )) != -1)
    {
	switch(lu)
	{
	case 'v':
	    verbose = true;
	    break;
	case 'l':
	    if(optarg == NULL)
		throw exception_range("-l option needs an argument");
	    add_item_to_list(optarg, ecoute);
	    break;
	case 'b':
	    background = true;
	    break;
	case '?':
	    cerr << "Ignoring Unknown argument given on command line: " << lu << endl;
	    break;
	default:
	    throw WEBDAR_BUG; // "known option by getopt but not known by webdar!
	}
    }

    if(optind < argc)
    {
	cerr << "Unknown options passed to " << argv[0] << ": ";
	for(unsigned int i = optind; i < argc; ++i)
	    cerr << argv[i];
	cerr << endl;
    }

    if(ecoute.size() < 1)
    {
	if(argc < 1 || argv[0] == NULL)
	    throw WEBDAR_BUG;
	else
	    throw exception_range(libdar::tools_printf("Usage: %s -l <IP:port> [-v] [-b]\n", argv[0]));
    }

    if(background)
	throw exception_feature("webdar in background as a daemon");
}

static void add_item_to_list(const char *optarg, vector<interface_port> & ecoute)
{
    const char *next = optarg;
    const char *start = optarg;
    bool seen_IP = false;
    interface_port tmp;


    while(*start != '\0')
    {
	while(*next != ':' && *next != ',' && *next != '\0')
	    ++next;

	switch(*next)
	{
	case ':':
	    if(!seen_IP)
	    {
		tmp.interface = string(start, next);
		seen_IP = true;
		++next;
		start = next;
	    }
	    else
		throw exception_range("missing IP address to listen on before ':'");
	    break;
	case ',':
	case '\0':
	    if(!seen_IP)
	    {
		ecoute.clear();
		tmp.interface = "";
	    }
	    tmp.port = webdar_tools_convert_to_int(string(start, next));
	    if(seen_IP)
	    {
		if(*next != '\0')
		    ++next;
		start = next;
	    }
	    else
		start = ""; // which will end the while loop
	    ecoute.push_back(tmp);
	    break;
	default:
	    throw WEBDAR_BUG;
	}
    }
}


static void close_all_listeners(int sig)
{
    creport->report(crit, "Signal received, action not yet implemented");

	/// check if current job is running

	/// if so report to central_report that we cannot interrupt the process
	/// either connect with appropriate rights and abort the running task

	/// else, end all listeners
	/// report to central_report that all listeners have ended, waiting for all connexion to terminate
	/// but a second signal will end directly
	/// report to all connexion about the request to shutdown
	/// prevent any libdar task to be run

	/// if no connexion nor job end immediately
}
