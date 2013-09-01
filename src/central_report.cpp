/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013 Denis Corbin
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

    // webdar headers

#include "webdar_tools.hpp"
#include "exceptions.hpp"
    //
#include "central_report.hpp"

static const char *priority2string(priority_t prio);
static int priority2syslogprio(priority_t prio);

using namespace std;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

mutex central_report::access;

void central_report::report(priority_t priority, const std::string & message)
{
    if(priority > min)
	return; // not logging "below" priority min

    access.lock();

    try
    {
	inherited_report(priority, message);
    }
    catch(...)
    {
	access.unlock();
	throw;
    }
    access.unlock();
}

    /////////////////////////////////////////////////////////////////////////////////////////////////////

void central_report_stdout::inherited_report(priority_t priority, const std::string & message)
{
    cout << "[thead " << pthread_self() << "][" << priority2string(priority) << "]   " << message << endl;
}

    /////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int central_report_syslog::num_obj = 0;
mutex central_report_syslog::num_obj_mod;

central_report_syslog::central_report_syslog(priority_t min_logged, const std::string & fixed_label, int facility): central_report(min_logged)
{
    num_obj_mod.lock();
    try
    {
	if(num_obj > 0)
	    throw WEBDAR_BUG;
	else
	{
	    label = fixed_label;
	    openlog(label.c_str(), LOG_NOWAIT, facility);
	    ++num_obj;
	}
    }
    catch(...)
    {
	num_obj_mod.unlock();
	throw;
    }
    num_obj_mod.unlock();
}

central_report_syslog::~central_report_syslog()
{
    num_obj_mod.lock();
    try
    {
	closelog();
	if(num_obj == 0)
	    throw WEBDAR_BUG;
	else
	    --num_obj;
    }
    catch(...)
    {
	num_obj_mod.unlock();
	throw;
    }
    num_obj_mod.unlock();
}

void central_report_syslog::inherited_report(priority_t priority, const std::string & message)
{
    std::string tmp = string("[") + label + "][" + webdar_tools_convert_to_string(pthread_self()) +"]  " + message;
    syslog(priority2syslogprio(priority), tmp.c_str());
}


    /////////////////////////////////////////////////////////////////////////////////////////////////////

static const char *priority2string(priority_t prio)
{
    const char *ret = NULL;

    switch(prio)
    {
    case crit:
	ret = "critical";
	break;
    case err:
	ret = "error";
	break;
    case warning:
	ret = "warning";
	break;
    case notice:
	ret = "notice";
	break;
    case info:
	ret = "information";
	break;
    case debug:
	ret = "debug";
	break;
    default:
	throw WEBDAR_BUG;
    }

    if(ret == NULL)
	throw WEBDAR_BUG;
    return ret;
}

static int priority2syslogprio(priority_t prio)
{
    int ret = 0;

    switch(prio)
    {
    case crit:
	ret = LOG_CRIT;
	break;
    case err:
	ret = LOG_ERR;
	break;
    case warning:
	ret = LOG_WARNING;
	break;
    case notice:
	ret = LOG_NOTICE;
	break;
    case info:
	ret = LOG_INFO;
	break;
    case debug:
	ret = LOG_DEBUG;
	break;
    default:
	throw WEBDAR_BUG;
    }

    return ret;
}
