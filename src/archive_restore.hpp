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

#ifndef ARCHIVE_RESTORE_HPP
#define ARCHIVE_RESTORE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "html_web_user_interaction.hpp"
#include "saisie.hpp"

class archive_restore : public libthreadar::thread_signal
{
public:
    archive_restore(): param(nullptr) {};
    archive_restore(const archive_restore & ref) = default;
    archive_restore(archive_restore && ref) noexcept = default;
    archive_restore & operator = (const archive_restore & ref) = default;
    archive_restore & operator = (archive_restore && ref) noexcept = default;
    ~archive_restore() { cancel(); join(); };

    void set_user_interaction(std::shared_ptr<html_web_user_interaction> ref) { ui = ref; };
    void set_parametrage(const saisie* x_param) { param = x_param; };

protected:

	/// inherited from class libthreadar::thread
    virtual void inherited_run() override;

	/// inherited from libthreadar::thread
    virtual void signaled_inherited_cancel() override;

private:
    std::shared_ptr<html_web_user_interaction> ui;
    const saisie* param;

};




#endif
