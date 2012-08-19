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
