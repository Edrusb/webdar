#include "answer.hpp"
#include "exceptions.hpp"
#include "webdar_tools.hpp"

using namespace std;

bool answer::find_attribute(const string & key, string & value) const
{
    string lkey = webdar_tools_to_lowercase(key);
    multimap<string,string>::const_iterator it = attributes.find(lkey);

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


void answer::write(connexion & output)
{
    string key, val;

    if(maj_vers != 1 || (min_vers != 0 && min_vers != 1))
       throw WEBDAR_BUG;

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
