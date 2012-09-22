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


    /// class session - holds information about a current user session

    /// a session object stores a work under progress. An internal mutex
    /// avoid two concurrent requests to be addressed a given session object.
    /// Session object are sollicitated by parser and chooser
    /// objects.

class session
{
public:
    struct session_summary
    {
	std::string user;
	std::string session_ID;
	bool attached;
    };

	// constructor of the class are left private intentionnaly
	// this class provides a global table of session one can create, lookup or destroy objects in/from this table

	//////////////////////////
	// Class methods
	//

    static unsigned int get_num_session();
    static void create_new(const std::string & session_ID,
			   const std::string & owner,
			   responder *resp);
    static std::vector<session_summary> get_summary();
    static answer get_answer_for(const std::string & session_ID,     //< the session to interact with
				 const request & req,                //< the request to be answered
				 const std::string new_cookie = ""); //< if set, modifies the session cookie to this new value (session holdover)

private:
	/// constructor
    session(const std::string & x_user,
	    responder *x_resp);

	/// destructor
    ~session(); //< free dynamically allocated fields


    mutex lock_this;          //< control the access to objects fields
    std::string owner;        //< the official owner of the thread: the authenticated user that lead to this thread creation
    std::string cookie;       //< the session cookie that is mandatory to sollicitate this session (used to avoid new authentication)
    responder *gui;           //< object containing the current GUI status; this object is managed by the session object; should never be NULL
    bool closing;             //< when closing the object must not be given for new request, only pending requests can be honored
    unsigned int pending_requests; //< how much requests are waiting in the pipe(s), a session object must not be destroyed if this number is greater than zero

    void lock_obj() const;
    void unlock_obj() const;
    answer give_answer(const request & req);
    bool is_closing() const { return closing; };
    void add_pending_request() { lock_obj(); ++pending_requests; unlock_obj(); };
    const std::string get_owner() const { return owner; };
    void change_cookie(const std::string & new_cookie) { lock_obj(); cookie = new_cookie; unlock_obj(); };


	////////////////
	// static types, variables and methods
	//

    static pthread_mutex_t lock_running;                     //< control access to runnng_session static table
    static std::map<std::string, session *> running_session; //< list of existing sessions

    static void add_in_the_list(session *obj);
    static void remove_from_the_list(session *obj);

    static void lock_class();
    static void unlock_class();
};

#endif
