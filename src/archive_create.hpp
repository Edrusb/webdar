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
#include "saisie.hpp"

    /// class archive_create run libdar creation in a dedicated thread

    /// Accordingly to the libhtreadar::thread paradigm, this class
    /// provides methods to set parameter to the comparison operation
    /// these are not set at object construction time, nor the thread
    /// is run at that time, but once all parameter have been set by
    /// invoking the libthreadar::run() method
    /// \note this class is not a graphical class at all but relies
    /// on aprovided web_user_interaction object to report status and
    /// intermediate information about the operation under process


class archive_create : public libthreadar::thread
{
public:
    archive_create(): param(nullptr) {};
    archive_create(const archive_create & ref) = default;
    archive_create(archive_create && ref) noexcept = default;
    archive_create & operator = (const archive_create & ref) = default;
    archive_create & operator = (archive_create && ref) noexcept = default;
    ~archive_create() = default;

	// parameters expected by the libdar::archive constructor

    void set_user_interaction(std::shared_ptr<html_web_user_interaction> ref) { ui = ref; };
    void set_parametrage(const saisie* x_param) { param = x_param; };

protected:

	/// inherited from class libthreadar::thread
    virtual void inherited_run() override;

private:
    std::shared_ptr<html_web_user_interaction> ui;
    const saisie* param;

};

#endif
