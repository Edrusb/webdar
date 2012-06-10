

// this is inspired from http://blog.emptycrate.com/node/270 But with the difference
// that thread managment is done in its own pure virtual class and arbitrary
// threaded work is done in inherited threads. Also has been added exception transmission
// support from sub threaded back to parent thread.

#include <pthread.h>

class thread
{
public:
	/// constructor
    thread();

	/// destructor
    virtual ~thread();

	/// launch the current object routing in a separated thread
    void run();

	/// checks whether the object is running in a separated thread
	///
	/// \param[out] id returns the thread_id upon success
	/// \return true if the object is running under a separated thread
	/// if false is returned, the argument is not set
    bool is_running(pthread_t & id) const;

	/// the caller will be suspended until the current object's thread ends
    void join() const;

	/// the caller explicitely kill this object's running thread
    void kill() const;

protected:

	/// action to be performed in the sperated thread
    virtual void inherited_run() = 0;

private:
    pthread_mutex_t field_control; //< mutex protecting access to object's data
    bool running;                  //< whether a thread is running
    pthread_t tid;                 //< the thread ID of the running thread if any
    bool joignable;                //< whether exist status of thread has to be retrieved

    static void *run_obj(void *obj);  //< called by pthread_create to spawn a new thread
};
