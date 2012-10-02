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
extern std::vector<std::string>  webdar_tools_split_by(char sep, const std::string & aggregate);
extern void webdar_tools_split_in_two(char sep, const std::string &aggregate, std::string & first, std::string & second);
extern std::string webdar_tools_remove_leadind_spaces(const std::string & input);
extern std::string webdar_tools_generate_random_string(unsigned int size);


#endif
