/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2024 Denis Corbin
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

#ifndef ARCHIVE_MERGE_HPP
#define ARCHIVE_MERGE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "web_user_interaction.hpp"
#include "html_options_merge.hpp"
#include "saisie.hpp"


    /// class archive_merge runs libdar merging operation in a dedicated thread

class archive_merge : public libthreadar::thread_signal
{
public:
    archive_merge(): param(nullptr) {};
    archive_merge(const archive_merge & ref) = default;
    archive_merge(archive_merge && ref) noexcept = default;
    archive_merge & operator = (const archive_merge & ref) = default;
    archive_merge & operator = (archive_merge && ref) noexcept = default;
    ~archive_merge() { cancel(); join(); };

    void set_user_interaction(const std::shared_ptr<html_web_user_interaction> ref) { ui = ref; };
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
