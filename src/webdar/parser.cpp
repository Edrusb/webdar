#include "webdar_tools.hpp"
#include "parser.hpp"

using namespace std;

#define BUFFER_SIZE 10240
#ifdef SSIZE_MAX
#if SSIZE_MAX < BUFFER_SIZE
#undef BUFFER_SIZE
#define BUFFER_SIZE SSIZE_MAX
#endif
#endif

parser::parser(connexion *input, central_report *log)
{
    if(input == NULL)
	throw WEBDAR_BUG;

    if(log == NULL)
	throw WEBDAR_BUG;

    if(input->get_status() != connexion::connected)
	throw exception_range("connection is already closed cannot read from it");

    source = input;
    buffer_size = BUFFER_SIZE;
    buffer = NULL;
    already_read = 0;
    data_size = 0;
    answered = true;
    maj_vers = 0;
    min_vers = 0;
    clog = log;
    close_asked_on_request = false;
    cached_method_completed = false;
    cached_uri_completed = false;

    buffer = new (nothrow) char[buffer_size];
    if(buffer == NULL)
	throw exception_memory();
}

parser::~parser()
{
    if(source != NULL)
	delete source;
    if(buffer != NULL)
	delete buffer;
}

bool parser::has_pending_request(uri & url)
{
    string tmp;

	// reading a single line from the connection
	// if so far no end of line could be read, there is no pending request ready for reading
	// a pending request is ready as soon as we get enough information to determin the
	// session_ID associated to that request
    if(answered == false)
	throw WEBDAR_BUG;

    if(!cached_method_completed)
    {
	if(get_token(cached_method == "", false, tmp))
	    cached_method_completed = true;
	cached_method += tmp;
    }

    if(cached_method_completed)
    {
	if(!cached_uri_completed)
	{
	    if(get_word(cached_uri == "", false, tmp))
		cached_uri_completed = true;
	    cached_uri += tmp;
	}
    }

    if(cached_uri_completed)
	url = uri_split(cached_method);

    return cached_uri_completed;
}

request parser::get_next_request()
{
    request ret;
    answer err_ans;

	// first level analysis / tokens
    string meth;
    string resource;
    string version;


	// splitted token
    uri res;

    if(!valid_source())
	throw exception_range("connection closed, cannot read request from it");

    if(!answered)
	throw WEBDAR_BUG; // last request has not been answred
    answered = false;

    try
    {
	try
	{
	    string key;
	    string val;
	    string body;

		// reading the first line of the request

	    if(cached_method_completed)
	    {
		meth = cached_method;
		cached_method = "";
		cached_method_completed = false;
	    }
	    else
	    {
		if(!get_token(cached_method == "", true, meth))
		    throw WEBDAR_BUG;
		meth = cached_method + meth;
		cached_method = "";
	    }

	    if(cached_uri_completed)
	    {
		resource = cached_uri;
		cached_uri = "";
		cached_uri_completed = false;
	    }
	    else
	    {
		if(!get_word(cached_uri == "", true, resource))
		    throw WEBDAR_BUG;
		resource = cached_uri + resource;
		cached_uri = "";
	    }

	    if(!get_word(true, true, version))
		throw WEBDAR_BUG;

	    if(version == "")
		version = "HTTP/0.9";

	    res = uri_split(resource);
	    if(res.size() < 2)
		throw WEBDAR_BUG;
		// uri_split should return: (scheme) + (hostname or empty string)

	    if(res[0] != "http" && res[0] != "")
		throw exception_range("unsupported scheme in URI");

		// HTTP version control

	    set_version(version);

	    if(maj_vers != 1 || (min_vers != 0 && min_vers != 1))
	    {
		maj_vers = 1;
		min_vers = 0;
		err_ans.set_status_reason(STATUS_CODE_NOT_IMPLEMENTED, version);
		clog->report(info, "Received request using an unsupported HTTP version: " + version);
		throw exception_input("unsupported HTTP version");
	    }

		// HTTP method control

	    if(meth != "GET" && meth != "POST" && meth != "HEAD")
	    {
		err_ans.set_status_reason(STATUS_CODE_NOT_IMPLEMENTED, version);
		clog->report(debug, "Received request using an unknown method: " + meth);
		throw exception_input("unknown method");
	    }

	    ret.set_method_url(meth, res);
	    skip_line();

		// reading the rest of the request header

	    while(!is_empty_line()) // which would mean the end of the header
	    {
		if(!get_token(true, true, key))
		    throw WEBDAR_BUG;
		if(key == "")
		{
		    err_ans.set_status_reason(STATUS_CODE_BAD_REQUEST, "non RFC1945 conformant message header");
		    throw exception_input("syntax error in request header");
		}
		skip_over(':');
		skip_over(' ');
		val = up_to_eol_with_LWS();
		ret.add_attribute(key, val);
	    }
	    skip_line(); // we now point to the beginning of the body

		// reading the body

	    if(meth == "POST")
	    {
		string length;
		unsigned int size;

		if(!ret.find_attribute("Content-Length", length))
		{
 		    err_ans.set_status_reason(STATUS_CODE_BAD_REQUEST, "missing Content-Length in header");
		    throw exception_input("syntax error in request header");
		}
		else
		{
		    try
		    {
			size = webdar_tools_convert_to_int(length);
		    }
		    catch(...)
		    {
			err_ans.set_status_reason(STATUS_CODE_BAD_REQUEST, "Invalid value given to Content-Length in header");
			throw exception_input("syntax error in request header");
		    }
		}
		body = up_to_length(size);
		ret.add_body(body);
	    }
	}
	catch(exception_input & e)
	{
	    if(err_ans.is_empty())
		throw WEBDAR_BUG;
	    send_answer(err_ans);
	    throw;
	}
    }
    catch(exception_base & e)
    {
	if(source != NULL)
	{
	    delete source;
	    source = NULL;
	}
	answered = true;
	throw;
    }

    req = ret;
    return ret;
}

