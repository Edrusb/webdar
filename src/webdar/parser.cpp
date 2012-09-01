#include "webdar_tools.hpp"
#include "parser.hpp"

using namespace std;

parser::parser(connexion *input)
{
    if(input == NULL)
	throw WEBDAR_BUG;

    if(input->get_status() != connexion::connected)
	throw exception_range("connection is already closed cannot read from it");

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


	// first level analysis / tokens
    string meth;
    string resource;
    string version;
	// splitted token
    uri res;
    unsigned int maj_vers;
    unsigned int min_vers;

    if(!valid())
	throw exception_range("connection closed, cannot read request from it");

	// reading the first line of the request

    meth = get_token();
    resource = get_token();
    version = get_token();

    res = uri_split(resource);
    split_version(version, maj_vers, min_vers);

    request ret = request(meth, res, maj_vers, min_vers);
    string key;
    string val;

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
    string body;

    if(maj_vers == 1 && maj_vers == 0) // HTTP/1.0
	body = up_to_eof();
    else
	body = up_to_length(body_length);

    ret.add_body(body);

    return ret;
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
		alread_read = data_size = 0;
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
    if(alread_read == data_size)
	throw exception_range("no more data available from connection");

    return buffer[already_read++];
}

char parser::read_test_first()
{
    if(already_read == data_size)
	fill_buffer();
    if(alread_read == data_size)
	throw exception_range("no more data available from connection");

    return buffer[already_read];
}

char parser::read_test_first()
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
    enum { method, hostname, path } lookup;
    string::iterator it = res.begin();
    string::iterator bk = it;

    while(it != res.end())
    {
	switch(lookup)
	{
	case method:
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

void parser::split_version(const string & version, unsigned int maj_vers, unsigned int min_vers)
{
    const char expected[] = { 'H', 'T', 'T', 'P', '/', '\0' };
    unsigned int len = strlen(expected);
    unsigned int i = 0;
    string tmp;
    string

    if(version.size() < len)
	throw exception_range("badly formated HTTP version field");

    while(i < len && expected[i] == version[i])
	++i;

    if(i < len)
	throw exception_range("badly formated HTTP version field");




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

string up_to_length(unsigned int length)
{
    string ret;

    while(--length >= 0)
	ret += read_one();

    return ret;
}
