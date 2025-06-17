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

#ifndef ARCHIVE_INIT_LIST_HPP
#define ARCHIVE_INIT_LIST_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <string>
#include <dar/libdar.hpp>
#include <libthreadar/libthreadar.hpp>
#include <memory>

    // webdar headers
#include "html_web_user_interaction.hpp"
#include "saisie.hpp"

    /// this objects is a thread object that provide access to the content of an existing archive

    /// the object must first be set with the location of the dar archive to open
    /// using set_archive_path(), set_archive_basename() and eventually set_archive_options_read()
    /// then the thread has to be run calling the libthreadar::run() inherited method which opens
    /// the archive. Once the archive has been openned successfully the thread ends and the method
    /// openned() returns true. Starting this point, the archive content is available
    /// calling get_children_in_table() or has_subdirectory(). Once completed close_archive() can be called
    /// or the object deleted.

class archive_init_list : public libthreadar::thread_signal
{
public:
    archive_init_list();
    archive_init_list(const archive_init_list & ref) = delete;
    archive_init_list(archive_init_list && ref) noexcept = delete;
    archive_init_list & operator = (const archive_init_list & ref) = delete;
    archive_init_list & operator = (archive_init_list && ref) noexcept = delete;
    ~archive_init_list() { cancel(); join(); };


	/// set the user interaction to report on when running the thread
    void set_user_interaction(std::shared_ptr<html_web_user_interaction> ref) { ui = ref; };
    void set_parametrage(const saisie* x_param) { param = x_param; };

	/// once the thread has been ran and has finished, we end up with an opened archive
	///
	/// \return true if the archive is available false if a failure occured and no archive available
    bool opened() const { return ptr != nullptr; };

	/// obtains contents from the opened archive in the calling thread
    const std::vector<libdar::list_entry> get_children_in_table(const std::string & dir) const;
    bool has_subdirectory(const std::string & dir) const;

	/// obtain the archive summary
    libdar::archive_summary get_summary() const;
    std::string get_archive_full_name() const;

	/// close the opened archive run in the calling thread
    void close_archive() { ptr.reset(); };

protected:

	/// inherited from class libthreadar::thread
    virtual void inherited_run() override;

	/// inherited from libthreadar::thread
    virtual void signaled_inherited_cancel() override;


private:
    const saisie* param;
    std::unique_ptr<libdar::archive> ptr;
    std::shared_ptr<html_web_user_interaction> ui;

};

#endif
