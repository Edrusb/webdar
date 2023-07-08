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

#include "request.hpp"
#include "exceptions.hpp"
#include "webdar_tools.hpp"

using namespace std;


void request::clear()
{
    status = init;
    cached_method = "";
    coordinates.clear();
    attributes.clear();
    body = "";
}

bool request::try_reading(proto_connexion & input)
{
    return read_method_uri(input, false);
}


void request::read(proto_connexion & input)
{
    string key;
    string val, valorig;

	///////////////////////////////////////////
	// reading the first line of the request
	//

    if(!read_method_uri(input, true))
	throw WEBDAR_BUG;

    status = reading_all;

	// VERSION field

    if(!get_word(input, true, true, val))
	throw WEBDAR_BUG;
    if(val == "")
	val = "HTTP/0.9";
    set_version(val);
    skip_line(input);


	///////////////////////////////////////////
	// reading the request's header
	//

    while(!is_empty_line(input)) // which would mean the end of the header
    {
	if(!get_token(input, true, true, key))
	    throw WEBDAR_BUG;
	if(key == "")
	{
	    string mesg = "non RFC1945 conformant message header: empty string as entity-header field name";
	    clog->report(debug, mesg);
	    throw exception_range(mesg);
	}
	skip_over(input, ':');
	skip_over(input, ' ');
	val = up_to_eol_with_LWS(input);
	if(find_attribute(key, valorig))
	    add_attribute(key, valorig + ',' + val);
	else
	    add_attribute(key, val);
    }
    skip_line(input); // we now point to the beginning of the body

	///////////////////////////////////////////
	// reading the body
	//

    if(find_attribute(HDR_CONTENT_LENGTH, val))
    {
	int size;
	try
	{
	    size = webdar_tools_convert_to_int(val);
	}
	catch(exception_bug & e)
	{
	    throw;
	}
	catch(...)
	{
	    string mesg = string("Value given to ") + HDR_CONTENT_LENGTH + " is not an integer: " + val;
	    clog->report(debug, mesg);
	    throw exception_range(mesg);
	}
	body = up_to_length(input, size);
    }
    else
	body = "";

    status = completed;

    extract_cookies();

	///////////////////////////////////////////
	// Sanity checks
	//


	// HTTP version control

    if(maj_vers != 1 || (min_vers != 0 && min_vers != 1))
    {
	string mesg = "The received request is using an unsupported HTTP version: ";

	mesg += webdar_tools_convert_to_string(maj_vers) + "." + webdar_tools_convert_to_string(min_vers);
	clog->report(info, mesg);
	throw exception_input(mesg, STATUS_CODE_NOT_IMPLEMENTED);
    }


	// URI scheme

    if(coordinates.get_scheme() != "http" && coordinates.get_scheme() != "")
    {
	string mesg = "Unsupported scheme in URI: ";
	mesg += coordinates.get_scheme();

	clog->report(debug, mesg);
	throw exception_input(mesg, STATUS_CODE_BAD_REQUEST);
    }


	// HTTP method control

    if(cached_method != "GET" && cached_method != "POST" && cached_method != "HEAD")
    {
	string mesg = "The received request using an unknown method: ";
	mesg += cached_method;

	clog->report(debug, mesg);
	throw exception_input(mesg, STATUS_CODE_BAD_REQUEST);
    }

}

map<string,string> request::get_body_form() const
{
    string tmp, aux;
    vector<string> split;
    vector<string>::iterator it;
    map<string, string> ret;

    if(!find_attribute(HDR_CONTENT_TYPE, tmp))
	return ret;
    if(webdar_tools_to_canonical_case(tmp)
       != webdar_tools_to_canonical_case(VAL_CONTENT_TYPE_FORM))
	return ret;

    tmp = webdar_tools_decode_urlencoded(get_body());
    webdar_tools_split_by('&', tmp, split);
    for(vector<string>::iterator it = split.begin(); it != split.end(); ++it)
    {
	webdar_tools_split_in_two('=', *it, tmp, aux);
	ret[tmp] = aux;
    }

    return ret;
}

void request::add_cookie(const std::string & key, const std::string & value) const
{
    const_cast<request *>(this)->cookies[key] = value;
}

bool request::find_cookie(const std::string & key, std::string & value) const
{
    map<string, string>::const_iterator it = cookies.find(key);

    if(it != cookies.end())
    {
	value = it->second;
	return true;
    }
    else
	return false;
}

