#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <libdar/libdar.hpp>

#include "central_report.hpp"
#include "thread.hpp"
#include "exceptions.hpp"


    /// class listener
    ///
    /// listen to the defined TCP port for new connexions
    /// Upon new connexion, it builds a connexion objects
    /// and a "parser" object


class listener : public thread
{
public:
    listener(central_report *log, libdar::U_I port);   //< listen on localhost IPv4 or IPv6
    listener(central_report *log, const std::string & ip, libdar::U_I port); //< listen on the specified IPv4/v6 interface
    listener(const listener & ref) { throw WEBDAR_BUG; }; //< forbidding copy construction
    const listener & operator = (const listener & ref) { throw WEBDAR_BUG; }; //< forbidding assignment

    ~listener() { close(sockfd); };

protected:
	// inherited from thread;
    void inherited_run();

private:
    central_report *rep; //< where to report events
    int sockfd;          //< socket descriptor
    int famille;         //< domain familly of the socket

    void set_sockfd(int domain);
    void init(central_report *log, const std::string & ip,
	      libdar::U_I port);
};


#endif
