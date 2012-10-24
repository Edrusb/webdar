    // C system header files
extern "C"
{
#include <errno.h>
}

    // webdar headers
#include "mutex.hpp"

mutex::mutex()
{
    int ret = pthread_mutex_init(&mut, NULL);
    if(ret != 0)
	throw exception_system("Error while creating mutex", ret);
}

mutex::~mutex()
{
    (void)pthread_mutex_destroy(&mut);
}

void mutex::lock()
{
    switch(pthread_mutex_lock(&mut))
    {
    case 0:
	break;
    case EINVAL:
	throw WEBDAR_BUG;
    case EDEADLK:
	throw WEBDAR_BUG;
    case EPERM:
	throw WEBDAR_BUG;
    default:
	throw WEBDAR_BUG;
    }
}

void mutex::unlock()
{
    if(pthread_mutex_unlock(&mut) != 0)
	throw WEBDAR_BUG;
}


bool mutex::try_lock()
{
    int ret = pthread_mutex_trylock(&mut);
    if(ret != 0 && ret != EBUSY)
	throw exception_system("Error while trying locking mutex", ret);

    return ret == 0;
}

