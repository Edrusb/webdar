extern "C"
{
#include <errno.h>
#include <string.h>
}

#include "exceptions.hpp"
#include "thread.hpp"

using namespace std;

thread::thread()
{
    int ret = pthread_mutex_init(&field_control, NULL);
    if(ret != 0)
	throw exception_thread(string("Error creating mutex: ") + strerror(errno));
    running = false;
    joignable = false;
}

thread::~thread()
{
    if(running)
	kill();
    if(joignable)
	join();
    if(pthread_mutex_destroy(&field_control) != 0)
	throw WEBDAR_BUG;
}

void thread::run()
{
    if(pthread_mutex_lock(&field_control) != 0)
	throw WEBDAR_BUG;
    try
    {
	if(running)
	    throw exception_thread("Cannot run thread, object already running in a sperated thread");
	if(joignable)
	    throw exception_thread("Previous thread has not been joined and possible returned exception deleted");
	if(pthread_create(&tid, NULL, run_obj, this) != 0)
	    throw exception_thread(string("Failed creating a new thread: ") + strerror(errno));
	running = true;
	joignable = true;
    }
    catch(...)
    {
	if(pthread_mutex_unlock(&field_control) != 0)
	    throw WEBDAR_BUG;
	throw;
    }
    if(pthread_mutex_unlock(&field_control) != 0)
	throw WEBDAR_BUG;
}

bool thread::is_running(pthread_t & id) const
{
    bool ret;
    pthread_mutex_t *mutex = const_cast<pthread_mutex_t *>(&field_control);

    if(pthread_mutex_lock(mutex) != 0)
	throw WEBDAR_BUG;

    ret = running;
    if(running)
	id = tid;

    if(pthread_mutex_unlock(mutex) != 0)
	throw WEBDAR_BUG;

    return ret;
}

void thread::join() const
{
    pthread_t dyn_tid;

    if(is_running(dyn_tid) || joignable)
    {
	void *returned_exception;
	int ret = pthread_join(dyn_tid, &returned_exception);

	*(const_cast<bool *>(&joignable)) = false;
	if(ret != ESRCH && ret != 0)
	    throw exception_thread(string("Failed joining thread: ") + strerror(errno));
	if(returned_exception != NULL)
	{
	    exception_base *ebase = reinterpret_cast<exception_base *>(returned_exception);
	    if(ebase == NULL)
		throw WEBDAR_BUG;
	    try
	    {
		throw_as_most_derivated_class(ebase);
	    }
	    catch(...)
	    {
		delete ebase;
		throw;
	    }
	    throw WEBDAR_BUG; // we should never leave without throwing an exception thus this statment should never be reached
	}
    }
}

void thread::kill() const
{
    pthread_t dyn_tid;

    if(is_running(dyn_tid))
    {
	int ret = pthread_cancel(dyn_tid);
	if(ret != ESRCH && ret != 0)
	    throw exception_thread(string("Failed killing thread: ") + strerror(errno));
	*(const_cast<bool *>(&running)) = false;
	join(); // necessary to manage pending exception if any
    }
}

void *thread::run_obj(void *obj)
{
    void *ret = NULL;

    try
    {
	thread *tobj = reinterpret_cast<thread *>(obj);
	if(tobj == NULL)
	    throw WEBDAR_BUG;

	    // locking and unlocking object's mutex is a simple form of barrier
	    // this way we start working only when the caller has exited run()
	if(pthread_mutex_lock(&(tobj->field_control)) != 0)
	    throw WEBDAR_BUG;
	if(pthread_mutex_unlock(&(tobj->field_control)) != 0)
	    throw WEBDAR_BUG;

	try
	{
	    tobj->inherited_run();
	}
	catch(...)
	{
		// no need to use the mutex here as only this thread has to set
		// back running to false
	    tobj->running = false;
	    throw;
	}
	tobj->running = false;

	    // no need to use mutex here neither as the thread is ending and
	    // whatever another thread reads (true or false) both will work
	    // as no exception has to be catched and delete.
	tobj->joignable = false;
    }
    catch(exception_base & e)
    {
	ret = e.clone();
    }
    catch(...)
    {
	ret = new WEBDAR_BUG;
	if(ret == NULL)
	    ret = new exception_memory();
	    // if ret is still set to NULL because of a lack of memory
	    // we cannot go further and must ignore the error
    }

    return ret;
}
