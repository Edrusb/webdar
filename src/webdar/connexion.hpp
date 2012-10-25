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

	/// extracts one byte form the buffer / exception thrown if not available
    char read_one(bool blocking);

	/// gives the next char to be read, but do not remove it from the
	/// reading buffer / throw exception if not available
    char read_test_first(bool blocking);

	/// gives the second next char to be read, but do not remove it
	/// from the reading buffer / throw exception if not available (EOF)
    char read_test_second(bool blocking);

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

	// buffer management
    unsigned buffer_size;      //< size of the buffer
    char *buffer;              //< temporary area used for parsing, reading data FROM network
    unsigned int already_read; //< amount of data already read
    unsigned int data_size;    //< total of data in buffer, already read or not

	/// close the connexion
    void fermeture();

	/// read data from the socket

	/// \param[in] a where to store read data
	/// \param[in] size width of the buffer pointed to by a
	/// \param[in] blocking whether reading is blocking or not
    unsigned int read(char *a, unsigned int size, bool blocking);


	/// manages to get (read) data in buffer and set relative variables acordingly
    void fill_buffer(bool blocking);

};

#endif
