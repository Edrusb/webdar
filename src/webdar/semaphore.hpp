#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include <semaphore.h>

#include "exceptions.hpp"
#include "mutex.hpp"

class semaphore
{
public:
    semaphore(unsigned int val = 1);
    semaphore(const semaphore & ref) { throw WEBDAR_BUG; };
    const semaphore & operator = (const semaphore & ref) { throw WEBDAR_BUG; };
    ~semaphore();

	/// return whether the semaphore has pending threads waiting for another thread to unlock it
    bool waiting_threads() const;

	/// request lock
	///
	/// \note this call puts the caller in sleep until the resource semaphore is released by
	/// another thread
    void lock();

	/// release lock
	///
	/// \note a single thread suspended on this semaphore is then awaken
    void unlock();

private:
    int value;       //< this is the semaphore value
    mutex val_mutex; //< this controls modification to value
    mutex semaph;    //< this is mutex is used to suspend thread when more than one is requesting the lock
};


#endif
