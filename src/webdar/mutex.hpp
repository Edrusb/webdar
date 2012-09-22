#ifndef MUTEX_HPP
#define MUTEX_HPP

    // C system header files
extern "C"
{
#include <pthread.h>
}

    // webdar headers
#include "exceptions.hpp"

class mutex
{
public:
    mutex();
    mutex(const mutex & ref) { throw WEBDAR_BUG; };
    const mutex & operator = (const mutex & ref) { throw WEBDAR_BUG; };
    ~mutex();

	/// lock the mutex
	///
	/// \note this call puts the caller in sleep until the resource is released by
	/// another thread
    void lock();

	/// unlock the mutex
	///
	/// \note a single thread suspended on this mutex is then awaken
    void unlock();

	/// Tells whether calling lock() would currently suspend the caller or not
	///
	/// \return true if lock is acquired false if mutex is already locked
    bool try_lock();

private:
    pthread_mutex_t mut; //< the mutex
};

#endif

