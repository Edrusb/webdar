    /// class listener
    ///
    /// listen to the defined TCP port for new connexions
    /// Upon new connexion, it builds a connexion objects
    /// and a "parser" object

class listener : public thread
{
public:
    listener(U_I port);                         //< listen on localhost IPv4 or IPv6
    listener(const std::string & ip, U_I port); //< listen on the specified IPv4/v6 interface
    listener(const listener & ref) { throw WEBDAR_BUG; }; //< forbidding copy construction
    const listener & operator = (const listener & ref) { throw WEBDAR_BUG; }; //< forbidding assignment

    ~listener() { close(sockfd); };

protected:
	// inherited from thread;
    void inherited_run();

private:
    int sockfd;      //< socket descriptor
    int famille;     //< domain familly of the socket

    int set_sockfd(int domain);
    void init(const std::string & ip, U_I port, U_I max_session, U_I session_per_minute);
};
