#ifndef SESSION_HPP
#define SESSION_HPP

    // C system header files
extern "C"
{
}

    // C++ system header files
#include <list>

    // webdar headers
#include "req_ans.hpp"
#include "semaphore.hpp"

    /// class session - holds information about a current user session

    /// a session object stores a work under progress. An internal mutex
    /// avoid two concurrent requests to be addressed a given session object.
    /// Session object are sollicitated by parser and chooser
    /// objects.

class session
{
public:

	//////////////////////////
	// Object methods
	//

	// constructor of the class are left private intentionnaly
	// this class provides a global table of session one can create, lookup or destroy objects in/from this table

    answer give_answer(const request & req);
    const std::string get_owner() const { return owner; };
    void set_cookie(const std::string & cook) { cookie = cook; };
    const std::string get_cookie() const { return cookie; };
    bool has_waiting_threads() const { return lock_gui.waiting_thread(); };
    bool has_working_server() const { return lock_gui.working_thread(); };
    std::string get_session_ID() const { return session_ID; };

	//////////////////////////
	// Class types and methods
	//

    struct session_summary
    {
	std::string user;
	std::string session_ID;
	bool locked;
	bool libdar_running;
	void clear() { user = session_ID = ""; locked = libdar_running = false; };
    };

    static unsigned int get_num_session();
    static unsigned int get_num_session(const std::string & user);
    static std::vector<session_summary> get_summary();
    static std::string create_new(const std::string & owner,
				  responder *resp); /// returns the session_ID of the newly created session
    static session *acquire_session(const std::string & session_ID);
    static void release_session(session *sess);
    static bool get_session_cookie(const std::string & session_ID, string & sesscook);

private:
	/// constructor
    session(const std::string & sess_ID,
	    const std::string & x_user,
	    const std::string & cookie,
	    responder *x_resp);

	/// destructor
    ~session(); //< free dynamically allocated fields


    std::string owner;        //< the official owner of the thread: the authenticated user that lead to this thread creation
    std::string cookie;       //< the session cookie if any
    semaphore lock_gui;       //< required locking before accessing gui field
    responder *gui;           //< object containing the current GUI status; this object is managed by the session object; should never be NULL
    pthread_t tid;            //< holds the tid of the thread that acquired the object
    bool libdar_running;      //< unused for now
    std::string session_ID;   //< session_ID info (duplicated info to avoid table lookup and mutex lock)

    void check_caller() const;//< test whether the caller has properly acquired the lock on this object


	////////////////
	// static types, variables and methods
	//

    struct table
    {
	session *reference;     //< object reference
	unsigned int ref_given; //< number of time the reference to that object has been given
	bool closing;           //< if true the reference must not be given any longer
	clear(){ reference = NULL; ref_given = 0; closing = false; };
    };

    static mutex lock_running;                               //< control access to runnng_session static table
    static std::map<std::string, table> running_session;     //< list of existing sessions
};

#endif