void parser::send_answer(answer & ans)
{
    string code = webdar_tools_convert_to_string(ans.get_status_code());
    string key, val;

    if(answered)
	throw WEBDAR_BUG;

    try
    {
	if(ans.is_empty())
	    throw WEBDAR_BUG;

	    ////////////////////////////////////////
	    // conforming the answer to RFC 1945
	    //

	if(code.size() < 3)
	    throw WEBDAR_BUG;

	if(req.get_method() == "HEAD")
	    if(ans.get_body().size() != 0)
		ans.add_body(""); // drop the body from the answer

	if(code == "204"
	   || code  == "304"
	   || code[0] == '1')
	{
	    if(ans.get_body().size() > 0)
		throw WEBDAR_BUG;
	       // these responses must not include a body
	}
	else
	    ans.add_attribute("Content-Length", webdar_tools_convert_to_string(ans.get_body().size()));

    if(!ans.find_attribute("Content-Type", val))
	ans.add_attribute("Content-Type", "text/html");

    if(!ans.find_attribute("Date", val))
	ans.add_attribute("Date", webdar_tools_get_current_date());
    if(!ans.find_attribute("Expires", val))
	ans.add_attribute("Expires", webdar_tools_get_current_date());

    ans.add_attribute("Server", "webdar");

	    ////////////////////////////////////////
	    // sending the answer
	    //

    string version = "HTTP/"
	+ webdar_tools_convert_to_string(maj_vers)
	+"."
	+ webdar_tools_convert_to_string(min_vers);
    source->write(version.c_str(), version.size());
    source->write(" ", 1);
    source->write(code.c_str(), code.size());
    source->write(" ", 1);
    source->write(ans.get_reason().c_str(), ans.get_reason().size());
    source->write("\r\n", 2);

    ans.reset_read_next_attribute();
    while(ans.read_next_attribute(key, val))
    {
	source->write(key.c_str(), key.size());
	source->write(": ", 2);
	source->write(val.c_str(), val.size());
	source->write("\r\n", 2);
    }
    source->write("\r\n", 2);
    if(ans.get_body().size() > 0)
	source->write(ans.get_body().c_str(), ans.get_body().size());
    }
    catch(exception_base & e)
    {
	answered = true;
	throw;
    }
    answered = true;
}

void parser::fill_buffer(bool blocking)
{
    if(data_size < buffer_size) // there is some room to receive more data in the buffer
    {
	if(source == NULL)
	    throw WEBDAR_BUG;
	if(source->get_status() == connexion::connected)
	{
	    if(already_read == data_size)   // no more data to read, so we restart at the beginning
		already_read = data_size = 0;
	    else
	    {
		if(already_read > data_size)
		    throw WEBDAR_BUG;
		if(data_size > buffer_size)
		    throw WEBDAR_BUG;
		if(already_read > 0)
		    (void)memmove(buffer, buffer + already_read, data_size - already_read);
	    }
	    if(blocking)
		data_size += source->read(buffer + data_size, buffer_size - data_size);
	    else
		data_size += source->non_blocking_read(buffer + data_size, buffer_size - data_size);
	}
    }
}

char parser::read_one(bool blocking)
{
    if(already_read == data_size)
	fill_buffer(blocking);
    if(already_read == data_size)
	throw exception_range("no more data available from connection");

    return buffer[already_read++];
}

char parser::read_test_first(bool blocking)
{
    if(already_read == data_size)
	fill_buffer(blocking);
    if(already_read == data_size)
	throw exception_range("no more data available from connection");

    return buffer[already_read];
}

char parser::read_test_second(bool blocking)
{
    if(data_size - already_read < 2)
	fill_buffer(blocking);
    if(data_size - already_read < 2)
	throw exception_range("no more data available from connection");

    return buffer[already_read + 1];
}

bool parser::is_empty_line()
{
    bool ret;

    try
    {
	ret = (read_test_first(true) == '\r') && (read_test_second(true) == '\n');
    }
    catch(exception_base & e)
    {
	ret = false;
	    // no throw!
    }

    return ret;
}

