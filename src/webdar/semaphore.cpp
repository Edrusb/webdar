#include "semaphore.hpp"

semaphore::semaphore()
{
    value = 1;
}

semaphore::~semaphore()
{
    if(value != 1)
	throw WEBDAR_BUG;
}

bool semaphore::waiting_thread() const
{
    return value < 0; // reading of integer is atomic CPU single operation, no need to lock val_mutex
}

bool semaphore::working_thread() const
{
    return value < 1; // reading of integer is atomic CPU single operation, no need to lock val_mutex
}

void semaphore::lock()
{
    val_mutex.lock();
    --value;
    val_mutex.unlock();
    semaph.lock();
}

void semaphore::unlock()
{
    val_mutex.lock();
    try
    {
	if(value == 1)
	    throw WEBDAR_BUG;
	++value;
    }
    catch(...)
    {
	val_mutex.unlock();
	throw;
    }
    val_mutex.unlock();
    semaph.unlock();
}

void semaphore::reset()
{
    val_mutex.lock();
    try
    {
	while(value < 1)
	{
	    semaph.unlock();
	    ++value;
	}
    }
    catch(...)
    {
	val_mutex.unlock();
	throw;
    }
    val_mutex.unlock();
}




