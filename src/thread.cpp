/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013 Denis Corbin
//
// This file is part of Webdar
//
//  Webdar is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Webdar is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Webdar.  If not, see <http://www.gnu.org/licenses/>
//
//----
//  to contact the author: dar.linux@free.fr
/*********************************************************************/

    // C system header files
extern "C"
{
#include <errno.h>
#include <string.h>
}

    // C++ headers
#include <dar/libdar.hpp>

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
    thread *me = const_cast<thread *>(this);

    if(is_running() || joignable)
    {
	void *returned_exception;
#ifdef THREADED
	int ret = pthread_join(tid, &returned_exception);
#else
	int ret = 0;
	returned_exception = returned;
	me->returned = NULL;
#endif
	me->joignable = false;
	if(ret != ESRCH && ret != 0)
	    throw exception_system("Failed joining thread: ", errno);
	if(returned_exception != NULL && returned_exception != PTHREAD_CANCELED)
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
	thread *me = const_cast<thread *>(this);
	int ret;

	ret = pthread_cancel(dyn_tid);
	if(ret != ESRCH && ret != 0)
	    throw exception_system("Failed killing thread: ", errno);

	if(me == NULL)
	    throw WEBDAR_BUG;
#ifndef THREADED
	me->returned = NULL;
#endif
	me->running = false;
	me->joignable = false;
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
	if(ret == NULL)
	    throw;
    }
    catch(libdar::Elibcall & e)
    {
	ret = new (nothrow) exception_libcall(e);
	if(ret == NULL)
	    throw;
    }
    catch(libdar::Ebug & e)
    {
	exception_libcall *tmp = new (nothrow) exception_libcall(e);
	ret = tmp;
	if(ret == NULL)
	{
	    if(tmp != NULL)
		delete tmp;
	    throw;
	}
	tmp->change_message(e.dump_str());
    }
    catch(libdar::Egeneric & e)
    {
	ret = new (nothrow) exception_range(string("LIBDAR error: " + e.get_message()));
	if(ret == NULL)
	    throw;
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
