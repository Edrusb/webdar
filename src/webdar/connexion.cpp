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

unsigned int connexion::read(char *a, unsigned int size)
{
    bool loop = true;
    ssize_t lu = 0;

    if(etat != connected)
	throw WEBDAR_BUG;

    while(loop)
    {
	loop = false;
	lu = recv(filedesc, a, size, 0);
	if(lu == 0)
	    fermeture();
	if(lu < 0)
	{
	    switch(errno)
	    {
	    case EINTR:
		loop = true;
		break;
	    default:
		throw exception_system("Error met while sending data: ", errno);
	    }
	}
    }

    return (unsigned int)lu;
}

bool connexion::write(const char *a, unsigned int size)
{
    bool ret = true;
    bool loop = false;

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
	if(filedesc < 0)
	    throw WEBDAR_BUG;
	close(filedesc);
	filedesc = -1;
	etat = not_connected;
    }
}
