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

#ifndef ARCHIVE_TEST_HPP
#define ARCHIVE_TEST_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <string>

    // webdar headers
#include "html_web_user_interaction.hpp"
#include "saisie.hpp"

    /// class archive_test wraps libdar archive testing operation in a dedicated thread

class archive_test : public libthreadar::thread_signal
{
public:
    archive_test();
    archive_test(const archive_test & ref) = delete;
    archive_test(archive_test && ref) noexcept = default;
    archive_test & operator = (const archive_test & ref) = delete;
    archive_test & operator = (archive_test && ref) noexcept = default;
    ~archive_test() { cancel(); join(); };

    void set_user_interaction(const std::shared_ptr<html_web_user_interaction> ref) { ui = ref; };
    void set_parametrage(const saisie* x_param) { param = x_param; };

protected:
	/// inherited from class thread
    virtual void inherited_run() override;

	/// inherited from libthreadar::thread
    virtual void signaled_inherited_cancel() override;

private:
    const saisie* param;
    std::shared_ptr<html_web_user_interaction> ui;

};

#endif
