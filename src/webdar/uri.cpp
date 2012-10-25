    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "exceptions.hpp"



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


void uri::read(const string & res)
{
    enum { scheme, hostname, path } lookup = scheme;
    string::const_iterator it = res.begin();
    string::const_iterator bk = it;

    clear();

    while(it != res.end())
    {
	switch(lookup)
	{
	case scheme:
	    if(*it != ':' && *it != '/')
		++it;
	    else
	    {
		if(*it == '/')
		{
		    url.push_back(""); // scheme not provided
		    lookup = hostname;
		    it = bk;
		}
		else
		{
		    url.push_back(string(bk, it));
		    ++it;
		    bk = it;
		    lookup = hostname;
		}
	    }
	    break;
	case hostname:
	    if(*it == '/')
	    {
		++it;
		if(it != res.end() && *it == '/') // this a URI with a net_path
		{
		    ++it;
		    bk = it;
		    lookup = path; // well, here the first member of the path will be the hostname, we do not need here to add an empty string
		}
		else // just an absolute path
		{
		    url.push_back(""); // empty host, the URL is not a net_path
		    bk = it;
		    lookup = path;
		}
	    }
	    else // just a relative path
	    {
		url.push_back(""); // empty host, the URL is not a net_path
		bk = it;
		lookup = path;
	    }
	    break;
	case path:
	    if(*it == '/')
	    {
		url.push_back(string(bk, it));
		++it;
		bk = it;
	    }
	    else
		++it;
	    break;
	default:
	    throw WEBDAR_BUG;
	}
    }

    switch(lookup)
    {
    case scheme:
	url.push_back("");
	    // no break;
    case hostname:
	url.push_back("");
	break;
    case path:
	break;
    default:
	throw WEBDAR_BUG;
    }

    if(bk != it)
	url.push_back(string(bk, it));
}
