extern "C"
{
#include <ctype.h>
#include <time.h>
}

    // libraries header files
#include <libdar/libdar.hpp>

    // C++ system header files
#include <iostream>

    // webdar headers
#include "webdar_tools.hpp"
#include "exceptions.hpp"

using namespace std;

static string convert_to_string_two_digits(int val);

int webdar_tools_convert_to_int(const string & ref)
{
    int ret;
    stringstream buf;

    buf.str(ref);
    buf >> ret;
    if(buf.rdstate() != ios_base::goodbit && buf.rdstate() != ios_base::eofbit)
	throw exception_range(libdar::tools_printf("Cannot convert %S to integer", &ref));

    return ret;
}

void webdar_tools_split_by(char sep, const string & aggregate, vector<string> & splitted)
{
    splitted.clear();

    string::const_iterator it = aggregate.begin();
    string::const_iterator bk = it;

    while(it != aggregate.end())
    {
	if(*it == sep)
	{
	    splitted.push_back(string(bk, it));
	    ++it;
	    bk = it;
	}
	else
	    ++it;
    }

    if(bk != aggregate.end())
	splitted.push_back(string(bk, it));
}


void webdar_tools_split_in_two(char sep, const string &aggregate, string & first, string & second)
{
    string::const_iterator it = aggregate.begin();

    while(it != aggregate.end() && *it != sep)
	++it;

    first = string(aggregate.begin(), it);
    if(it != aggregate.end())
	++it;
    second = string(it, aggregate.end());
}

string webdar_tools_remove_leading_spaces(const string & input)
{
    string::const_iterator it = input.begin();

    while(it != input.end() && *it == ' ')
	++it;

    return string(it, input.end());
}

void webdar_tools_init_randomization()
{
    int pas = getpid() % getppid();
    srand(time(NULL));

    if(pas == 0)
	pas = 10;
    for(unsigned int i = getpid() ; i > pas ; i -= pas)
	(void)rand();
}

string webdar_tools_generate_random_string(unsigned int size)
{
    string ret = "";
    unsigned int x;

    for(unsigned int i = 0; i < size; ++i)
    {
	x = rand() % 62;
	if(x < 10)
	    ret += char(x + 48); // digits 0 - 9
	else
	    if(x < 36) // 10 <= x < 36
		ret += char(55 + x); // uppercase letters A - Z
	    else  // 36 <= x < 61
		ret += char(61 + x); // lowercase letters a - z
    }

    return ret;
}

string webdar_tools_get_session_ID_from_URI(const uri & url)
{
    if(url.size() > 2)
	return url[2];
    else
	return "";
}


string webdar_tools_to_lowercase(const string & ch)
{
    string ret = "";

    for(string::const_iterator it = ch.begin(); it != ch.end(); ++it)
    {
	if(isalpha((*it)))
	    ret += tolower(*it);
	else
	    ret += *it;
    }

    return ret;
}

string webdar_tools_get_current_date()
{
    string ret;
    struct tm bd;
    time_t brut;

    (void)time(&brut);
    (void)gmtime_r(&brut, &bd);

    switch(bd.tm_wday)
    {
    case 0:
	ret += "Sun";
	break;
    case 1:
	ret += "Mon";
	break;
    case 2:
	ret += "Tue";
	break;
    case 3:
	ret += "Wed";
	break;
    case 4:
	ret += "Thu";
	break;
    case 5:
	ret += "Fri";
	break;
    case 6:
	ret += "Sat";
	break;
    default:
	throw WEBDAR_BUG;
    }

    ret += ", ";
    ret += convert_to_string_two_digits(bd.tm_mday);
    ret += " ";

    switch(bd.tm_mon)
    {
    case 0:
	ret += "Jan";
	break;
    case 1:
	ret += "Feb";
	break;
    case 2:
	ret += "Jan";
	break;
    case 3:
	ret += "Mar";
	break;
    case 4:
	ret += "Apr";
	break;
    case 5:
	ret += "May";
	break;
    case 6:
	ret += "Jun";
	break;
    case 7:
	ret += "Jul";
	break;
    case 8:
	ret += "Aug";
	break;
    case 9:
	ret += "Sep";
	break;
    case 10:
	ret += "Oct";
	break;
    case 11:
	ret += "Nov";
	break;
    case 12:
	ret += "Dec";
	break;
    default:
	throw WEBDAR_BUG;
    }

    ret += " ";
    ret += webdar_tools_convert_to_string(bd.tm_year + 1900);
    ret += " ";
    ret += convert_to_string_two_digits(bd.tm_hour);
    ret += ":";
    ret += convert_to_string_two_digits(bd.tm_min);
    ret += ":";
    ret += convert_to_string_two_digits(bd.tm_sec);
    ret += " GMT";

    return ret;
}


static string convert_to_string_two_digits(int val)
{
    string ret;

    if(val > 99 || val < 0)
	throw WEBDAR_BUG;

    if(val < 10)
	ret += "0";
    ret += webdar_tools_convert_to_string(val);

    return ret;
}
