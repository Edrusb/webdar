/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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
#include "my_config.h"
extern "C"
{
#if HAVE_SIGNAL_H
#include <signal.h>
#endif

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if HAVE_STRING_H
#include <string.h>
#endif

#if HAVE_ERRNO_H
#include <errno.h>
#endif

#if HAVE_SIGNAL_H
#include <signal.h>
#endif

#if HAVE_TIME_H
#include <time.h>
#endif

#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
}

/// the compiler version MACRO
#ifndef __VERSION__
#define __VERSION__ "unknown"
#endif

/// the compiler Nature MACRO
#ifdef __GNUC__
#define CC_NAT "GNUC"
#else
#define CC_NAT "unknown"
#endif

    // C++ system header files
#include <iostream>
#include <vector>
#include <new>
#include <set>
#include <memory>

    // libraries header files
#include <dar/libdar.hpp>
#include <dar/tools.hpp>
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
#include "environment.hpp"
#include "global_parameters.hpp"
#include "server_pool.hpp"

#define WEBDAR_EXIT_OK 0
#define WEBDAR_EXIT_SYNTAX 1
#define WEBDAR_EXIT_WEBDAR_BUG 2
#define WEBDAR_EXIT_LIBDAR_BUG 3
#define WEBDAR_EXIT_SIGT1 4
#define WEBDAR_EXIT_SIGT2 5
#define WEBDAR_EXIT_RESOURCE 6

#define DEFAULT_TCP_PORT 8008
#define DEFAULT_POOL_SIZE 50
#define SECURED_MEM_BYTE_SIZE 524288

    /// \mainpage
    /// The following describes the overall architecture of the webdar software
    ///
    /// The webdar main() launches:
    /// - a \ref central_report object that will gather logs from all internal components
    ///   and output them to a specified destination (command-line, syslog, or other alternatives)
    /// - an \ref authentication object to validate user provided credentials
    /// - a \ref ssl_context information that carries the necessary information to setup a server side SSL session
    /// - a \ref listener object for each IPv4 and IPv6 specified on command-line, they all receive
    ///   the a pointer to the \ref central_report and \ref authentication module as well as a copy of the
    ///   the \ref ssl_context and the IP and port to listen on.
    /// Then the main() routine is pending for all listeners to end, or a signal to come to end the
    /// webdar process.
    ///
    /// The role of a \ref listener object is to create a \ref proto_connexion for each new incoming TCP session.
    /// This proto_connexion is passed with pointers to the central_report and authentication objects to
    /// a new \ref server object. Each server object with the help of a \ref parser object transforms the TCP byte
    /// stream into a suite of HTTP \ref request, and transmit back the corresponding HTTP \ref answer.
    /// These answers are obtained from either:
    /// - a \ref static_object for static components (images, licensing text, and so on)!
    /// - a \ref challenge object if the client has not yet been authenticated
    /// - a \ref choose object if the client is authenticated but the pointed to session does not exist
    /// - the \ref user_interface object from the \ref session pointed to by the HTTP request.
    /// The user_interface object is acquired by the server object from the \ref session class.
    ///
    /// the \ref session *class* manages a list of session *objects* associated with a reference counter
    /// that keep trace of the servers that have been given the session reference (only one can interact
    /// with the session at a given time). Only when the counter drops to zero that a closing session
    /// request can be executed leading the session to be torn down. A few flags are also associated with
    /// this session object in that table (locked, running libdar, or closing).
    ///
    /// The \ref session *objects* manage a \ref user_interface object to provide the answer to each HTTP request.
    /// this is associated with a semaphore for locking and the thread_id of the \ref server object
    /// (which is from a libdar::thread inherited class) managing the session. Such server object has
    /// obtained the session management calling session::acquire_session() and has not yet called
    /// session::release_session().
    ///
    /// \ref user_interface class provides mean to setup HTTP response to HTTP request for that particular
    /// session. In other words, it handle user interaction thought the web interface. For that it
    /// relies on a set of \ref body_builder inherited classes within which, complex objects are build from simpler
    /// ones. Body builder objects are setup under a tree-like hierachy of adopted body_builder objects, with the a
    /// \ref html_page at the root. The user_interface class has
    /// several status and for each a different body_builder object is used to provide the body of the
    /// HTTP answer:
    /// - config:       object parametrage of class \ref saisie
    /// - listing_open: object in_list     of class \ref html_listing_page
    /// - listing:      object in_action   of class \ref html_libdar_running_page
    /// - running:      object in_action   of class \ref html_libdar_running_page
    /// - error:        onject in_error    of class \ref html_error
    ///
    /// most if not all \ref body_builder inherited class have a name starting with the html_* prefix
    /// Some components to behave as expected should be adopted by an \ref html_form object, these classes
    /// have a name staring with html_form_*.
    ///
    /// Pay attention that some objects are based on \ref html_form (they contain a set of html_form_ components
    /// adopted by an html_form root object and have a class name starting with html_*_form_* like
    /// html_mask_form_path or html_mask_form_filename. These are not to be adopted by an html form
    /// as they embedd an html_form object as private field.
    ///

