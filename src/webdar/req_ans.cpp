#include "req_ans.hpp"
#include "exceptions.hpp"

using namespace std;

request::request(const std::string & method, const uri & url)
{
    meth = method;
    coordinates = url;
}

void request::add_attribute(const std::string & key, const std::string & value)
{
    attributes[key] = value;
}

void request::add_body(const std::string & key)
{
    body = key;
}

bool request::find_attribute(const std::string & key, std::string & value) const
{
    map<string, string>::const_iterator it = attributes.find(key);

    if(it != attributes.end())
    {
	value = it->second;
	return true;
    }
    else
	return false;
}

    /////////////////////////////////////////////////////////////////////////////////////


answer::answer(unsigned int status_code, const string & reason_phrase)
{
    status = status_code;
    reason = reason_phrase;
    next_read = attributes.begin();
}


bool answer::find_attribute(const string & key, string & value) const
{
    map<string,string>::const_iterator it = attributes.find(key);

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
