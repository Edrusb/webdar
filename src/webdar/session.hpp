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

class session
{
public:
	/// constructor of the class are left private intentionnaly
	/// this class provides a global table of session one can create lookup or destroy

    static unsigned int get_num_session();
    static void create_new(const std::string & session_ID,
			   const std::string & owner,
			   const std::string & cookie);
    static bool find(const std::string & session_ID, session & founded);
    static bool close(const std::string & session_ID);


	/// returns the username of creator of the session
    std::string get_user() const { return owner; };

	/// return info about the existing sessions
    static std::vector<session *> get_existing_sessions();

private:
    bool running_job;         //< whether a libdar job is running in a separated thread
    pthread_t tid;            //< points to the libdar runing threaded job (thread manager for that session), valid only if running_job is true
//    pthread_mutex_t obj_lock; //< control access to object's field
    std::string owner;        //< the official owner of the thread: the authenticated user that lead to this thread creation
    std::string cookie;       //< the session cookie that is mandatory to sollicitate this session (used to avoid new authentication)
    html *gui;                //< object containing the current GUI status; this object is managed by the session object

	/// build a new session with default settings
    session(); //< initializes object fields and initialize mutex, record the object in the class list,

	/// destructor
    ~session(); //< closes/releases mutex and dynamically allocated fields, removes the object from the class list, eventually end the libdar work under progress


	// many field will be added to contain the different options values used when building the operation

	//// static types, variables and methods /////

    static pthread_mutex_t lock_running; //< control access to runnng_session table
    static std::map<std::string, session *> running_session; //< list of existing sessions

    static void add_me_in_the_list(session *obj);
    static void remove_me_from_the_list(session *obj);
};

#endif
