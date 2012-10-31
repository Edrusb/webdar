#ifndef BASE64_HPP
#define BASE64_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "exceptions.hpp"

class base64
{
public:
    base64(const std::string & str);

    std::string decode() const;
    std::string encode() const { throw exception_feature("base64::encode"); }

private:
    std::string ch;

    static std::string convert(char a, char b, char c, char d);
    static unsigned int value64(char a);
};

#endif
