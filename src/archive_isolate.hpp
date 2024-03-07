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
#include "html_web_user_interaction.hpp"
#include "html_options_isolate.hpp"
#include "saisie.hpp"

class archive_isolate : public libthreadar::thread
{
public:
    archive_isolate();
    archive_isolate(const archive_isolate & ref) = default;
    archive_isolate(archive_isolate && ref) noexcept = default;
    archive_isolate & operator = (const archive_isolate & ref) = default;
    archive_isolate & operator = (archive_isolate && ref) noexcept = default;
    ~archive_isolate() = default;

	// needed to read the archive to isolate

    void set_user_interaction(std::shared_ptr<html_web_user_interaction> ref) { ui = ref; };
    void set_parametrage(const saisie* x_param) { param = x_param; };

protected:
	/// inherited from class libthreadar::thread
    virtual void inherited_run() override;

private:
    std::shared_ptr<html_web_user_interaction> ui;
    const saisie* param;

	// the following field are setup from param and ui in inherited_run() subthrerad
    libdar::path archpath;
    std::string basename;
    libdar::archive_options_read read_opt;
    libdar::path dest_path;
    std::string dest_basename;
    libdar::archive_options_isolate isol_opt;
};

#endif
