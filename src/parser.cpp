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

#include "webdar_tools.hpp"
#include "date.hpp"
#include "parser.hpp"
#include "tokens.hpp"

using namespace std;


parser::parser(unique_ptr<connexion> & input, shared_ptr<central_report> log): req(log)
{
    if(!input)
	throw WEBDAR_BUG;

    if(log == nullptr)
	throw WEBDAR_BUG;

    if(input->get_status() != connexion::connected)
	throw exception_range("connection is already closed cannot read from it");

    answered = true;
    source = std::move(input);
}

void parser::close()
{
    source.reset();
	// this should invoke the destructor on the pointed to connexion object
}

bool parser::get_next_request_uri(uri & url)
{
    if(!answered)
	throw WEBDAR_BUG;
    valid_source();

    if(req.try_reading(*source))
    {
	url = req.get_uri();
	return true;
    }
    else
	return false;
}

const request & parser::get_request()
{
    if(!answered)
	throw WEBDAR_BUG; // last request has not been answred
    valid_source();

    answered = false;

    try
    {
	req.clear();
	req.read(*source);
    }
    catch(exception_input & e)
    {
	answer err;
	string tmp;

	err.set_status(e.get_error_code());
	err.set_reason(e.get_message());
	send_answer(err);
	throw;
    }
    catch(exception_base & e)
    {
	source.reset();
	throw;
    }

    valid_source(); // avoid returning a empty request when the socket is closed

    return req;
}

void parser::send_answer(answer & ans)
{
    if(answered)
	throw WEBDAR_BUG;
    valid_source();

    try
    {
	if(!ans.is_valid())
	    throw WEBDAR_BUG;
	checks_main(req, ans);
	ans.write(*source);
	answered = true;
	req.clear();
    }
    catch(exception_bug & e)
    {
	throw;
    }
    catch(exception_base & e)
    {
	source.reset();
	answered = true;
	    // no throw
    }
}

void parser::checks_main(const request & req, answer & ans)
{
    checks_webdar(req, ans);
    checks_rfc1945(req, ans);
}

void parser::checks_webdar(const request & req, answer & ans)
{
    string val;

	// responding with the same version as the one used in the request
    ans.set_version(req.get_maj_version(), req.get_min_version());

	// adding a Date header if missing
    if(!ans.find_attribute(HDR_DATE, val))
	ans.set_attribute(HDR_DATE, date().get_canonical_format());

	// adding an Expires header if missing
    if(!ans.find_attribute(HDR_EXPIRES, val))
	ans.set_attribute(HDR_EXPIRES, date().get_canonical_format());

	// adding a default text/html content type if not specified
    if(ans.get_body().size() > 0)
    {
	if(!ans.find_attribute(HDR_CONTENT_TYPE, val))
	    ans.set_attribute(HDR_CONTENT_TYPE, "text/html");
    }
}

void parser::checks_rfc1945(const request & req, answer & ans)
{
    string val;
    unsigned int code = ans.get_status_code();


	// HEAD requests must not be answered with a body

    if(req.get_method() == "HEAD")
	ans.drop_body_keep_header();

	// Conditional GET

    if(req.get_method() == "GET" && req.find_attribute(HDR_IF_MODIFIED_SINCE, val))
    {
	try
	{
	    date when = val;
	    string lastmod;

	    if(ans.find_attribute(HDR_LAST_MODIFIED, lastmod))
	    {
		date last = lastmod;
		if(last <= when && ans.get_status_code() == STATUS_CODE_OK)
		{
		    ans.set_status(STATUS_CODE_NOT_MODIFIED);
		    ans.add_body("");
		    // we can drop the body from the answer
		    // as it has not been modifed since last
		    // seen by this client
		}
	    }
	}
	catch(exception_bug & e)
	{
	    throw;
	}
	catch(exception_range & e)
	{
		// ignore If-modified-Since request header in case of error
		// that's it: inconditionnaly send the answer's body
	}
    }


	// RFC 1945 defines that several status code must not own a body in the answer

    if(code == STATUS_CODE_NO_CONTENT
       || code  == STATUS_CODE_NOT_MODIFIED
       || (code > 99 && code < 200))
    {
	if(ans.get_body().size() > 0)
	    throw WEBDAR_BUG;
	    // these responses must not include a body
    }
}
