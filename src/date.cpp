/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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

    // C system header files
#include "my_config.h"
extern "C"
{
#if HAVE_ERRNO_H
#include <errno.h>
#endif
}

    // C++ system header files


    // webdar headers
#include "exceptions.hpp"
#include "webdar_tools.hpp"
    //
#include "date.hpp"

using namespace std;

static string convert_to_string_two_digits(int val);

date::date()
{
    time_t tmp;
    (void)time(&tmp);
    if(tmp == ((time_t) -1))
	throw exception_system("Error met while trying to get current time", errno);
    if(gmtime_r(&tmp, &val) == nullptr)
	throw exception_system("Error met while trying to brake down time", errno);
}

date::date(const string & when)
{
    val.tm_sec = val.tm_min = val.tm_hour = 0;
    val.tm_mday = val.tm_mon = val.tm_year = 0;
    val.tm_wday = val.tm_yday = val.tm_isdst = 0;
    if(strptime(when.c_str(), "%a, %d %b %Y %H:%M:%S GMT", &val) == nullptr)
	if(strptime(when.c_str(), "%A, %d-%b-%y %H:%M:%S GMT", &val) == nullptr)
	    if(strptime(when.c_str(), "%a %b %d %H:%M:%S %Y", &val) == nullptr)
		throw exception_range("Badly formated date");
}


string date::get_canonical_format() const
{
    string ret;

    switch(val.tm_wday)
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
    ret += convert_to_string_two_digits(val.tm_mday);
    ret += " ";

    switch(val.tm_mon)
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
    ret += webdar_tools_convert_to_string(val.tm_year + 1900);
    ret += " ";
    ret += convert_to_string_two_digits(val.tm_hour);
    ret += ":";
    ret += convert_to_string_two_digits(val.tm_min);
    ret += ":";
    ret += convert_to_string_two_digits(val.tm_sec);
    ret += " GMT";

    return ret;
}

bool date::operator < (const date & ref) const
{
    bool res;

    if(webdar_tools_semi_less_than(val.tm_year, ref.val.tm_year, res))
	return res;
    if(webdar_tools_semi_less_than(val.tm_yday, ref.val.tm_yday, res))
	return res;
    if(webdar_tools_semi_less_than(val.tm_hour, ref.val.tm_hour, res))
	return res;
    if(webdar_tools_semi_less_than(val.tm_min, ref.val.tm_min, res))
	return res;
    if(webdar_tools_semi_less_than(val.tm_sec, ref.val.tm_sec, res))
	return res;
    return false; // this is equal to ref
}

bool date::operator == (const date & ref) const
{
    struct tm c1 = val;
    struct tm c2 = ref.val;
    int d1 = mktime(&c1);
    int d2 = mktime(&c2);

    return d1 == d2; // yes, d1 or d2 may be set to -1 in case of error
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


