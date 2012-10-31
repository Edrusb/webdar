#ifndef AUTHENTICATION_HPP
#define AUTHENTICATION_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers

class authentication
{
public:
    virtual ~authentication() {};

    virtual bool valid_credentials(const std::string & username, const std::string & credential) const = 0;
};



class authentication_unix : public authentication
{
/// A IMPLEMENTER

};

class authentication_cli : public authentication
{
public:
    authentication_cli(const std::string & username, const std::string & password) { user = username; pass = password; };

    virtual bool valid_credentials(const std::string & username, const std::string & credential) const { return username == user && credential == pass; }; // no need of mutex for this class

private:
    std::string user;
    std::string pass;

};


#endif
