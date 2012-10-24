#ifndef CONNEXION_HPP
#define CONNEXION_HPP

    // C++ system header files
#include <string>

    // webdar headers
#include "exceptions.hpp"

    /// class connexion - holds TCP sessions from clients

    /// this class provide read/write access to a TCP connexion
    /// it may support future evolution by mean of inherited class
    /// that will provide for example ssl ciphering for example
class connexion
{
public:
    enum status
    {
	connected,    //< both read and write are allowed
	not_connected //< session is closed both directions
    };

	/// create a new object based on a existing socket filedescriptor
    connexion(int fd, const std::string & peerip, unsigned int peerport);

	/// forbidding copy constuctor and assignment operator
    connexion(const connexion & ref) { throw WEBDAR_BUG; };
    const connexion & operator = (const connexion & ref) { throw WEBDAR_BUG; };

	/// destructor is virtual to permit further evolution by mean of inherited classes
    virtual ~connexion();

    status get_status() const { return etat; };
    const std::string & get_ip() const { return ip; };
    unsigned int get_port() const { return port; };

	/// read data from the socket

	/// \param[in] a pointers where to put read bytes
	/// \param[in] size maximum number of byte to read
	/// \return the number of read bytes.
	/// \note if the connexion object is 'disconnected' read returns zero
	/// \note this call may block if no data is available for reading
	/// \note this call must not block if some data is available for reading even
	/// if its amount is less than 'size'
    virtual unsigned int read(char *a, unsigned int size)
    { return common_read(a, size, true); };

    virtual unsigned int non_blocking_read(char *a, unsigned int size)
    { return common_read(a, size, false); };

	/// write data to the socket

	/// param[in] a data to write
	/// param[in] size amount of byte to write
	/// \return true upon success of false if connexion is broken
    virtual bool write(const char *a, unsigned int size);

private:
    status etat;       //< connexion status
    int filedesc;      //< socket file descriptor
    std::string ip;    //< IP of the peer host
    unsigned int port; //< port of the peer port

    void fermeture();  //< close connection
    unsigned int common_read(char *a, unsigned int size, bool blocking);
};

#endif
