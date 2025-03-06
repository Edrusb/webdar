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

#ifndef ARCHIVE_COMPARE_HPP
#define ARCHIVE_COMPARE_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "html_web_user_interaction.hpp"
#include "saisie.hpp"

    /// class archive_compare run libdar comparison in a dedicated thread

    /// Accordingly to the libhtreadar::thread paradigm, this class
    /// provides methods to set parameter to the comparison operation
    /// these are not set at object construction time, nor the thread
    /// is run at that time, but once all parameter have been set by
    /// invoking the libthreadar::run() method
    /// \note this class is not a graphical class at all but relies
    /// on aprovided web_user_interaction object to report status and
    /// intermediate information about the operation under process

class archive_compare : public libthreadar::thread_signal
{
public:
    archive_compare(): param(nullptr) {};
    archive_compare(const archive_compare & ref) = default;
    archive_compare(archive_compare && ref) noexcept = default;
    archive_compare & operator = (const archive_compare & ref) = default;
    archive_compare & operator = (archive_compare && ref) noexcept = default;
    ~archive_compare() { cancel(); join(); };

	// parameters expected by the libdar::archive constructor
	// and libdar::archive::op_diff method

    void set_user_interaction(std::shared_ptr<html_web_user_interaction> ref) { ui = ref; };
    void set_parametrage(const saisie* x_param) { param = x_param; };

protected:

	/// inherited from class libthreadar::thread

	/// this fires the libdar comparison operation with the so far
	/// provided parameters
    virtual void inherited_run() override;

	/// inherited from libthreadar::thread
    virtual void signaled_inherited_cancel() override;

private:
    const saisie* param;
    std::shared_ptr<html_web_user_interaction> ui;

};

#endif
