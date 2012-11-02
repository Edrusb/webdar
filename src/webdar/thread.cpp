
    // C system header files
extern "C"
{
#include <errno.h>
#include <string.h>
}

    // webdar headers
#include "exceptions.hpp"
#include "thread.hpp"

using namespace std;

thread::thread()
{
    running = false;
    joignable = false;
    cancellable = 0; // is cancellable at startup
    sigemptyset(&sigmask);
#ifndef THREADED
    returned = NULL;
#endif
}

thread::~thread()
{
    try
    {
	kill();
	join();
    }
    catch(exception_bug & e)
    {
	throw;
    }
    catch(...)
    {
	    // a destructor should not generate execptions
	    // such exceptions should have been generated earlier using join() if
	    // it had any importance to be taken care of
    }
}

void thread::run()
{
    thread::primitive_suspend_cancellation_requests();
    try
    {
#ifdef THREADED
	field_control.lock();
#endif
	try
	{
	    if(running)
		throw exception_thread("Cannot run thread, object already running in a sperated thread");
	    if(joignable)
		throw exception_thread("Previous thread has not been joined and possibly returned exception is deleted");
	    cancellable = 0; // should not be needed, but does not hurt
#ifdef THREADED
	    if(pthread_create(&tid, NULL, run_obj, this) != 0)
		throw exception_system("Failed creating a new thread: ", errno);
	    running = true;
#else
	    returned = run_obj(this);
	    running = false;
#endif
	    joignable = true;
	}
	catch(...)
	{
#ifdef THREADED
	    field_control.unlock();
#endif
	    throw;
	}
#ifdef THREADED
	field_control.unlock();
#endif
    }
    catch(...)
    {
	thread::primitive_resume_cancellation_requests();
	throw;
    }
    thread::primitive_resume_cancellation_requests();
}

bool thread::is_running(pthread_t & id) const
{
    bool ret;
    mutex *mut = const_cast<mutex *>(&field_control);

    if(is_running())
    {
	thread::primitive_suspend_cancellation_requests();
	try
	{
	    mut->lock();

	    ret = running;
	    if(running)
		id = tid;

	    mut->unlock();
	}
	catch(...)
	{
	    thread::primitive_resume_cancellation_requests();
	    throw;
	}
	thread::primitive_resume_cancellation_requests();

	return ret;
    }
    else
	return false;
}

void thread::join() const
{
    pthread_t dyn_tid;

    if(is_running() || joignable)
    {
	void *returned_exception;
#ifdef THREADED
	int ret = pthread_join(tid, &returned_exception);
#else
	int ret = 0;
	returned_exception = returned;
	const_cast<thread *>(this)->returned = NULL;
#endif
	*(const_cast<bool *>(&joignable)) = false;
	if(ret != ESRCH && ret != 0)
	    throw exception_system("Failed joining thread: ", errno);
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
	    throw exception_system("Failed killing thread: ", errno);
	*(const_cast<bool *>(&running)) = false;
	*(const_cast<bool *>(&joignable)) = false;
    }
}

void thread::suspend_cancellation_requests() const
{
    if(cancellable == 0)
	thread::primitive_suspend_cancellation_requests();
    ++(*(const_cast<unsigned int *>(&cancellable)));
}

void thread::resume_cancellation_requests() const
{
    if(cancellable == 0)
	throw WEBDAR_BUG;
    --(*(const_cast<unsigned int *>(&cancellable)));
    if(cancellable == 0)
	thread::primitive_resume_cancellation_requests();
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
	thread::primitive_suspend_cancellation_requests();
	tobj->field_control.lock();
	tobj->field_control.unlock();
	if(pthread_sigmask(SIG_SETMASK , &(tobj->sigmask), NULL) != 0)
	    throw exception_system("Failing setting signal mask for thread", errno);
	    //
	thread::primitive_resume_cancellation_requests();

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
    catch(...) // this statement is useless but explicitely states
    {          // that some implementations of pthread use exception
	throw; // for the thread cancellation mechanism, so we must
	       // not interfeer with this and let unknown exception
	       // to pass through, transparently
    }

    return ret;
}

void thread::primitive_suspend_cancellation_requests()
{
    int previous_state;

    if(pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &previous_state) != 0)
	throw exception_thread("unable to set cancellation state to disable");
}

void thread::primitive_resume_cancellation_requests()
{
    int previous_state;

    if(pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &previous_state) != 0)
	throw exception_thread("unable to set cancellation state to disable");
}
