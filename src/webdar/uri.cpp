    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers




    //
#include "uri.hpp"

using namespace std;



const std::string uri::get_string() const
{
    std::string ret = url.size() > 1 ? url[0] : "";

    for(unsigned int i = 1; i < url.size(); ++i)
	ret += string("/") + url[i];

    return ret;
}
