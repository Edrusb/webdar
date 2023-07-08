/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2022 Denis Corbin
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
#include "tokens.hpp"
    //
#include "answer.hpp"


using namespace std;

void answer::clear()
{
    status = maj_vers = min_vers = 0;
    reason = "";
    attributes.clear();
    next_read = attributes.begin();
    set_attribute(HDR_SERVER, "webdar/0.0");
    add_body(""); // this adds the Content-Lenght header
};

void answer::add_cookie(const std::string & key, const std::string & value)
{
    string oldval;
    string xval = key+"="+value;
    if(find_attribute(HDR_SET_COOKIE, oldval))
	set_attribute(HDR_SET_COOKIE, oldval + "; " + xval);
	// we cannot use add_attribute_member as it builds a comma (,) separated list
	// while the Set-Cookie field receives a semi-column (;) separated list
    else
	set_attribute(HDR_SET_COOKIE, xval);
}

void answer::add_body(const std::string & key)
{
    body = key;
    set_attribute(HDR_CONTENT_LENGTH, webdar_tools_convert_to_string(body.size()));
}

bool answer::is_valid() const
{
    return status < 600 && status > 99;
}

void answer::write(proto_connexion & output)
{
    string key, val;

    if(maj_vers != 1 || (min_vers != 0 && min_vers != 1))
	throw exception_feature("Unsupported HTTP protocole version: "
				+ webdar_tools_convert_to_string(maj_vers)
				+ "/"
				+ webdar_tools_convert_to_string(min_vers));

    if(status < 100 && status > 599)
	throw WEBDAR_BUG;

    val = string("HTTP/") + webdar_tools_convert_to_string(maj_vers)
	+ "." + webdar_tools_convert_to_string(min_vers);
    output.write(val.c_str(), val.size());
    output.write(" ", 1);
    val = webdar_tools_convert_to_string(status);
    output.write(val.c_str(), val.size());
    output.write(" ", 1);
    output.write(reason.c_str(), reason.size());
    output.write("\r\n", 2);

    reset_read_next_attribute();
    while(read_next_attribute(key, val))
    {
	output.write(key.c_str(), key.size());
	output.write(": ", 2);
	output.write(val.c_str(), val.size());
	output.write("\r\n", 2);
    }
    output.write("\r\n", 2);
    if(get_body().size() > 0)
	output.write(body.c_str(), body.size());

	// flushing output writings
    output.flush_write();
}

void answer::add_attribute_member(const std::string & key, const std::string & value)
{
    string in_place;
    if(find_attribute(key, in_place))
	set_attribute(key, in_place + "," + value);
    else
	set_attribute(key, value);
}

bool answer::find_attribute(const string & key, string & value) const
{
    string lkey = webdar_tools_to_canonical_case(key);
    map<string,string>::const_iterator it = attributes.find(lkey);

    if(it != attributes.end())
    {
	value = it->second;
	return true;
    }
    else
	return false;
}

void answer::reset_read_next_attribute() const
{
    next_read = attributes.begin();
}

bool answer::read_next_attribute(std::string & key, std::string & value) const
{
    if(next_read != attributes.end())
    {
	key = next_read->first;
	value = next_read->second;
	++next_read;
	return true;
    }
    else
	return false;
}

void answer::copy_from(const answer & ref)
{
    status = ref.status;
    reason = ref.reason;
    maj_vers = ref.maj_vers;
    min_vers = ref.min_vers;
    attributes = ref.attributes;
    body = ref.body;
    next_read = attributes.begin();
}
