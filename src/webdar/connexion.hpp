
    /// class connexion - holds TCP sessions from clients

    /// this class provide read/write access to a TCP connexion
    /// it may support future evolution by mean of inherited class
    /// that will provide for example ssl ciphering for example
class connexion
{
public:
    enum status
    {
	connected,   //< both read and write are allowed
	no_connected //< session is closed both directions
    };

	/// create a new object based on a existing socket filedescriptor
    connexion(int fd, const std::string & peerip, U_I peerport);

	/// forbidding copy constuctor and assignment operator
    connexion(const connexion & ref) { throw WEBDAR_BUG; };
    const connexion & operator = (const connexion & ref) { throw WEBDAR_BUG; };

	/// destructor is virtual to permit further evolution by mean of inherited classes
    virtual ~connexion();

    status get_status() const { return etat; };
    const string & get_ip() const { return ip; };
    U_I get_port() const { return port; };

	/// read data from the socket

	/// \param[in] a pointers where to put read bytes
	/// \param[in] size maximum number of byte to read
	/// \return the number of read bytes.
	/// \note if the connexion object is 'disconnected' read returns zero
	/// \note this call may block if no data is available for reading
	/// \note this call must not block if some data is available for reading even
	/// if its amount is less than 'size'
    virtual U_I read(char *a, U_I size);

	/// write data to the socket

	/// param[in] a data to write
	/// param[in] size amount of byte to write
    virtual void write(const char *a, U_I size);

private:
    status etat;      //< connexion status
    int filedesc;     //< socket file descriptor
    std::string ip;   //< IP of the peer host
    U_I port;         //< port of the peer port
};
