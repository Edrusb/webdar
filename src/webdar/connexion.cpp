    //  C system header files
extern "C"
{
#include <errno.h>
#include <limits.h>
}

    // webdar headers

#include "connexion.hpp"

using namespace std;

#define BUFFER_SIZE 10240
#ifdef SSIZE_MAX
#if SSIZE_MAX < BUFFER_SIZE
#undef BUFFER_SIZE
#define BUFFER_SIZE SSIZE_MAX
#endif
#endif

connexion::connexion(int fd, const string & peerip, unsigned int peerport)
{
    etat = connected;
    filedesc = fd;
    ip = peerip;
    port = peerport;
    buffer_size = BUFFER_SIZE;
    buffer = NULL;
    already_read = 0;
    data_size = 0;

    buffer = new (nothrow) char[buffer_size];
    if(buffer == NULL)
	throw exception_memory();
}

connexion::~connexion()
{
    fermeture();
    if(buffer != NULL)
	delete buffer;
}

unsigned int connexion::read(char *a, unsigned int size, bool blocking)
{
    bool loop = true;
    ssize_t lu = 0;
    int flag = blocking ? 0 : MSG_DONTWAIT;

    if(etat != connected)
	throw WEBDAR_BUG;

    while(loop)
    {
	loop = false;
	lu = recv(filedesc, a, size, flag);
	if(lu == 0)
	    fermeture();
	if(lu < 0)
	{
	    switch(errno)
	    {
	    case EINTR:
		loop = true;
		break;
	    case EAGAIN:
		if(blocking)
		    throw WEBDAR_BUG;
		else
		    lu = 0;
		break;
	    default:
		throw exception_system("Error met while receiving data: ", errno);
	    }
	}
    }

    return (unsigned int)lu;
}

bool connexion::write(const char *a, unsigned int size)
{
    bool ret = true;
    bool loop = true;

    if(etat != connected)
	throw WEBDAR_BUG;

    while(loop)
    {
	loop = false;
	if(send(filedesc, (void *)a, size, MSG_NOSIGNAL) < 0)
	{
	    switch(errno)
	    {
	    case EPIPE:
		    // the connection is broken / closed by the other end
		fermeture();
		ret = false;
		break;
	    case EINTR:
		loop = true;
	    default:
		throw exception_system("Error met while sending data: ", errno);
	    }
	}
    }

    return ret;
}


void connexion::fermeture()
{
    if(etat == connected)
    {
	int shuted;
	int errnono;
	if(filedesc < 0)
	    throw WEBDAR_BUG;
	shuted = shutdown(filedesc, SHUT_RDWR);
	if(shuted != 0)
	    errnono = errno;
	close(filedesc);
	filedesc = -1;
	etat = not_connected;
	if(shuted != 0)
	    throw exception_system("failed shutting down the socket", errnono);
    }
}

void connexion::fill_buffer(bool blocking)
{
    if(data_size < buffer_size) // there is some room to receive more data in the buffer
    {
	if(get_status() == connexion::connected)
	{
	    if(already_read == data_size)   // no more data to read, so we restart at the beginning
		already_read = data_size = 0;
	    else
	    {
		if(already_read > data_size)
		    throw WEBDAR_BUG;
		if(data_size > buffer_size)
		    throw WEBDAR_BUG;
		if(already_read > 0)
		    (void)memmove(buffer, buffer + already_read, data_size - already_read);
	    }
	    data_size += read(buffer + data_size, buffer_size - data_size, blocking);
	}
	else
	{
	    if(already_read == data_size)
		throw exception_range("Connexion closed and no more data available for reading");
	}
    }
}

char connexion::read_one(bool blocking)
{
    if(already_read == data_size)
	fill_buffer(blocking);
    if(already_read == data_size)
	throw exception_range("no more data available from connection");

    return buffer[already_read++];
}

char connexion::read_test_first(bool blocking)
{
    if(already_read == data_size)
	fill_buffer(blocking);
    if(already_read == data_size)
	throw exception_range("no more data available from connection");

    return buffer[already_read];
}

char connexion::read_test_second(bool blocking)
{
    if(data_size - already_read < 2)
	fill_buffer(blocking);
    if(data_size - already_read < 2)
	throw exception_range("no more data available from connection");

    return buffer[already_read + 1];
}
