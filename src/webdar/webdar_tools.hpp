#ifndef WEBDAR_TOOLS_HPP
#define WEBDAR_TOOLS_HPP

#include <sstream>

template <class T> std::string webdar_tools_convert_to_string(T val)
{
    std::stringstream tmp;
    tmp << val;
    return tmp.str();
}

#endif
