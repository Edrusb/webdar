
extern "C"
{
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unsitd.h>
}

#include "listener.hpp"

using namespace std;

static struct in_addr string_to_network_IPv4(const std::string & ip);
static struct in6_addr string_to_network_IPv6(const std::string & ip);
static string network_IPv4_to_string(const struct in_addr & ip);
static string network_IPv6_to_string(const struct in6_addr & ip);

listener::listener(U_I port)
{
    try
    {
	init("::1", port);
    }
    catch(...)
    {
	init("127.0.0.1", port);
	    // no throw;
    }
}

listener::listener(const std::string & ip, U_I port)
{
    init(ip, port);
}

void listener::init(const std::string & ip, U_I port, U_I max_session, U_I session_per_minute)
{
    int tmp = 0;

    sockfd = -1;

    try
    {
	struct sockaddr *ptr;
	socklen_t ptr_len = 0;
	struct sockaddr_in sin;
	struct sockaddr_in6 sin6;

	try
	{
	    (void)memset(&sin6, 0, sizeof(sin6));
	    sin6.sin6_family = AF_INET6;
	    sin6.sin6_port = htons(port);
		// sin6.sin6_flowinfo is not modified (set to zero here)
	    sin6.sin6_addr = string_to_network_IPv6(ip.c_str());
		// sin6.sin6_scope_id is not modified (set to zero here)
	    ptr = & sin6;
	    ptr_len = sizeof(sin6);

	    set_sockfd(AF_INET6);
	}
	catch(...)
	{
	    (void)memset(&sin, 0, sizeof(sin));
	    sin.sin_family = AF_INET;
	    sin.sin_port = htons(port);
	    sin.sin_addr = string_to_network_IPv4(ip.c_str());
	    ptr = & sin;
	    ptr_len = sizeof(sin);

	    set_sockfd(AF_INET);
		// no throw
	}

	if(bind(sockfd, ptr, ptr_len) < 0)
	    throw exception_system("Error while binding socket to network", errno);

	    // we use SOMAXCONN here because its meaning depends on OS implementations
	    // So we do not use to control the max number of concurrent session
	if(listen(sockfd, SOMAXCONN) < 0)
	    throw exception_system("Error while trying to set the socket to listen", errno);
    }
    catch(...)
    {
	if(sockfd >= 0)
	    close(sockfd);
	sockfd = -1;
	throw;
    }
}

void listener::set_sockfd(int domain)
{
    sockfd = socket(domain, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd < 0)
	throw exception_system("Error creating socket", errno);
    famille = domain;
}

virtual void listener::inherited_run()
{
    int ret = 0;
    struct sockaddr *addr = NULL;
    socklen_t addrlen = 0;
    socklen_t addrlen_ref = 0;
    struct sockadd_in sin;
    struct sockadd_in6 sin6;
    string ip;
    U_I port;
    connexion *con = NULL;

    switch(famille)
    {
    case AF_INET6:
	addr = & sin6;
	addrlen = sizeof(sin6);
	break;
    case AF_INET:
	addr = & sin;
	addrlen = sizeof(sin);
	break;
    default:
	throw WEBDAR_BUG;
    }

    addrlen_ref = addrlen;

    while(true)
    {
	(void)memset(addr, 0, addrlen);
	ret = accept(sockfd, addr, &addrlen);

	if(ret < 0)
	{
	    switch(errno)
	    {
	    case ECONNABORTED:
	    case EFAU
	    case EINTR:
	    case EPROTO:
	    case EPERM:
	    case ENOSR:
	    case ETIMEOUT:
	    case ERSTARSYS:
		break;
	    case EMFILE:
	    case ENOBUFS:
	    case ENOMEM:
		sleep(1);
		break;
	    case EAGAIN:
		throw WEBDAR_BUG;
	    case EWOULDBLOCK:
		throw WEBDAR_BUG;
	    case EBADF:
		throw WEBDAR_BUG;
	    case EINVAL:
		throw WEBDAR_BUG;
	    case ENOTSOCK:
		throw WEBDAR_BUG;
	    case EOPNOTSUPP:
		throw WEBDAR_BUG;
	    case ESOCKNOTSUPPORT:
		throw WEBDAR_BUG;
	    case EPROTONOTSUPPORT:
		throw WEBDAR_BUG;
	    default:
		throw WEBDAR_BUG;
	    }
	    continue;
	}

	if(addrlen > addrlen_ref) // addr is truncated
	    throw WEBDAR_BUG;

	switch(famille)
	{
	case AF_INET6:
	    ip = network_IPv6_to_string(sin6.sin6_addr);
	    port = ntohs(sin6.sin6_port);
	    break;
	case AF_INET:
	    ip = network_IPv4_to_string(sin.sin_addr);
	    port = ntohs(sin.sin_port);
	    break;
	default:
	    throw WEBDAR_BUG:
	}

	con = new connexion(ret, ip, port);
	if(con == NULL)
	    throw exception_memory();
	else
	{
	    try
	    {
		parser::run_new_parser(con);
		con = NULL;
	    }
	    catch(...)
	    {
		delete con;
		throw;
	    }
	}
    }
}

static struct in_addr string_to_network_IPv4(const std::string & ip)
{
    struct in_addr ret;

    switch(inet_pton(AF_INET, ip.c_str(), &ret.s_addr))
    {
    case 0:
	throw exception_range("Badly formated IPv4 address");
    case -1:
	throw exception_system("Error while parsing IPv4 address", errno);
    case 1:
	break; // success
    default:
	throw WEBDAR_BUG;
    }

    return ret;
}

static struct in6_addr string_to_network_IPv6(const std::string & ip)
{
    struct in6_addr ret;

    switch(inet_pton(AF_INET6, ip.c_str(), &ret.s6_addr))
    {
    case 0:
	throw exception_range("Badly formated IPv6 address");
    case -1:
	throw exception_system("Error while parsing IPv6 address", errno);
    case 1:
	break; // success
    default:
	throw WEBDAR_BUG;
    }

    return ret;
}

static string network_IPv4_to_string(const struct in_addr & ip)
{
    string ret;
    const int BUFSZ = 20;
    char buffer[BUFSZ];
    char *ptr = inet_ntop(AF_INET, &ip, buffer, BUFSZ);

    if(ptr == NULL)
	throw exception_system("error converting IPv4 address to its textual representation", errno);
    else
	ret = *ptr;

    return ret;
}

static string network_IPv6_to_string(const struct in6_addr & ip)
{
    string ret;
    const int BUFSZ = 50;
    char buffer[BUFSZ];
    char * inet_ntop(AF_INET6, &ip, buffer, BUFSZ);

    if(ptr == NULL)
	throw exception_system("error converting IPv4 address to its textual representation", errno);
    else
	ret = *ptr;

    return ret;
}


