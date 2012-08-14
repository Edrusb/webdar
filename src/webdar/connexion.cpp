#include "connexion.hpp"

using namespace std;
using namespace libdar;

connexion::connexion(int fd, const string & peerip, U_I peerport)
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

U_I connexion::read(char *a, U_I size)
{
	// a implementer


}

void connexion::write(const char *a, U_I size)
{
	// a implementer

}


