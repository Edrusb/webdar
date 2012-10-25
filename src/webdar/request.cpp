#include "request.hpp"
#include "exceptions.hpp"
#include "webdar_tools.hpp"

using namespace std;


    /////////////////////////////////////////////////////////////////////////////////////
    // cookie_list implementation
    //

bool cookie_list::find(const std::string & key, std::string & value) const
{
    map<string, string>::const_iterator it = contents.find(key);

    if(it != contents.end())
    {
	value = it->second;
	return true;
    }
    else
	return false;
}


    /////////////////////////////////////////////////////////////////////////////////////
    // request implementation
    //


bool request::try_reading(connexion & input)
{
    read_method_uri(input, false);
}

bool request::read_method_uri(connexion & input, bool blocking)
{
    string tmp;

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
	    {
		coordinates.read(cached_uri);
		if(coordinates.size() < 2)
		    throw WEBDAR_BUG;
		    // uri_split should return: (scheme) + (hostname or empty string)
	    }
	}
    }

    switch(status)
    {
    case init:
    case method_read:
    case uri_read:
	break;
    case completed:
	throw WEBDAR_BUG;
    default:
	throw WEBDAR_BUG;
    }

    return status == uri_read;
}

void request::read(connexion & input)
{
    string key;
    string val;

    if(&input == NULL)
	throw WEBDAR_BUG;

	///////////////////////////////////////////
	// reading the first line of the request
	//

    if(!read_method_uri(input, true))
	throw WEBDAR_BUG;

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

    if(coordinates[0] != "http" && coordinates[0] != "")
    {
	string mesg = "Unsupported scheme in URI: ";
	mesg += coordinates[0];

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


bool request::find_attribute(const std::string & key, std::string & value) const
{
    string lkey = webdar_tools_to_lowercase(key);
    multimap<string, string>::const_iterator it = attributes.find(lkey);

    if(status != completed)
	throw WEBDAR_BUG;

    if(it != attributes.end())
    {
	value = it->second;
	return true;
    }
    else
	return false;
}

cookie_list request::extract_cookies() const
{
    cookie_list ret;
    pair< multimap<string,string>::const_iterator, multimap<string,string>::const_iterator > range = attributes.equal_range("Cookie");
    multimap<string,string>::const_iterator it = range.first;

    while(it != range.second)
    {
	vector<string> splitted;
	vector<string>::iterator ut;

	webdar_tools_split_by(';', it->second, splitted);
	ut = splitted.begin();
	while(ut != splitted.end())
	{
	    string key, val;

	    webdar_tools_split_in_two('=', *ut, key, val);
	    key = webdar_tools_remove_leading_spaces(key);
	    ret.add(key, val);
	    ++ut;
	}
	++it;
    }

    return ret;
}

bool request::is_empty_line(connexion & input)
{
    bool ret;

    try
    {
	if(&input == NULL)
	    throw WEBDAR_BUG;
	ret = (input.read_test_first(true) == '\r') && (input.read_test_second(true) == '\n');
    }
    catch(exception_base & e)
    {
	ret = false;
	    // no throw!
    }

    return ret;
}

string request::up_to_eol(connexion & input)
{
    string ret;
    bool loop = true;
    char a;

    if(&input == NULL)
	throw WEBDAR_BUG;

    try
    {
	while(input.read_test_first(true) != '\r' || input.read_test_second(true) != '\n')
	    ret += input.read_one(true);
	skip_line(input);
    }
    catch(exception_base & e)
    {
	    // we ignore here any end of connection
    }
    return ret;
}

string request::up_to_eof(connexion & input)
{
    string ret;

    if(&input == NULL)
	throw WEBDAR_BUG;

    try
    {
	while(true)
	    ret += input.read_one(true);
    }
    catch(exception_base & e)
    {
	    // this is the way we know that we reached eof
    }

    return ret;
}

void request::skip_over(connexion & input, char a)
{
    if(&input == NULL)
	throw WEBDAR_BUG;

    try
    {
	while(input.read_one(true) != a)
	    ;
    }
    catch(exception_base & e)
    {
	    // we ignore here any end of connection
    }
}


string request::up_to_length(connexion & input, unsigned int length)
{
    string ret;

    if(&input == NULL)
	throw WEBDAR_BUG;

	/// TO ME IMPROVED ADDING A READ CALL WITH INPUT BUFFER TO CONNEXION CLASS
    while(--length >= 0)
	ret += input.read_one(true);

    return ret;
}

void request::skip_line(connexion & input)
{
    bool loop = true;
    char a;

    if(&input == NULL)
	throw WEBDAR_BUG;
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
    catch(exception_base & e)
    {
	    // we ignore here any end of connection
    }
}

string request::up_to_eol_with_LWS(connexion & input)
{
    string ret;
    bool loop = false;

    if(&input == NULL)
	throw WEBDAR_BUG;

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
    catch(exception_range & e)
    {
	    // nothing done, as we reached end of file
	    // we return the data read so far
    }

    return ret;
}


bool request::get_token(connexion & input, bool initial, bool blocking, string & token)
{
    bool ret = true;
    bool loop = true;
    char a;
    token = "";

    if(&input == NULL)
	throw WEBDAR_BUG;

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
    catch(exception_base & e)
    {
	if(!blocking)
	    ret = false;
	    // else (blocking read) nothing to do, we just reach the end of file
	    // so we return the so far read data
    }

    return ret;
}

bool request::get_word(connexion & input, bool initial, bool blocking, string & word)
{
    string tmp;
    char ctmp;
    bool loop = false;
    bool ret = false;

    if(&input == NULL)
	throw WEBDAR_BUG;

    word = "";
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
		word += input.read_one(blocking);
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
