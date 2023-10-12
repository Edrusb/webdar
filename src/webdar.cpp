/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2023 Denis Corbin
//
// This file is part of Webdar
//
//  Webdar is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Webdar is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Webdar.  If not, see <http://www.gnu.org/licenses/>
//
//----
//  to contact the author: dar.linux@free.fr
/*********************************************************************/

    // C system header files
extern "C"
{
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
}

    // C++ system header files
#include <iostream>
#include <vector>
#include <new>
#include <set>
#include <memory>

    // libraries header files
#include <dar/libdar.hpp>
#include <libthreadar/libthreadar.hpp>


    // webdar headers
#include "exceptions.hpp"
#include "central_report.hpp"
#include "listener.hpp"
#include "webdar_tools.hpp"
#include "server.hpp"
#include "authentication.hpp"
#include "base64.hpp"
#include "choose.hpp"
#include "static_object_library.hpp"

#define WEBDAR_EXIT_OK 0
#define WEBDAR_EXIT_SYNTAX 1
#define WEBDAR_EXIT_WEBDAR_BUG 2
#define WEBDAR_EXIT_LIBDAR_BUG 3
#define WEBDAR_EXIT_SIGT1 4
#define WEBDAR_EXIT_SIGT2 5
#define WEBDAR_EXIT_RESOURCE 6

#define DEFAULT_TCP_PORT 8008

using namespace std;

struct interface_port
{
    std::string interface; //< if empty means any interface
    unsigned int port;
    interface_port() { interface = ""; port = DEFAULT_TCP_PORT; };
};

static void parse_cmd(int argc, char *argv[],
		      vector<interface_port> & ecoute,
		      bool & verbose,
		      bool & background,
		      int & facility,
		      string & certificate,
		      string & privateK);

static void add_item_to_list(const char *optarg, vector<interface_port> & ecoute);
static void close_all_listeners(int sig);
static void libdar_init();
static void libdar_end();

    // yes, this will point to a global object, this class handle concurrent access,
    // no problem in this multi-threaded program.
    // it is necessary to have this global for signal handler able to report what they do
static shared_ptr<central_report> creport;
static vector<listener *> taches;

static void signal_handler(int x);
static string reminder_msg;
static time_t last_trigger;

