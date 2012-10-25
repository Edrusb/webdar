#ifndef WEBDAR_TOOLS_HPP
#define WEBDAR_TOOLS_HPP

    // C++ system header files
#include <sstream>
#include <vector>
#include <string>

    // webdar headers
#include "uri.hpp"


template <class T> std::string webdar_tools_convert_to_string(T val)
{
    std::stringstream tmp;
    tmp << val;
    return tmp.str();
}

    /// semi comparison
    ///
    /// \param[in] a first member of the comparison
    /// \param[in] b second member of the comparision
    /// \param[out] result result of the comparision (true if a < b, false if b < a)
    /// \return true if a < b or b < a else false
template <class T> bool webdar_tools_semi_less_than(T a, T b, bool & result)
{
    if(a < b)
    {
	result = true;
	return true;
    }
    if(b < a)
    {
	result = false;
	return true;
    }
    return false; // assu
}

extern int webdar_tools_convert_to_int(const std::string & ref);
extern void webdar_tools_split_by(char sep, const std::string & aggregate, std::vector<std::string> & splitted);
extern void webdar_tools_split_in_two(char sep, const std::string &aggregate, std::string & first, std::string & second);
extern std::string webdar_tools_remove_leading_spaces(const std::string & input);
extern void webdar_tools_init_randomization();
extern std::string webdar_tools_generate_random_string(unsigned int size);
extern std::string webdar_tools_get_session_ID_from_URI(const uri & url);
extern std::string webdar_tools_to_lowercase(const std::string & ch);

#endif
