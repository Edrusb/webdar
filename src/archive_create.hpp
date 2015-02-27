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

#ifndef ARCHIVE_CREATE_HPP
#define ARCHIVE_CREATE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "web_user_interaction.hpp"
#include "html_options_create.hpp"

class archive_create : public libthreadar::thread
{
public:
    archive_create(): archpath("/"), fs_root("/") { progressive_report = NULL; };

    void set_user_interaction(web_user_interaction & ref) { ui = ref; };
    void set_archive_path(const std::string & val);
    void set_archive_basename(const std::string & val) { basename = val; };
    void set_archive_extension(const std::string & val) { extension = val; };
    void set_archive_options_create(const libdar::archive_options_create & val) { opt = val; };
    void clear_archive_options_reference() { has_ref = false; };
    void set_archive_options_reference(const std::string & refpath,
				       const std::string & basename,
				       const std::string & extension,
				       const libdar::archive_options_read & readopt);
    void set_fs_root(const std::string & val);
    void set_progressive_report(libdar::statistics *ptr) { progressive_report = ptr; };

protected:
	/// inherited from class libthreadar::thread
    virtual void inherited_run();

private:
    web_user_interaction ui;
    libdar::path archpath;
    std::string basename;
    std::string extension;
    libdar::path fs_root;
    libdar::archive_options_create opt;
    bool has_ref;
    std::string ref_path;
    std::string ref_basename;
    std::string ref_extension;
    libdar::archive_options_read ref_opt;
    libdar::statistics *progressive_report;
};

#endif
