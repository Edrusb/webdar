#include "semaphore.hpp"

semaphore::semaphore(unsigned int val)
{
    value = (int)val;
}

semaphore::~semaphore()
{
    if(value != 1)
	throw WEBDAR_BUG;
}

bool semaphore::waiting_threads() const
{
    return value < 0;
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
    ++value;
    val_mutex.unlock();
    semaph.unlock();
}


