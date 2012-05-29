

extern "C"
{
#include <pthread.h>
}

#include <list>
#include "connexion.hpp"

    /// class session - holds information about a current user session

    /// a session is a work under progress. User may or may not be connected
    /// but no more one user can be connected at a time to a given session
    /// sessions are executed each in a separated thread. This thread aspect
    /// is managed by the class session itself. The work of a session object
    /// is to process request and build answers to the client throw a connexion
    /// object.

class session
{
public:

	/// constructor and destructors are set PRIVATE!
	/// there is no way to access objects of that class by other
	/// mean than the static members
	/// \note listeners can never release or known when a session
	/// is finished, this they cannot manage sessions objects
	/// in consequence, the management is devoted to the class itself.
	/// Listeners can only ask for a new session creation or bind
	/// a new connexion to an existing session (referred by its "ID").

	/// provides information about existing threads
    struct public_session_info
    {
	U_I session_id; //< identifies the session
	bool is_connected;       //< true if a thread is connected for that session
	bool is_running_libdar;  //< true if a libdar job is running in this session
    };

	/// return info about the existing sessions
    static vector<public_session_info> read_sessions();
    static void disconnect_session(U_I sessid);
    static void connect_session(U_I sessid, connexion *ptr);
    static void build_new_session(connexion *ptr);

private:
    connexion *tcp; //< stores the existing connexion, NULL if not connected
    pthread_mutex_t obj_lock; //< control access to object's field

	/// build a new session with default settings
    session(); //< simply initializes object fields and initialize mutex

	/// destructor
    ~session(); //< simply closes/releases mutex and dynamically allocated fields

	/// force deconnection
	///
	/// \note if no connexion exists, an exception is thrown
	/// \note when a connexion exists and the object is run in a separated
	/// thread. This call may lead to thread terminaison
	/// (depending whether a libdar job is running ... to be defined)
    void drop_current_connexion();

	/// provide a new connection
	///
	/// \param[in] ptr points to a "new"-ly allocated connexion object.
	/// \note If this call succeeds the given object remains under
	/// the responsibility of the session, it will be "deleted" when
	/// no more needed.
	/// \note if a connection is already active an exception is throw as only
	/// one connection is allowed per session.
	/// \note this call does NOT activate a new thread
    void new_connexion(connexion *ptr);

	/// method expected to be run in its own thread to build answer to analysed requested received through the connexion
    void threaded_work();


	//// static types, variables and methods /////

    struct running
    {
	unsigned int sessioni_id; //< unique number assigned to each session object
	bool threaded; //< whether it is actively running
	bool ending;   //< whether the object is pending in destructor
	pthread_t tid; //< thread ID when actively running
	session *ptr;  //< points to the object (may a thread be running or not)
    };

    static bool initialized;
    static pthread_mutex_t lock_running; //< control access to runnng_session table
    static std::list<running> running_session; //< list of existing sessions running as separated thread
    static unsigned int next_session_id; //< session ID to use for next new session object

	/// method ran by pthread_create to activate a new thread
	///
	/// \param[in] obj points to the object to run in the newly created thread
    static void *launch_thread(void *obj);

    static void go_running(session *me);
};