using namespace std;

    /// struct used to store TCP port and network interface

struct interface_port
{
    string interface; //< if empty means any interface
    unsigned int port;
    interface_port() { interface = ""; port = DEFAULT_TCP_PORT; };
};

static void parse_cmd(int argc, char *argv[],
		      vector<interface_port> & ecoute,
		      bool & verbose,
		      bool & background,
		      int & facility,
		      string & certificate,
		      string & privateK,
		      unsigned int & max_srv);

static void add_item_to_list(const char *optarg, vector<interface_port> & ecoute);
static void close_all_listeners(int sig);
static void libdar_init();
static void libdar_end();
static void usage(const char* argv0);
static void show_ver();

    // yes, this will point to a global object, this class handle concurrent access,
    // no problem in this multi-threaded program.
    // it is necessary to have this global for signal handler able to report what they do
static shared_ptr<central_report> creport;
static vector<listener *> taches;
static shared_ptr<server_pool> pool;

static void signal_handler(int x);
static string reminder_msg;
static time_t last_trigger;

int main(int argc, char *argv[], char** env)
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
    unsigned int max_srv;
    unique_ptr<ssl_context> cipher(nullptr);

    last_trigger = time(nullptr) - 1;
    global_envir.feed(env);
    umask(0177); // so far the only place we create file is while writing configuration to file (config may contain passwords)

    try
    {
	webdar_tools_init_randomization();
#ifdef WEBDAR_DEV
	fixed_pass = "";
#else
	fixed_pass = webdar_tools_generate_random_string(10);
#endif
	shared_ptr<authentication_cli> auth(new (nothrow) authentication_cli(fixed_user, fixed_pass));

	if(!auth)
	    throw exception_memory();
	    // not using std::make_share above in order
	    // to through an exception from the class tree of exception_base


	    /////////////////////////////////////////////////
	    // analysing command-line arguments
	parse_cmd(argc, argv,
		  ecoute,
		  verbose,
		  background,
		  facility,
		  certificate,
		  privateK,
		  max_srv);


	    /////////////////////////////////////////////////
	    // creating central report object
	if(verbose)
	    min = debug;
	else
	    min = info;

	if(background)
	    creport.reset(new (nothrow) central_report_syslog(min, "webdar", facility));
	else
	    creport.reset(new (nothrow) central_report_stdout(min));

	if(!creport)
	    throw exception_memory();

	if(background)
	    throw exception_feature("background as a daemon");

	creport->report(debug, "central report object has been created");

	    /////////////////////////////////////////////////
	    // set signal handlers for type 1 and type 2

	libthreadar::thread_signal::change_default_signal(THREAD_SIGNAL);
	    // SIGUSR2 is used by libthread::thread_signal

	set<int> signals_list;
	set<int>::iterator sl_it;

	signals_list.insert(SIGHUP);
	signals_list.insert(SIGINT);
	signals_list.insert(SIGALRM);
	signals_list.insert(SIGTERM);
	signals_list.insert(SIGUSR1);

	sl_it = signals_list.begin();
	while(sl_it != signals_list.end())
	{
	    const char *signal_name = strsignal(*sl_it);
	    if(signal_name != nullptr)
		creport->report(debug, libdar::tools_printf("Adding hanlder for signal %s", strsignal(*sl_it)));
	    else
		creport->report(debug, libdar::tools_printf("Adding hanlder for signal %d", *sl_it));

	    if(signal(*sl_it, signal_handler) == SIG_ERR)
	    {
		if(signal_name != nullptr)
		    throw exception_system(string("Cannot set signal handle for ") + string(strsignal(*sl_it)), errno);
		else
		    throw exception_system(string("Cannot set signal handle for ") + to_string(*sl_it), errno);
	    }
	    ++sl_it;
	}

	    /////////////////////////////////////////////////
	    // creating the server_pool, managing servers
	    // which each, interact with a browser through an
	    // http/https connection.

	pool.reset(new (nothrow) server_pool(max_srv, creport));
	if(!pool)
	    throw exception_memory();

	creport->report(debug, libdar::tools_printf("A pool of %d server(s) has been created", max_srv));


	    /////////////////////////////////////////////////

	try
	{
	    vector<interface_port>::iterator it = ecoute.begin();
	    listener *tmp = nullptr;

	    static_object_library::init();
	    try
	    {

		libdar_init();

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
			    tmp = new (nothrow) listener(creport, auth, cipher, pool, it->port);
			else
			    tmp = new (nothrow) listener(creport, auth, cipher, pool, it->interface, it->port);
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
			// looping while not all (listener) thread have ended

		    while(!taches.empty())
		    {
			if(taches.back() == nullptr)
			    taches.pop_back();
			else
			    if(!taches.back()->is_running())
			    {
				taches.back()->join(); // may throw exception
				delete taches.back();
				taches.back() = nullptr;
			    }
			    else
				taches.back()->join(); // we are suspended waiting for this task to end
		    }

		    creport->report(info, "all listener threads have ended");

			/////////////////////////////////////////////////
			// killing remaining server threads

		    pool->join();
		    creport->report(info, "all server threads have ended");
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

		    pool->cancel();
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
    catch(libthreadar::exception_base & e)
    {
	cerr << "Uncaught libthreadar exception: " << e.get_message(": ") << endl;
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
		      string & privateK,
		      unsigned int & max_srv)
{
    bool default_basic_auth = true;
    int lu;
	// prevents getopt to show a message when unknown option is met
	// we will report that ourselfves:
    opterr = 0;
    verbose = false;
    background = false;
    facility = LOG_USER;
    max_srv = DEFAULT_POOL_SIZE;
    ecoute.clear();

    while((lu = getopt(argc, argv, "vl:bC:K:hm:w:V")) != -1)
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
		throw exception_range("-C option needs a file name");
	    break;
	case 'K':
	    privateK = optarg;
	    if(privateK.empty())
		throw exception_range("-K option needs a file name");
	    break;
	case 'h':
	    usage(argv[0]);
	    break;
	case '?':
	    cerr << "Ignoring Unknown argument given on command line: " << lu << endl;
	    break;
	case 'm':
	    max_srv = webdar_tools_convert_to_int(optarg);
	    break;
	case 'w':
	    if(optarg != nullptr && strncasecmp("yes", optarg, strlen("yes")) == 0)
	    {
		server::force_disconnection_at_end_of_session(false);
		html_disconnect::force_disconnection_at_end_of_session(false);
	    }
	    else if(optarg != nullptr && strncasecmp("no", optarg, strlen("no")) == 0)
	    {
		server::force_disconnection_at_end_of_session(true);
		html_disconnect::force_disconnection_at_end_of_session(true);
	    }
	    else
		throw exception_range("-w option needs \"yes\" or \"no\" as argument");
	    break;
	case 'V':
	    show_ver();
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
	creport->report(crit, "SIGNAL RECEIVED: propagating signal to all listener thread objects...");
	for(vector<listener *>::iterator it = taches.begin();
	    it != taches.end();
	    ++it)
	{
	    if(*it == nullptr)
		throw WEBDAR_BUG;
	    else
		(*it)->cancel();
	}

	creport->report(crit, "SIGNAL RECEIVED: propagating signal to all server thread objects...");

	if(pool)
	    pool->cancel();

	creport->report(crit, "SIGNAL RECEIVED: All listeners and server thread objects have been asked to end");
    }
}

static void libdar_init()
{
    libdar::U_I maj, med, min;

    libdar::get_version(maj, med, min, (libdar::U_I)(SECURED_MEM_BYTE_SIZE), true);
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

static void usage(const char* argv0)
{
    string msg = "\n";
    msg += libdar::tools_printf("Usage: %s [-l <IP>[:port]] [-v] [-b <facility>] [-w <yes|no>] [-m <num>] [-C <certificate file> -K <private key file>]\n", argv0);
    msg += libdar::tools_printf("     : %s -V\n", argv0);
    msg += libdar::tools_printf("     : %s -h\n\n", argv0);
    msg += libdar::tools_printf("  -l : IP/port webdar will listen on. Defaults to loopback IP on TCP port %d\n", DEFAULT_TCP_PORT);
    msg += libdar::tools_printf("  -v : verbose output\n");
    msg += libdar::tools_printf("  -b : webdar in background sending messages to syslog <facility> (not yet implemented)\n");
    msg += libdar::tools_printf("  -w : yes: basic auth (no disconnection from browser), no: authentication requested for each TCP session\n");
    msg += libdar::tools_printf("  -m : max number of concurrent TCP sessions (%d by default)\n", DEFAULT_POOL_SIZE);
    msg += libdar::tools_printf("  -V : shows version information and exits\n");
    msg += libdar::tools_printf("  -h : displays this short help\n");
    msg += libdar::tools_printf("  -C : certificate from the PKI to authenticate the -K-given private key\n");
    msg += libdar::tools_printf("  -K : private key to use to cipher exchanges with ssl (HTTPS sessions)\n\n");
    msg += libdar::tools_printf("Creating a self signed certificate can be achieved with:\n");
    msg += libdar::tools_printf("     openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout mycert.pem -out mycert.pem\n");
    msg += libdar::tools_printf("You can then setup webdar for HTTPS this way:\n");
    msg += libdar::tools_printf("     webdar -l 0.0.0.0 -v -C mycert.pem -K mycert.pem\n\n");

    cout << msg << endl;
    exit(1);
}

static void show_ver()
{
    string msg;
    msg += libdar::tools_printf("\nWebdar version %s, Copyright (C) 2002-2025 Denis Corbin\n", WEBDAR_VERSION);
    msg += libdar::tools_printf(gettext(" compiled the %s with %s version %s\n"), __DATE__, CC_NAT, __VERSION__);

    cout << msg << endl;
    exit(1);
}
