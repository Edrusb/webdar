#ifndef CENTRAL_REPORT_HPP
#define CENTRAL_REPORT_HPP

    //  C system header files
extern "C"
{
#include <syslog.h>
#include <pthread.h>
}

    // C++ system header files
#include <string>


    /// means by which webdar event get reported either to stdout or to syslog

enum priority_t { crit, err, warning, notice, info, debug };

class central_report
{
public:
    central_report(priority_t min_logged): min(min_logged) {};
    ~central_report() {};

    void report(priority_t priority, const std::string & message);

protected:
    virtual void inherited_report(priority_t priority, const std::string & message) = 0;

private:
    priority_t min;
    static pthread_mutex_t access; //< caller must acquire mutex before reporting
};


class central_report_stdout : public central_report
{
public:
    central_report_stdout(priority_t min_logged): central_report(min_logged) {};

protected:
    virtual void inherited_report(priority_t priority, const std::string & message);
};

class central_report_syslog : public central_report
{
public:
    central_report_syslog(priority_t min_logged, const std::string & fixed_label, int facility);
    ~central_report_syslog();

protected:
    virtual void inherited_report(priority_t priority, const std::string & message);

private:
    std::string label; //< need to exist during the whole life of the object for syslog(3) to work as expected

	// static fields
    static unsigned int num_obj; //< number of oject created (only one is allowed as syslog routine is not re-entrant
    static pthread_mutex_t num_obj_mod; //< controls the write access to num_obj

};

#endif
