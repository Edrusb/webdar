    /// class listener
    ///
    /// listen to the defined TCP port for new connexions
    /// the listener must CONTROL the number of active TCP session
    /// new session rate. Upon new connexion, it build a
    /// connexion objects and give it to a new parser object

class listener
{
public:
    listener(U_I port, U_I max_session, U_I session_per_minute);
    listener(ipv4, U_I port, U_I max_session, U_I session_per_minute);
    listener(ipv6, U_I port, U_I max_session, U_I session_per_minute);

private:
	// to be defined
};
