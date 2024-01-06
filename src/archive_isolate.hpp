/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2023 Denis Corbin
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

#ifndef ARCHIVE_ISOLATE_HPP
#define ARCHIVE_ISOLATE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "web_user_interaction.hpp"
#include "html_options_isolate.hpp"

class archive_isolate : public libthreadar::thread
{
public:
    archive_isolate(): archpath("/") { has_ref = false; };
    archive_isolate(const archive_isolate & ref) = default;
    archive_isolate(archive_isolate && ref) noexcept = default;
    archive_isolate & operator = (const archive_isolate & ref) = default;
    archive_isolate & operator = (archive_isolate && ref) noexcept = default;
    ~archive_isolate() = default;

    void set_user_interaction(const std::shared_ptr<web_user_interaction> & ref) { ui = ref; };
    void set_archive_path(const std::string & val);
    void set_archive_basename(const std::string & val) { if(val.empty()) throw exception_range("empty string is not a valid basename"); basename = val; };
    void set_archive_extension(const std::string & val) {  if(val.empty()) throw exception_range("empty string is not a valid dar extension"); extension = val; };
    void set_archive_options_isolate(const libdar::archive_options_isolate & val) { opt = val; };
    void clear_archive_options_reference() { has_ref = false; };
    void set_archive_reference(const std::string & refpath,
			       const std::string & basename,
			       const std::string & extension,
			       const libdar::archive_options_read & readopt);

protected:
	/// inherited from class libthreadar::thread
    virtual void inherited_run() override;

private:
    std::shared_ptr<web_user_interaction> ui;
    libdar::path archpath;
    std::string basename;
    std::string extension;
    libdar::archive_options_isolate opt;
    bool has_ref;
    std::string ref_path;
    std::string ref_basename;
    std::string ref_extension;
    libdar::archive_options_read ref_opt;
};

#endif