bool parser::get_token(bool initial, bool blocking, string & token)
{
    bool ret = true;
    bool loop = true;
    char a;
    token = "";

    try
    {
	while(loop)
	{
	    a = read_test_first(blocking);

	    if((a == ' ' || a == '\t') && initial)
		a = read_one(blocking);
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
		    token += read_one(blocking);
		    initial = false;
		}
		else
		    loop = false;
	    }
	}
    }
    catch(exception_base & e)
    {
	if(!blocking)
	    ret = false;
	    // else (blocking read) nothing to do, we just reach the end of file
	    // so we return the so far read data
    }

    return ret;
}

bool parser::get_word(bool initial, bool blocking, string & word)
{
    string tmp;
    char ctmp;
    bool loop = false;
    bool ret = false;

    word = "";
    do
    {
	loop = false;
	ret = get_token(initial, blocking, tmp);
	if(tmp != "")
	{
	    initial = false;
	    word += tmp;
	}
	if(ret)
	{
	    try
	    {
		ctmp = read_test_first(blocking);
	    }
	    catch(...)
	    {
		    // get_token() could define that the token is completed
		    // this it can read at least one byte further
		    // so read_test_first() has no reason to throw an exception
		throw WEBDAR_BUG;
	    }
	    switch(ctmp)
	    {
	    case '/':
	    case ':':
	    case '=':
	    case '@':
	    case '?':
		word += read_one(blocking);
		initial = false;
		loop = true;
		break;
	    defaut:
		break; // nothing to do, as loop is already false
	    }
	}
    }
    while(loop);

    return ret;
}

uri parser::uri_split(const string & res)
{
    uri ret;
    enum { scheme, hostname, path } lookup = scheme;
    string::const_iterator it = res.begin();
    string::const_iterator bk = it;

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
		    ret.push_back(""); // scheme not provided
		    lookup = hostname;
		    it = bk;
		}
		else
		{
		    ret.push_back(string(bk, it));
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
		    ret.push_back(""); // empty host, the URL is not a net_path
		    bk = it;
		    lookup = path;
		}
	    }
	    else // just a relative path
	    {
		ret.push_back(""); // empty host, the URL is not a net_path
		bk = it;
		lookup = path;
	    }
	    break;
	case path:
	    if(*it == '/')
	    {
		ret.push_back(string(bk, it));
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
	ret.push_back("");
	    // no break;
    case hostname:
	ret.push_back("");
	break;
    case path:
	break;
    default:
	throw WEBDAR_BUG;
    }

    if(bk != it)
	ret.push_back(string(bk, it));

    return ret;
}

void parser::set_version(const string & version)
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
	throw exception_range("badly formated HTTP version field");
    bk = it;

    while(it != version.end() && *it != '.')
	++it;

    if(it == version.end())
	throw exception_range("badly formated HTTP version field");

    maj_vers = webdar_tools_convert_to_int(string(bk, it));
    ++it;
    if(it == version.end())
	throw exception_range("badly formated HTTP version field");
    min_vers = webdar_tools_convert_to_int(string(it, version.end()));
}

void parser::skip_line()
{
    bool loop = true;
    char a;

    try
    {

	while(loop)
	{
	    a = read_one(true);
	    if(a == '\r')
	    {
		if(read_test_first(true) == '\n')
		{
		    a = read_one(true);
		    loop = false;
		}
	    }
	}

    }
    catch(exception_base & e)
    {
	    // we ignore here any end of connection
    }
}

string parser::up_to_eol()
{
    string ret;
    bool loop = true;
    char a;

    try
    {
	while(read_test_first(true) != '\r' || read_test_second(true) != '\n')
	    ret += read_one(true);
	skip_line();
    }
    catch(exception_base & e)
    {
	    // we ignore here any end of connection
    }
    return ret;
}

string parser::up_to_eol_with_LWS()
{
    string ret;
    bool loop = false;

    try
    {
	do
	{
	    loop = false;
	    ret += up_to_eol();
	    if(read_test_first(true) == ' ' || read_test_first(true) == '\t')
	    {
		loop = true;
		while(read_test_first(true) == ' ' || read_test_first(true) == '\t')
		    (void)read_one(true);
		ret += ' '; // "All LWS including folding, have the same semantics as SP"
	    }
	}
	while(loop);
    }
    catch(exception_range & e)
    {
	    // nothing done, as we reached end of file
	    // we return the data read so far
    }

    return ret;
}

void parser::skip_over(char a)
{
    try
    {
	while(read_one(true) != a)
	    ;
    }
    catch(exception_base & e)
    {
	    // we ignore here any end of connection
    }

}

string parser::up_to_eof()
{
    string ret;

    try
    {
	while(true)
	    ret += read_one(true);
    }
    catch(exception_base & e)
    {
	    // this is the way we know that we reached eof
    }

    return ret;
}

string parser::up_to_length(unsigned int length)
{
    string ret;

    while(--length >= 0)
	ret += read_one(true);

    return ret;
}
