#ifndef THREAD_HPP
#define THREAD_HPP

    // C system header files
extern "C"
{
#include <pthread.h>
}

// this is inspired from http://blog.emptycrate.com/node/270 But with the difference
// that thread managment is done in its own pure virtual class and arbitrary
// threaded work is done in inherited threads. Also has been added exception transmission
// support from sub threaded back to parent thread.


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

	/// the caller send a cancellation request to this object's running thread if any
    void kill() const;

protected:

	/// action to be performed in the sperated thread
    virtual void inherited_run() = 0;

	/// routines available for inherited class to avoid cancellation
	/// in critical sections of inherited_run()
    void suspend_cancellation_requests() const;
    void resume_cancellation_requests() const;

private:
    pthread_mutex_t field_control; //< mutex protecting access to object's data
    bool running;                  //< whether a thread is running
    pthread_t tid;                 //< the thread ID of the running thread if any
    bool joignable;                //< whether exist status of thread has to be retrieved
    unsigned int cancellable;      //< this field is not protected by mutex as it ougth to be modified only by the spawn thread. It allows suspend/resume cancellation requests to be re-entrant (0 = cancellation accepted)

    static void *run_obj(void *obj);  //< called by pthread_create to spawn a new thread
    static void primitive_suspend_cancellation_requests();
    static void primitive_resume_cancellation_requests();
};

#endif
