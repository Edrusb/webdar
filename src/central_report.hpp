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

#ifndef CENTRAL_REPORT_HPP
#define CENTRAL_REPORT_HPP

    //  C system header files
#include "my_config.h"
extern "C"
{
#if HAVE_SYSLOG_H
#include <syslog.h>
#endif

#if HAVE_PTHREAD_H
#include <pthread.h>
#endif
}

    // C++ system header files
#include <string>
#include <libthreadar/libthreadar.hpp>

    // webdar headers

    /// means by which webdar event get reported either to stdout or to syslog

enum priority_t { crit, err, warning, notice, info, debug };

    /// pure virtual class defining the common interface of log centralization implementations

class central_report
{
public:
    central_report(priority_t min_logged): min(min_logged) {};
    central_report(const central_report & ref) = delete;
    central_report(central_report && ref) noexcept = delete;
    central_report & operator = (const central_report & ref) = delete;
    central_report & operator = (central_report && ref) noexcept = delete;
    virtual ~central_report() noexcept(false) {};

    void report(priority_t priority, const std::string & message);

protected:
    virtual void inherited_report(priority_t priority, const std::string & message) = 0;

private:
    priority_t min;
    static libthreadar::mutex access; //< caller must acquire mutex before reporting
};

    /// implements a central_report logging centralization which output the logs on stdout

class central_report_stdout : public central_report
{
public:
    central_report_stdout(priority_t min_logged): central_report(min_logged) {};

protected:
    virtual void inherited_report(priority_t priority, const std::string & message) override;
};

    /// implements a central_report logging centralization which send logs to syslog

class central_report_syslog : public central_report
{
public:
    central_report_syslog(priority_t min_logged, const std::string & fixed_label, int facility);
    ~central_report_syslog() noexcept(false);

protected:
    virtual void inherited_report(priority_t priority, const std::string & message) override;

private:
    std::string label; //< need to exist during the whole life of the object for syslog(3) to work as expected

	// static fields
    static unsigned int num_obj; //< number of object created (only one is allowed as syslog routine is not re-entrant)
    static libthreadar::mutex num_obj_mod;    //< controls the write access to num_obj

};

#endif
