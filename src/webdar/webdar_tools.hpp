#ifndef WEBDAR_TOOLS_HPP
#define WEBDAR_TOOLS_HPP

    // C++ system header files
#include <sstream>

template <class T> std::string webdar_tools_convert_to_string(T val)
{
    std::stringstream tmp;
    tmp << val;
    return tmp.str();
}

extern int webdar_tools_convert_to_int(const std::string & ref);

#endif
