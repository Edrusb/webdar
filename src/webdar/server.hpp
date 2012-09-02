#ifndef SERVER_HPP
#define SERVER_HPP

    // webdar headers
#include "parser.hpp"
#include "thread.hpp"
#include "central_report.hpp"

#include <list>

class server: public thread
{
public:
	// constructor & Destructor are intentionally set as private methods

    static bool run_new_server(central_report *log, connexion *source);
    static void set_max_server(unsigned int val) { max_server = val; };
    static void kill_all_servers();

protected:
    void inherited_run();

private:
    server(central_report *log, connexion *source);
    server(const server & ref): src(ref.src) { throw WEBDAR_BUG; };
    const server & operator = (const server & ref) { throw WEBDAR_BUG; };

    parser src;          //< this object manages the given connexion in constructor
    central_report *rep; //< just a pointer to an existing object, must not be deleted by "this"

	/// static fields
    static pthread_mutex_t lock_counter;  //< manages access to all static fields
    static unsigned int max_server;       //< max allowed number of concurrent thread
    static std::list<server *> instances; //< list of existing server objects
};

#endif