int main(int argc, char *argv[], char **env)
{
    vector<interface_port> ecoute;
    bool verbose;
    bool background;
    int facility;
    int ret = WEBDAR_EXIT_OK;
    priority_t min;
    string fixed_user = "admin";
    string fixed_pass;
    string certificate;
    string privateK;
    unique_ptr<ssl_context> cipher(nullptr);

    last_trigger = time(nullptr) - 1;

    try
    {
	webdar_tools_init_randomization();
//	fixed_pass = "";
	fixed_pass = webdar_tools_generate_random_string(10);
	shared_ptr<authentication_cli> auth(new (nothrow) authentication_cli(fixed_user, fixed_pass));

	if(!auth)
	    throw exception_memory();
	    // not using std::make_share above in order
	    // to through an exception from the class tree of exception_base

	    /////////////////////////////////////////////////
	    // set signal handlers for type 1 and type 2

	set<int> signals_list;
	set<int>::iterator sl_it;

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
	    if(signal_name != nullptr)
		cout << "Adding hanlder for signal " << strsignal(*sl_it) << endl;
	    else
		cout << "Adding hanlder for signal " << *sl_it << endl;
	    if(signal(*sl_it, signal_handler) == SIG_ERR)
	    {
		if(signal_name != nullptr)
		    throw exception_system(std::string("Cannot set signal handle for ") + std::string(strsignal(*sl_it)), errno);
		else
		    throw exception_system(std::string("Cannot set signal handle for ") + std::to_string(*sl_it), errno);
	    }
	    ++sl_it;
	}


	    /////////////////////////////////////////////////
	    // analysing command-line arguments
	parse_cmd(argc, argv,
		  ecoute,
		  verbose,
		  background,
		  facility,
		  certificate,
		  privateK);


	    /////////////////////////////////////////////////
	    // creating central report object
	if(verbose)
	    min = debug;
	else
	    min = info;

	central_report* tmp = nullptr;

	if(background)
	    tmp = new (nothrow) central_report_syslog(min, "webdar", facility);
	else
	    tmp = new (nothrow) central_report_stdout(min);

	if(tmp == nullptr)
	    throw exception_memory();
	else
	    creport.reset(tmp); // tmp is now managed by the shared_ptr

	if(background)
	    throw exception_feature("background as a daemon");

	creport->report(debug, "central report object has been created");


	    /////////////////////////////////////////////////

	try
	{
	    vector<interface_port>::iterator it = ecoute.begin();
	    listener *tmp = nullptr;

	    static_object_library::init();
	    try
	    {

		libdar_init();
		server::set_max_server(10);

		try
		{

			/////////////////////////////////////////////////
			// creating and launching all threads

		    reminder_msg = string ("\n\n\tYou can now point your browser at:\n");

		    while(it != ecoute.end())
		    {
			if(! certificate.empty() && ! privateK.empty())
			{
			    cipher.reset(new (nothrow) ssl_context(certificate, privateK));
			    if(!cipher)
				throw exception_memory();
			    creport->report(info, "A new SSL context has been created");
			}

			reminder_msg += string("\t\t")
			    + (cipher ? "https://" : "http://")
			    + (it->interface == "" ? "127.0.0.1" : it->interface)
			    + string(":") + to_string(it->port) + string("\n");

			if(it->interface == "")
			    tmp = new (nothrow) listener(creport, auth, cipher, it->port);
			else
			    tmp = new (nothrow) listener(creport, auth, cipher, it->interface, it->port);
			if(tmp == nullptr)
			    throw exception_memory();
			else
			{
			    taches.push_back(tmp);
			    tmp->run();
			}

			if(cipher)
			    throw WEBDAR_BUG;

			++it;
		    }

		    creport->report(debug, "all listener threads have been launched, main thread waiting for all of them to complete");

		    reminder_msg += string("\tand use the following to authenticate:\n")
			+ string("\t\tuser name = ") + fixed_user + "\n"
			+ string("\t\tpassword  = ") + fixed_pass + "\n\n";

		    creport->report(warning, reminder_msg);

			/////////////////////////////////////////////////
			// looping while not all thread have ended

		    while(!taches.empty())
		    {
			if(taches.back() == nullptr)
			    taches.pop_back();
			else
			    if(!taches.back()->is_running())
			    {
				taches.back()->join();
				delete taches.back();
				taches.back() = nullptr;
			    }
			    else
				taches.back()->join();
		    }

		    creport->report(info, "all listener threads have ended, waiting for existing sessions to end");

			/////////////////////////////////////////////////
			// killing remaining server threads

		    server::kill_all_servers();

		    creport->report(info, "all server threads have ended");

		    choose::cleanup_memory();
		}
		catch(...)
		{
		    vector<listener *>::iterator ta = taches.begin();

		    while(ta != taches.end())
		    {
			if(*ta != nullptr)
			{
			    delete *ta;
			    *ta = nullptr;
			    ++ta;
			}
		    }
		    taches.clear();
		    throw;
		}
	    }
	    catch(...)
	    {
		static_object_library::release();
		throw;
	    }
	    static_object_library::release();
	}
	catch(...)
	{
	    creport.reset();
	    libdar_end();
	    throw;
	}
	creport.reset();
	libdar_end();
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
    catch(libthreadar::exception_thread & e)
    {
	cerr << "Uncaught exception_thread reached program boundary! " << e.get_message(": ") << endl;
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
    catch(libdar::Egeneric & e)
    {
	cerr << "LIBDAR exception caught: "<< e.get_message() << endl;
	ret = WEBDAR_EXIT_RESOURCE;
    }
    catch(...)
    {
	cerr << "Unknown non webdar exception met" << endl;
	ret = WEBDAR_EXIT_WEBDAR_BUG;
    }

    return ret;
}

static void parse_cmd(int argc, char *argv[],
		      vector<interface_port> & ecoute,
		      bool & verbose,
		      bool & background,
		      int & facility,
		      string & certificate,
		      string & privateK)
{
    int lu;
	// prevents getopt to show a message when unknown option is met
	// we will report that ourselfves:
    opterr = 0;
    verbose = false;
    background = false;
    facility = LOG_USER;
    ecoute.clear();

    while((lu = getopt(argc, argv, "vl:bC:K:h" )) != -1)
    {
	switch(lu)
	{
	case 'v':
	    verbose = true;
	    break;
	case 'l':
	    if(optarg == nullptr)
		throw exception_range("-l option needs an argument");
	    add_item_to_list(optarg, ecoute);
	    break;
	case 'b':
	    background = true;
	    break;
	case 'C':
	    certificate = optarg;
	    if(certificate.empty())
		throw exception_range("-C option needs a filename");
	    break;
	case 'K':
	    privateK = optarg;
	    if(privateK.empty())
		throw exception_range("-K option needs a filename");
	    break;
	case 'h':
	    throw exception_range(std::string("Usage: ")
				  + std::string(argv[0])
				  + std::string(" [-l <IP>[:port]] [-v] [-b] [-C <certificate file> -K <private key file>]\n"));
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
	for(int i = optind; i < argc; ++i)
	    cerr << argv[i];
	cerr << endl;
    }

    if(ecoute.size() < 1)
    {
	if(argc < 1 || argv[0] == nullptr)
	    throw WEBDAR_BUG;
	else
	    add_item_to_list((string("127.0.0.1:") + to_string(DEFAULT_TCP_PORT)).c_str(), ecoute);
   }

    if(background)
	throw exception_feature("webdar in background as a daemon");
}

static void add_item_to_list(const char *optarg, vector<interface_port> & ecoute)
{
    vector<string> coma;
    interface_port couple;

    ecoute.clear();
    webdar_tools_split_by(',', optarg, coma);

    for(vector<string>::iterator itc = coma.begin();
	itc != coma.end();
	++itc)
    {
	string m1, m2;

	if(itc->size() == 0)
	    throw exception_range(string("Error while parsing argument, empty argument provided in comma separated list"));

	webdar_tools_split_in_two(']', *itc, m1, m2);
	if(m2 == "") // either final ']' or no ']' at all
	{
	    if(*(itc->rbegin()) == ']') // there was a final ']' thus IPv6 is expected (without port specs)
	    {
		string tmp;

		webdar_tools_split_in_two('[', m1, tmp, m2);
		if(tmp.empty() && ! m2.empty()) //  full IPv6 specification
		{
		    couple.port = DEFAULT_TCP_PORT;
		    couple.interface = m2;
		}
		else
		    throw exception_range(string("Error while parsing argument \"") + *itc + "\" : unbalanced '[' and ']'");
	    }
	    else // no IPv6 provided, ':' are now only possible for port specification
	    {
		webdar_tools_split_in_two(':', *itc, m1, m2);
		if(m2 == "") // no ':' or final ':'
		{
		    if( *(itc->rbegin()) == ':')
			throw exception_range(string("Error while parsing argument \"") + *itc + "\" : unexpected ':' at end of argument");
		    else
		    {
			couple.port = DEFAULT_TCP_PORT;
			couple.interface = m1;
		    }
		}
		else // port specification provided
		{
		    try
		    {
			couple.port = webdar_tools_convert_to_int(m2);
		    }
		    catch(exception_base & e)
		    {
			throw exception_range(string("Error while parsing argument \"") + *itc + "\" : " + e.get_message());
		    }

		    if(m1.size() > 0)
			couple.interface = m1;
		    else
			throw exception_range(string("Error while parsing argument \"") + *itc + "\" : missing IP of hostname specification before ':'");
		}
	    }
	}
	else // an IPv6 was given followed by a port specification
	{
	    string tmp1, tmp2;

	    if(m1.size() < 4 || *(m1.begin()) != '[') // smallest IPv6 specs if [::]
		throw exception_range(string("Error while parsing argument \"") + *itc + "\" : invalid IPv6 specification: " + m1);

	    webdar_tools_split_in_two('[', m1, tmp1, tmp2);
	    if(! tmp1.empty())
		throw exception_range(string("unexpected string before IPv6 address in argument: ") + *itc);
	    if(tmp2.empty())
		throw exception_range(string("missing '[' before IPv6 address in argument: ") + *itc);
	    couple.interface = tmp2;

	    webdar_tools_split_in_two(':', m2, tmp1, tmp2);
	    if(! tmp1.empty() || tmp2.empty()) // extra garbage between IPv6 and ':'
		throw exception_range(string("Port specification missing after IPV6 in argument: ") + *itc);

	    try
	    {
		couple.port = webdar_tools_convert_to_int(tmp2);
	    }
	    catch(exception_base & e)
	    {
		throw exception_range(string("Error while parsing argument \"") + *itc + "\" : " + e.get_message());
	    }
	}

	ecoute.push_back(couple);
    }
}


static void close_all_listeners(int sig)
{
	/// check if a current job is running
    bool job_running = false;
    vector<session::session_summary> current = session::get_summary();
    vector<session::session_summary>::iterator it = current.begin();
    while(it != current.end() && !it->libdar_running)
	++it;
    job_running = (it != current.end());

	/// if so report to central_report that we cannot interrupt the process
	/// either connect with appropriate rights and abort the running task
    if(job_running)
	creport->report(crit, "SIGNAL RECEIVED: A libdar job is currently running, cannot stop the operation, stop the running job(s) before from web interface");
    else
    {
	creport->report(crit, "SIGNAL RECEIVED: no libdar job is currently running, aborting all currently running sessions...");
	for(vector<listener *>::iterator it = taches.begin();
	    it != taches.end();
	    ++it)
	{
	    if(*it == nullptr)
		throw WEBDAR_BUG;
	    else
		(*it)->kill();
	}
	server::kill_all_servers();

	creport->report(crit, "SIGNAL RECEIVED: no more sessions is running");
    }
}

static void libdar_init()
{
    libdar::U_I maj, med, min;

    libdar::get_version(maj, med, min);
    if(maj != libdar::LIBDAR_COMPILE_TIME_MAJOR ||
       med < libdar::LIBDAR_COMPILE_TIME_MEDIUM)
        throw libdar::Erange("initialization",
			     "we are linking against a wrong libdar");
}

static void libdar_end()
{
    libdar::close_and_clean();
}

static void signal_handler(int x)
{
    static const int delta = 2;

    time_t now = time(nullptr);

    signal(x, signal_handler);
    if(now - last_trigger < delta)
	close_all_listeners(x);
    else
    {
	creport->report(warning, reminder_msg);
	creport->report(warning, string("\n\n\t\tfor real action instead of displaying this reminder message, hit twice this key combinaison in less than ")
			+to_string(delta)+string(" second(s)\n\n"));
	last_trigger = now;
    }
}