bool request::find_attribute(const std::string & key, std::string & value) const
{
    string lkey = webdar_tools_to_canonical_case(key);
    map<string, string>::const_iterator it = attributes.find(lkey);

    if(status < reading_all)
	throw WEBDAR_BUG;

    if(it != attributes.end())
    {
	value = it->second;
	return true;
    }
    else
	return false;
}

void request::extract_cookies()
{
    string key, val;
    bool found = find_attribute(HDR_COOKIE, val);
    cookies.clear();

    if(found)
    {
	vector<string> comma_sep;
	vector<string>::iterator it;
	vector<string> tmp;
	vector<string> semi_col_sep;

	webdar_tools_split_by(',', val, comma_sep);
	for(it = comma_sep.begin(); it != comma_sep.end(); ++it)
	{
	    webdar_tools_split_by(';', *it, tmp);
	    webdar_tools_concat_vectors(semi_col_sep, tmp);
	}
	for(it = semi_col_sep.begin(); it != semi_col_sep.end(); ++it)
	{
	    webdar_tools_split_in_two('=', *it, key, val);
	    cookies[key] = val;
	}
	drop_attribute(HDR_COOKIE);
    }
}

bool request::read_method_uri(proto_connexion & input, bool blocking)
{
    string tmp;
    string cached_uri;

    if(status > uri_read)
	throw WEBDAR_BUG;

    if(status == init)
    {
	if(get_token(input, cached_method == "", blocking, tmp))
	    status = method_read;
	cached_method += tmp;
    }

    if(status == method_read)
    {
	if(status != uri_read)
	{
	    if(get_word(input, cached_uri == "", blocking, tmp))
		status = uri_read;
	    cached_uri += tmp;

	    if(status == uri_read)
		coordinates.read(cached_uri);
	}
    }

    switch(status)
    {
    case init:
    case method_read:
    case uri_read:
	break;
    case reading_all:
    case completed:
	throw WEBDAR_BUG;
    default:
	throw WEBDAR_BUG;
    }

    return status == uri_read;
}

void request::drop_attribute(const std::string & key)
{
    map<string, string>::iterator it = attributes.find(key);

    if(it != attributes.end())
	attributes.erase(it);
}

bool request::is_empty_line(proto_connexion & input)
{
    bool ret;

    try
    {
	ret = (input.read_test_first(true) == '\r') && (input.read_test_second(true) == '\n');
    }
    catch(exception_bug & e)
    {
	throw;
    }
    catch(exception_base & e)
    {
	ret = false;
	    // no throw!
    }

    return ret;
}

string request::up_to_eol(proto_connexion & input)
{
    string ret;

    try
    {
	while(input.read_test_first(true) != '\r' || input.read_test_second(true) != '\n')
	    ret += input.read_one(true);
	skip_line(input);
    }
    catch(exception_bug & e)
    {
	throw;
    }
    catch(exception_base & e)
    {
	    // we ignore here any end of connection
    }
    return ret;
}

string request::up_to_eof(proto_connexion & input)
{
    string ret;

    try
    {
	while(true)
	    ret += input.read_one(true);
    }
    catch(exception_bug & e)
    {
	throw;
    }
    catch(exception_base & e)
    {
	    // this is the way we know that we reached eof
    }

    return ret;
}

void request::skip_over(proto_connexion & input, char a)
{
    try
    {
	while(input.read_one(true) != a)
	    ;
    }
    catch(exception_bug & e)
    {
	throw;
    }
    catch(exception_base & e)
    {
	    // we ignore here any end of connection
    }
}


string request::up_to_length(proto_connexion & input, unsigned int length)
{
    string ret;

	/// TO BE IMPROVED ADDING A READ CALL WITH INPUT BUFFER TO PROTO_CONNEXION CLASS
    while(length > 0)
    {
	ret += input.read_one(true);
	--length;
    }

    return ret;
}

void request::skip_line(proto_connexion & input)
{
    bool loop = true;
    char a;

    try
    {
	while(loop)
	{
	    a = input.read_one(true);
	    if(a == '\r')
	    {
		if(input.read_test_first(true) == '\n')
		{
		    a = input.read_one(true);
		    loop = false;
		}
	    }
	}
    }
    catch(exception_bug & e)
    {
	throw;
    }
    catch(exception_base & e)
    {
	    // we ignore here any end of connection
    }
}

