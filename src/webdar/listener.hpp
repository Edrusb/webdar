    /// class listener
    ///
    /// listen to the defined TCP port for new connexions
    /// the listener must CONTROL the number of active TCP session
    /// new session rate. Upon new connexion, it build the
    /// connexion objects, the class of connexion does eventually
    /// the authentication part. Listener checks whether existing session
    /// exists. if so, it launch a chooser object thread that will speak HTML
    /// and let the user decide whether to reconnect or not to an existing
    /// session.

class listener
{
public:
    listener(U_I port, U_I max_session, U_I session_per_minute);
    listener(ipv4, U_I port, U_I max_session, U_I session_per_minute);
    listener(ipv6, U_I port, U_I max_session, U_I session_per_minute);

private:
    vector<chooser> pool; //< the size depends on the number of running chooser, it cannot exceed max_session anyway. Object not running are not destroyed they just stay available
};
