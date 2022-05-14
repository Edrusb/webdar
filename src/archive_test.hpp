/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2022 Denis Corbin
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

#ifndef ARCHIVE_TEST_HPP
#define ARCHIVE_TEST_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <string>

    // webdar headers
#include "web_user_interaction.hpp"

class archive_test : public libthreadar::thread
{
public:
    archive_test(): archpath("/") { progressive_report = NULL; };

    void set_user_interaction(web_user_interaction & ref) { ui = std::make_shared<web_user_interaction>(ref); };
    void set_archive_path(const std::string & val);
    void set_archive_basename(const std::string & val) { basename = val; };
    void set_archive_options_read(const libdar::archive_options_read & val) { read_opt = val; };
    void set_archive_options_test(const libdar::archive_options_test & val) { test_opt = val; };
    void set_progressive_report(libdar::statistics *ptr) { progressive_report = ptr; };

protected:
	/// inherited from class thread
    virtual void inherited_run();

private:
    std::shared_ptr<web_user_interaction> ui;
    libdar::path archpath;
    std::string basename;
    libdar::archive_options_read read_opt;
    libdar::archive_options_test test_opt;
    libdar::statistics *progressive_report;
};

#endif
