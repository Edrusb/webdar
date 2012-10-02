    // libraries header files
#include <libdar/libdar.hpp>

    // C++ system header files
#include <iostream>

    // webdar headers
#include "webdar_tools.hpp"
#include "exceptions.hpp"

using namespace std;

int webdar_tools_convert_to_int(const std::string & ref)
{
    int ret;
    std::stringstream buf;

    buf.str(ref);
    buf >> ret;
    if(buf.rdstate() != ios_base::goodbit && buf.rdstate() != ios_base::eofbit)
	throw exception_range(libdar::tools_printf("Cannot convert %S to integer", &ref));

    return ret;
}

void webdar_tools_split_by(char sep, const string & aggregate, vector<string> splitted)
{
    splitted.clear();

    string::const_iterator it = aggregate.begin();
    string::const_iterator bk = it;

    while(it != aggregate.end())
    {
	if(*it == sep)
	{
	    splitted.push_back(string(bk, it));
	    ++it;
	    bk = it;
	}
	else
	    ++it;
    }

    if(bk != aggregate.end())
	splitted.push_back(string(bk, it));
}


void webdar_tools_split_in_two(char sep, const string &aggregate, string & first, string & second)
{
    string::const_iterator it = aggregate.begin();

    while(it != aggregate.end() && *it != sep)
	++it;

    first = string(aggregate.begin(), it);
    if(it != aggregate.end())
	++it;
    second = string(it, aggregate.end());
}

extern void webdar_tools_remove_leading_spaces(const std::string & input)
{
    string::const_iterator it = input.begin();

    while(it != input.end() && *it == ' ')
	++it;

    return string(it, input.end());
}
