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

#ifndef THREAD_HPP
#define THREAD_HPP

    /// \file thread.hpp
    /// \brief holds the definition of the thread class
    ///
    /// this is inspired from http://blog.emptycrate.com/node/270 But with the difference
    /// that thread managment is done in its own pure virtual class and arbitrary
    /// threaded work is done in inherited classes. Also has been added exception transmission
    /// support from sub threaded back to parent thread using the join() method

    // C system header files
extern "C"
{
#include <pthread.h>
#include <signal.h>
}

    // webdar headers
#include "mutex.hpp"


    // if the following #define is commented out, the class thread implementation does
    // not use threads, this may be useful for debugging purposes
#define THREADED

class thread
{
public:
	/// constructor
    thread();

	/// destructor
    virtual ~thread();

	/// set signal mask for this object's next thread to be run
    void set_signal_mask(const sigset_t & mask) { sigmask = mask; };

	/// launch the current object routing in a separated thread
    void run();

	/// checks whether the object is running in a separated thread
	///
	/// \param[out] id returns the thread_id upon success
	/// \return true if the object is running under a separated thread
	/// if false is returned, the argument is not set
    bool is_running(pthread_t & id) const;
    bool is_running() const { return running; };

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
    mutex field_control;           //< mutex protecting access to object's data
    bool running;                  //< whether a thread is running
    pthread_t tid;                 //< the thread ID of the running thread if any
    bool joignable;                //< whether exist status of thread has to be retrieved
    unsigned int cancellable;      //< this field is not protected by mutex as it ougth to be modified only by the spawn thread. It allows suspend/resume cancellation requests to be re-entrant (0 = cancellation accepted)
    sigset_t sigmask;              //< signal mask to use for the thread
#ifndef THREADED
    void *returned;                //< to workaround the non use of join() when threading is disabled for debugging
#endif
	// static members

    static void *run_obj(void *obj);  //< called by pthread_create to spawn a new thread
    static void primitive_suspend_cancellation_requests();
    static void primitive_resume_cancellation_requests();
};

#endif
