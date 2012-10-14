#include "webdar_tools.hpp"
#include "parser.hpp"

using namespace std;

parser::parser(connexion *input, central_report *log)
{
    if(input == NULL)
	throw WEBDAR_BUG;

    if(log == NULL)
	throw WEBDAR_BUG;

    if(input->get_status() != connexion::connected)
	throw exception_range("connection is already closed cannot read from it");

    clog = log;
    source = input;
}

parser::~parser()
{
    if(source != NULL)
	delete source;
}

request parser::get_next_request()
{
    unsigned int body_length = 0;
    request ret;

	// first level analysis / tokens
    string meth;
    string resource;
    string version;


	// splitted token
    uri res;
    unsigned int maj_vers;
    unsigned int min_vers;

    if(!valid_source())
	throw exception_range("connection closed, cannot read request from it");

    if(!answered)
	throw WEBDAR_BUG; // last request has not been answred
    answered = false;

    try
    {
	string key;
	string val;
	string body;

	    // reading the first line of the request

	meth = get_token();
	resource = get_token();
	version = get_token();

	if(version == "")
	    version = "HTTP/0.9";

	res = uri_split(resource);
	if(res.size() < 2)
	    throw WEBDAR_BUG; // uri_split should return: (scheme) + (hostname or empty string)
	if(res[0] != "http")
	    throw exception_range("unsupported scheme in URI");

	set_version(version);
	if(maj_vers != 1)
	{
	    answer ans;

	    ans.set_status_reason(STATUS_CODE_HTTP_VERSION_NOT_SUPPORTED, "HTTP version not supported");
		// add the close token to answ
	    send_answer(ans);
	    throw exception_range("unsupported HTTP version");
	}
	if(min_vers > 1)
	    clog->report(err, "HTTP minor version is higher than what this software is aware of, trying to cope with it anyway: " + version);


	ret.set_method_url(meth, res);
	skip_line();

	    // reading the rest of the header

	while(!is_empty_line())
	{
	    key = get_token();
	    skip_over(':');
	    skip_over(' ');
	    val = up_to_eol();
	    skip_line();
	    ret.add_attribute(key, val);
	    if(key == "Content-length")
		body_length = webdar_tools_convert_to_int(val);
	}

	skip_line();

	    // reading the body

	if(maj_vers == 1 && maj_vers == 0) // HTTP/1.0
	    body = up_to_eof();
	else
	    body = up_to_length(body_length);

	ret.add_body(body);
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

    return ret;
}


void parser::send_answer(const answer & ans)
{
    if(answered)
	throw WEBDAR_BUG;

    try
    {
	if(ans.is_empty())
	    throw WEBDAR_BUG;

	    // check conformance with RFC

	    /// <<<<<< A IMPLEMENTER

	    // sending the answer

	string code = webdar_tools_convert_to_string(ans.get_status_code());
	string key, val;
	source->write(code.c_str(), code.size());
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
	source->write("\r\n\r\n", 4);
	source->write(ans.get_body().c_str(), ans.get_body().size());
    }
    catch(exception_base & e)
    {
	answered = true;
	throw;
    }
    answered = true;
}

void parser::fill_buffer()
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
	    data_size += source->read(buffer + data_size, buffer_size - data_size);
	}
    }
}

char parser::read_one()
{
    if(already_read == data_size)
	fill_buffer();
    if(already_read == data_size)
	throw exception_range("no more data available from connection");

    return buffer[already_read++];
}

char parser::read_test_first()
{
    if(already_read == data_size)
	fill_buffer();
    if(already_read == data_size)
	throw exception_range("no more data available from connection");

    return buffer[already_read];
}

char parser::read_test_second()
{
    if(data_size - already_read < 2)
	fill_buffer();
    if(data_size - already_read < 2)
	throw exception_range("no more data available from connection");

    return buffer[already_read + 1];
}

bool parser::is_empty_line()
{
    bool ret;

    try
    {
	ret = (read_test_first() == '\r') && (read_test_second() == '\n');
    }
    catch(exception_base & e)
    {
	ret = false;
	    // no throw!
    }

    return ret;
}

string parser::get_token()
{
    string ret;
    bool loop = true;
    char a;

    try
    {
	while(loop)
	{
	    a = read_test_first();

	    if(a >= 'a' && a <= 'z'
	       || a >= 'A' && a <= 'Z'
	       || a >= '0' && a <= '9'
	       || a == '+' || a == '-' || a == '.' || a == ':'
	       || a == '$' || a == '_' || a == '!' || a == '*'
	       || a == '\'' || a == '(' || a == ')' || a == ','
	       || a == '/')
		ret += read_one();
	    else
		loop = false;
	}
    }
    catch(exception_base & e)
    {
	    // nothing to do, we just reach the end of the data
	    // so we return the so far read data
    }

    return ret;
}

uri parser::uri_split(const string & res)
{
    uri ret;
    enum { scheme, hostname, path } lookup;
    string::const_iterator it = res.begin();
    string::const_iterator bk = it;

    while(it != res.end())
    {
	switch(lookup)
	{
	case scheme:
	    if(*it != ':')
		++it;
	    else
	    {
		ret.push_back(string(bk, it));
		++it;
		bk = it;
		lookup = hostname;
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
		else // just a relative path
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
    ++it;
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
	    a = read_one();
	    if(a == '\r')
	    {
		if(read_test_first() == '\n')
		{
		    a = read_one();
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
	while(read_test_first() != '\r' || read_test_second() != '\n')
	    ret += read_one();
    }
    catch(exception_base & e)
    {
	    // we ignore here any end of connection
    }
    return ret;
}

void parser::skip_over(char a)
{
    try
    {
	while(read_one() != a)
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
	    ret += read_one();
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
	ret += read_one();

    return ret;
}
