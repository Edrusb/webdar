    //  C system header files
extern "C"
{
#include <errno.h>
}

    // webdar headers

#include "connexion.hpp"

using namespace std;

connexion::connexion(int fd, const string & peerip, unsigned int peerport)
{
    etat = connected;
    filedesc = fd;
    ip = peerip;
    port = peerport;
}

connexion::~connexion()
{
    fermeture();
}

unsigned int connexion::common_read(char *a, unsigned int size, bool blocking)
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
