#ifndef SERVER_HPP
#define SERVER_HPP

    // C++ system header files
#include <list>

    // webdar headers
#include "parser.hpp"
#include "thread.hpp"
#include "central_report.hpp"
#include "mutex.hpp"
#include "session.hpp"
#include "authentication.hpp"


class server: public thread
{
public:
	// constructor & Destructor are intentionally set as private methods

    static bool run_new_server(central_report *log, authentication *auth, connexion *source);
    static void set_max_server(unsigned int val) { max_server = val; };
    static void kill_server(pthread_t tid);
    static void kill_all_servers();
    static void throw_a_pending_exception();

	/// used by another server to ask this object to release the session it uses
    void release_session() { can_keep_session = false; }; // no need of mutex here, several concurrent call will lead to the same result.

protected:
    void inherited_run();

private:
    server(central_report *log, authentication *auth, connexion *source);
    server(const server & ref): src(ref.src) { throw WEBDAR_BUG; };
    const server & operator = (const server & ref) { throw WEBDAR_BUG; };

    parser src;              //< this object manages the given connexion in constructor
    central_report *rep;     //< just a pointer to an existing object, must not be deleted by "this"
    authentication *authsrc; //< object to consult for user authentications
    bool can_keep_session;   //< whether aonther object asked interacting with the session we use
    session *locked_session; //< the current session we use (we have acquired it mutex)

	/// static fields
    static mutex lock_counter;            //< manages access to all static fields
    static unsigned int max_server;       //< max allowed number of concurrent thread
    static std::list<server *> instances; //< list of existing server objects
};

#endif
