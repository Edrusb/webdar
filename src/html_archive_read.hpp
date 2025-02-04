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

#ifndef HTML_ARCHIVE_READ_HPP
#define HTML_ARCHIVE_READ_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <memory>
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "html_form.hpp"
#include "html_form_input.hpp"
#include "html_form_input_file.hpp"
#include "html_form_fieldset.hpp"
#include "html_options_read.hpp"
#include "html_libdar_running_popup.hpp"
#include "actor.hpp"
#include "guichet.hpp"

    /// class html_archive_read let user define the archive path, basename and option to read

    /// it is mainly used in class saisie

class html_archive_read: public body_builder, public actor, public libthreadar::thread_signal
{
public:
    html_archive_read(const std::string & archive_description);
    html_archive_read(const html_archive_read & ref) = delete;
    html_archive_read(html_archive_read && ref) noexcept = delete;
    html_archive_read & operator = (const html_archive_read & ref) = delete;
    html_archive_read & operator = (html_archive_read && ref) noexcept = delete;
    ~html_archive_read() { cancel(); join(); };

	/// mandatory call to invoke ASAP after constructor
    void set_biblio(const std::shared_ptr<bibliotheque> & ptr);

	// available fields for libdar

    std::string get_archive_path() const;
    std::string get_archive_basename() const;
    libdar::archive_options_read get_read_options(std::shared_ptr<html_web_user_interaction> dialog) const { return opt_read->get_options(dialog); };

    virtual void on_event(const std::string & event_name) override;


protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from class libthreadar::thread
    virtual void inherited_run() override;

	/// inherited from libthreadar::thread
    virtual void signaled_inherited_cancel() override;

private:
    html_form form;
    html_form_fieldset fs;
    html_form_input_file arch_path;

    guichet guichet_opt_read;
    std::shared_ptr<html_options_read> opt_read;

    html_libdar_running_popup libdarexec;

	/// delay entrepot update when waiting inherited_get_body_part() to be executed

	/// \note not doing so may lead to run() this object which involves an html_web_user_interaction
	/// while the object is not visible and this this interaction component not operational
    bool need_entrepot_update;

    void update_entrepot();

};

#endif
