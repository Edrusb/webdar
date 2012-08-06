#include "connexion.hpp"

using namespace std;

connexion::connexion(int fd, const string & peerip, U_I peerport) : generic_file(gf_read_write)
{
    etat = connected;
    filedesc = fd;
    ip = peerip;
    port = peerport;
}

virtual connexion::~connexion()
{
    if(etat == connected)
	close(filedesc);
}

U_I inherited_read(char *a, U_I size)
{
// need to know when to stop reading. or change the paradigm of read (EOF) or do not inherit from generic_file


}

void inherited_write(const char *a, U_I size)
{


}


