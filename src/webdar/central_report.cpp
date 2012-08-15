
    // webdar headers

#include "webdar_tools.hpp"
#include "exceptions.hpp"
    //
#include "central_report.hpp"

static const char *priority2string(priority_t prio);
static int priority2syslogprio(priority_t prio);

using namespace std;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

pthread_mutex_t central_report::access = PTHREAD_MUTEX_INITIALIZER;

void central_report::report(priority_t priority, const std::string & message)
{
    if(priority > min)
	return; // not logging "below" priority min

    if(pthread_mutex_lock(&access) != 0)
	throw WEBDAR_BUG;

    try
    {
	inherited_report(priority, message);
    }
    catch(...)
    {
	if(pthread_mutex_unlock(&access) != 0)
	    throw WEBDAR_BUG;
	throw;
    }
    if(pthread_mutex_unlock(&access) != 0)
	throw WEBDAR_BUG;
}

    /////////////////////////////////////////////////////////////////////////////////////////////////////

void central_report_stdout::inherited_report(priority_t priority, const std::string & message)
{
    cout << "[thead " << pthread_self() << "][" << priority2string(priority) << "]   " << message << endl;
}

    /////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int central_report_syslog::num_obj = 0;
pthread_mutex_t central_report_syslog::num_obj_mod = PTHREAD_MUTEX_INITIALIZER;

central_report_syslog::central_report_syslog(priority_t min_logged, const std::string & fixed_label, int facility): central_report(min_logged)
{
    if(pthread_mutex_lock(&num_obj_mod) != 0)
	throw WEBDAR_BUG;
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
	if(pthread_mutex_unlock(&num_obj_mod) != 0)
	    throw WEBDAR_BUG;
	throw;
    }
    if(pthread_mutex_unlock(&num_obj_mod) != 0)
	throw WEBDAR_BUG;
}

central_report_syslog::~central_report_syslog()
{
   if(pthread_mutex_lock(&num_obj_mod) != 0)
	throw WEBDAR_BUG;
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
	if(pthread_mutex_unlock(&num_obj_mod) != 0)
	    throw WEBDAR_BUG;
	throw;
    }
    if(pthread_mutex_unlock(&num_obj_mod) != 0)
	throw WEBDAR_BUG;
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
