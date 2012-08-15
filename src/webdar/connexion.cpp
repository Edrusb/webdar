
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
    if(etat == connected)
	close(filedesc);
}

unsigned int connexion::read(char *a, unsigned int size)
{
	// a implementer


}

void connexion::write(const char *a, unsigned int size)
{
	// a implementer

}


