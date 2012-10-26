#include "webdar_tools.hpp"
#include "date.hpp"
#include "parser.hpp"

using namespace std;


parser::parser(connexion *input, central_report *log): req(log)
{
    if(input == NULL)
	throw WEBDAR_BUG;

    if(log == NULL)
	throw WEBDAR_BUG;

    if(input->get_status() != connexion::connected)
	throw exception_range("connection is already closed cannot read from it");

    clog = log;
    answered = true;
    source = input;
}

parser::~parser()
{
    if(source != NULL)
	delete source;
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
	if(source != NULL)
	{
	    delete source;
	    source = NULL;
	}
	    // no throw
	    // answered stays false
    }

    return req;
}

void parser::send_answer(answer & ans)
{
    if(answered)
	throw WEBDAR_BUG;
    valid_source();

    try
    {
	if(ans.is_empty())
	    throw WEBDAR_BUG;
	checks_main(req, ans);
	ans.write(*source);
	answered = true;
    }
    catch(exception_base & e)
    {
	if(source != NULL)
	{
	    delete source;
	    source = NULL;
	}
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

	// adding a server header
    ans.add_attribute(HDR_SERVER, "webdar/0.0");

	// responding with the same version as the one used in the request
    ans.set_version(req.get_maj_version(), req.get_min_version());

	// adding a Date header if missing
    if(!ans.find_attribute("Date", val))
	ans.add_attribute("Date", date().get_canonical_format());

	// adding an Expires header if missing
    if(!ans.find_attribute("Expires", val))
	ans.add_attribute("Expires", date().get_canonical_format());

	// adding a default text/html content type if not specified
    if(ans.get_body().size() > 0)
    {
	if(!ans.find_attribute(HDR_CONTENT_TYPE, val))
	    ans.add_attribute(HDR_CONTENT_TYPE, "text/html");
    }
}

void parser::checks_rfc1945(const request & req, answer & ans)
{
    string val;
    unsigned int code = ans.get_status_code();


	// HEAD requests must not be answered with a body

    if(req.get_method() == "HEAD")
	if(ans.get_body().size() != 0)
	    ans.add_body(""); // drop the body from the answer


	// Conditional GET

    if(req.get_method() == "GET" && req.find_attribute("If-modified-Since", val))
    {
	try
	{
	    date when = val;
	    string lastmod;

	    if(ans.find_attribute("Last-Modified", lastmod))
	    {
		date last = lastmod;
		if(last <= when)
		    ans.add_body("");
		    // we can drop the body from the answer
		    // as it has not been modifed since last
		    // seen by this client
	    }
	}
	catch(exception_range & e)
	{
		// ignore If-modified-Since request header in case of error
		// that's it: inconditionnaly send the answer's body
	}
    }


	// RFC 1945 defines that several status code must not own a body in the answer

    if(code == 204
       || code  == 304
       || (code > 100 && code < 200))
    {
	if(ans.get_body().size() > 0)
	    throw WEBDAR_BUG;
	    // these responses must not include a body
    }
    else
    {
	if(!ans.find_attribute(HDR_CONTENT_LENGTH, val))
	    ans.add_attribute(HDR_CONTENT_LENGTH, webdar_tools_convert_to_string(ans.get_body().size()));
    }
}