string request::up_to_eol_with_LWS(proto_connexion & input)
{
    string ret;
    bool loop = false;

    try
    {
	do
	{
	    loop = false;
	    ret += up_to_eol(input);
	    if(input.read_test_first(true) == ' ' || input.read_test_first(true) == '\t')
	    {
		loop = true;
		while(input.read_test_first(true) == ' ' || input.read_test_first(true) == '\t')
		    (void)input.read_one(true);
		ret += ' '; // "All LWS including folding, have the same semantics as SP"
	    }
	}
	while(loop);
    }
    catch(exception_bug & e)
    {
	throw;
    }
    catch(exception_range & e)
    {
	    // nothing done, as we reached end of file
	    // we return the data read so far
    }

    return ret;
}


bool request::get_token(proto_connexion & input, bool initial, bool blocking, string & token)
{
    bool ret = true;
    bool loop = true;
    char a;
    token = "";

    try
    {
	while(loop)
	{
	    a = input.read_test_first(blocking);

	    if((a == ' ' || a == '\t') && initial)
		a = input.read_one(blocking);
	    else
	    {
		    // token is built of any char except CTLs or tspecials
		    // tspecials are:  ()<>@,;:\"/[]?={} space and tab
		    // as defined by RFC 1945 paragraph 2.2 "Basic Rules".
		if(a < 127 && a > 31
		   && a != '(' && a != ')' && a != '<' && a != '>'
		   && a != '@' && a != ',' && a != ';' && a != ':'
		   && a != '\\' && a != '"' && a != '/' && a != '['
		   && a != ']' && a != '?' && a != '=' && a != '{'
		   && a != '}' && a != ' ' && a != '\t')
		{
		    token += input.read_one(blocking);
		    initial = false;
		}
		else
		    loop = false;
	    }
	}
    }
    catch(exception_bug & e)
    {
	throw;
    }
    catch(exception_base & e)
    {
	if(!blocking)
	    ret = false;
	else   // blocking read: returning the read data or failing if not data has been read so far
	    if(token == "")
		throw;
    }

    return ret;
}

bool request::get_word(proto_connexion & input, bool initial, bool blocking, string & word)
{
    string tmp;
    char ctmp;
    bool loop = false;
    bool ret = false;

    word = "";

    try
    {
	do
	{
	    loop = false;
	    ret = get_token(input, initial, blocking, tmp);
	    if(tmp != "")
	    {
		initial = false;
		word += tmp;
	    }
	    if(ret)
	    {
		try
		{
		    ctmp = input.read_test_first(blocking);
		}
		catch(exception_bug & e)
		{
		    throw;
		}
		catch(...)
		{
			// EOF met
		    if(word != "" || !blocking)
			break; // exiting the while loop
		    else
			throw;
			// nothing to return so far so propagating the exception
		}

		switch(ctmp)
		{
		case '/':
		case ':':
		case '=':
		case '@':
		case '?':
		    word += input.read_one(blocking);
		    initial = false;
		    loop = true;
		    break;
		default:
		    break; // nothing to do, as loop is already false
		}
	    }
	}
	while(loop);
    }
    catch(exception_bug & e)
    {
	throw;
    }
    catch(exception_base & e)
    {
	if(word == "")
	    throw;
    }

    return ret;
}


void request::set_version(const string & version)
{
    static const char expected[] = { 'H', 'T', 'T', 'P', '/', '\0' };
    const char *ptr = expected;
    string::const_iterator it = version.begin();
    string::const_iterator bk = it;

    while(it != version.end() && *ptr != '\0' && *it == *ptr)
    {
	++ptr;
	++it;
    }

    if(*ptr != '\0' || it == version.end())
    {
	string mesg = string("badly formated HTTP version field: ") + version;
	clog->report(debug, mesg);
	throw exception_range(mesg);
    }
    bk = it;

    while(it != version.end() && *it != '.')
	++it;

    if(it == version.end())
    {
	string mesg = string("badly formated HTTP version field: ") + version;
	clog->report(debug, mesg);
	throw exception_range(mesg);
    }

    maj_vers = webdar_tools_convert_to_int(string(bk, it));
    ++it;
    if(it == version.end())
    {
	string mesg = string("badly formated HTTP version field: ") + version;
	clog->report(debug, mesg);
	throw exception_range(mesg);
    }

    min_vers = webdar_tools_convert_to_int(string(it, version.end()));
}
