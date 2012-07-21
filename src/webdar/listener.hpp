    /// class listener
    ///
    /// listen to the defined TCP port for new connexions
    /// the listener must CONTROL the number of active TCP session
    /// new session rate. Upon new connexion, it build a
    /// connexion objects and give it to a new parser object

class listener : public thread
{
public:
    listener(U_I port);                         //< listen on localhost IPv4 or IPv6
    listener(const std::string & ip, U_I port); //< listen on the specified IPv4/v6 interface

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
