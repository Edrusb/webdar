#include "req_ans.hpp"
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


void request::set_method_url(const string & method, const uri & url)
{
    meth = method;
    coordinates = url;
}

bool request::find_attribute(const std::string & key, std::string & value) const
{
    multimap<string, string>::const_iterator it = attributes.find(key);

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

    /////////////////////////////////////////////////////////////////////////////////////
    // answer implementation
    //

void answer::set_status_reason(unsigned int status_code, const string & reason_phrase)
{
    status = status_code;
    reason = reason_phrase;
    next_read = attributes.begin();
}


bool answer::find_attribute(const string & key, string & value) const
{
    multimap<string,string>::const_iterator it = attributes.find(key);

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
    answer *me = const_cast<answer *>(this);
    if(me == NULL)
	throw WEBDAR_BUG;
    me->next_read = attributes.begin();
}

bool answer::read_next_attribute(std::string & key, std::string & value) const
{
    answer *me = const_cast<answer *>(this);
    if(me == NULL)
	throw WEBDAR_BUG;

    if(next_read != attributes.end())
    {
	key = next_read->first;
	value = next_read->second;
	++(me->next_read);
	return true;
    }
    else
	return false;
}


void answer::add_cookie(const std::string & key, std::string & value)
{
    static const string xkey = "Set-Cookie";
    string xval = key+"="+value;
    add_attribute(xkey, xval);
}
