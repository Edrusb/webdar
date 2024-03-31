/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2023 Denis Corbin
//
// This file is part of Webdar
//
//  Webdar is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Webdar is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Webdar.  If not, see <http://www.gnu.org/licenses/>
//
//----
//  to contact the author: dar.linux@free.fr
/*********************************************************************/

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "exceptions.hpp"
#include "webdar_tools.hpp"


    //
#include "uri.hpp"

using namespace std;

bool uri::operator == (const uri & arg) const
{
    return scheme == arg.scheme
	&& hostname == arg.hostname
	&& path == arg.path;
}


const std::string uri::get_string() const
{
    std::string ret = "";

    if(scheme != "")
	ret += scheme + ":";

    if(hostname != "")
	ret += "//" + hostname;

    if(path.empty() && hostname != "")
	ret += "/";
    else
	ret += path.display();

    if(ret == "")
	ret = "/";

    return ret;
}

void uri::read(const string & res)
{
    enum { l_scheme, l_hostname, l_host_path, l_path } lookup = l_scheme;
    string::const_iterator it = res.begin();
    string::const_iterator bk = it;

    clear();

    while(it != res.end())
    {
	switch(lookup)
	{
	case l_scheme:
	    if(*it != ':' && *it != '/')
		++it;
	    else
	    {
		if(*it == '/')
		{
		    scheme = "";
		    lookup = l_hostname;
		    it = bk;
		}
		else
		{
		    scheme = string(bk, it);
		    ++it;
		    bk = it;
		    lookup = l_hostname;
		}
	    }
	    break;
	case l_hostname:
	    if(*it == '/')
	    {
		++it;
		if(it != res.end() && *it == '/') // this a URI with a net_path
		{
		    ++it;
		    bk = it;
		    lookup = l_host_path; // well, here the first member of the path will be the hostname, we do not need here to add an empty string
		}
		else // just an absolute path
		{
		    hostname = ""; // empty host, the URL is not a net_path
		    bk = it;
		    lookup = l_path;
		}
	    }
	    else // just a relative path
	    {
		hostname = ""; // empty host, the URL is not a net_path
		bk = it;
		lookup = l_path;
	    }
	    break;
	case l_host_path:
	    if(*it == '/')
	    {
		hostname = string(bk, it);
		++it;
		bk = it;
		lookup = l_path;
	    }
	    else
		++it;
	    break;
	case l_path:
	    path = chemin(string(it, res.end()));
	    it = res.end();
	    break;
	default:
	    throw WEBDAR_BUG;
	}
    }
}

