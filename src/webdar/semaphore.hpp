#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include <semaphore.h>

#include "exceptions.hpp"
#include "mutex.hpp"

    /// class semaphore is used as replacement of sem_t type in order to detect whether other
    /// thread than the one currently having the lock are waiting for the semaphore to be released

class semaphore
{
public:
    semaphore();
    semaphore(const semaphore & ref) { throw WEBDAR_BUG; };
    const semaphore & operator = (const semaphore & ref) { throw WEBDAR_BUG; };
    ~semaphore();

	/// return whether the semaphore has at least a pending thread waiting for another thread to unlock it
    bool waiting_thread() const;


	/// return whether the semaphore is locked (may it has pending thread or not)
    bool working_thread() const;

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
