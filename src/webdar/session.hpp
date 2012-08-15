#ifndef SESSION_HPP
#define SESSION_HPP

    // C system header files
extern "C"
{
#include <pthread.h>
}

    // C++ system header files
#include <list>

    // webdar headers
#include "connexion.hpp"

    /// class session - holds information about a current user session

    /// a session object stores a work under progress. An internal mutex
    /// avoid two concurrent requests to be addressed a given session object.
    /// Session object are sollicitated by parser and chooser
    /// objects.

class session : public req_ans
{
public:
	/// build a new session with default settings
    session(); //< initializes object fields and initialize mutex, record the object in the class list,

	/// destructor
    ~session(); //< closes/releases mutex and dynamically allocated fields, removes the object from the class list, eventually end the libdar work under progress

	/// inherited method from req_ans class
    answer get_answer(const request & req);

	/// returns the username of creator of the session
    std::string get_user() const { return owner; };

	/// return info about the existing sessions
    static std::vector<session *> get_existing_sessions();

private:
    bool running_job;         //< whether a libdar job is running in a separated thread
    pthread_t tid;            //< points to the libdar runing threaded job
    pthread_mutex_t obj_lock; //< control access to object's field
    std::string owner;        //< the official owner of the thread: the authenticated user that lead to this thread creation
    std::string cookie;       //< the session cookie that is mandatory to sollicitate this session (used to avoid new authentication)


	// many field will be added to contain the different options values used when building the operation

	//// static types, variables and methods /////

    static bool initialized;
    static pthread_mutex_t lock_running; //< control access to runnng_session table
    static std::list<session *> running_session; //< list of existing sessions running as separated thread

    static void add_me_in_the_list(session *obj);
    static void remove_me_from_the_list(session *obj);
};

#endif
