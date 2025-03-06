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

#ifndef HTML_ARCHIVE_REPAIR_HPP
#define HTML_ARCHIVE_REPAIR_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <libthreadar/libthreadar.hpp>


    // webdar headers
#include "body_builder.hpp"
#include "actor.hpp"
#include "html_form_input_file.hpp"
#include "html_form_fieldset.hpp"
#include "html_derouleur.hpp"
#include "html_options_repair.hpp"
#include "html_libdar_running_popup.hpp"
#include "guichet.hpp"

    /// html component used to collect repairing operation parameters from the user

class html_archive_repair: public body_builder, public libthreadar::thread_signal, public actor
{
public:
    html_archive_repair();
    html_archive_repair(const html_archive_repair & ref) = delete;
    html_archive_repair(html_archive_repair && ref) noexcept = delete;
    html_archive_repair & operator = (const html_archive_repair & ref) = delete;
    html_archive_repair & operator = (html_archive_repair && ref) noexcept = delete;
    ~html_archive_repair() { cancel(); join(); };

    const std::string & get_archive_path() const { return repair_dest.get_value(); };
    const std::string & get_archive_basename() const;
    libdar::archive_options_repair get_options(std::shared_ptr<html_web_user_interaction> dialog) const { return opt_repair->get_options(dialog); };

	/// mandatory call to invoke ASAP after constructor
    void set_biblio(const std::shared_ptr<bibliotheque> & ptr);

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

	/// inherited from libthreadar::thread
    virtual void inherited_run() override;

	/// inherited from libthreadar::thread
    virtual void signaled_inherited_cancel() override;


private:
	/// delay entrepot update when waiting inherited_get_body_part() to be executed

	/// \note not doing so may lead to run() this object which involves an html_web_user_interaction
	/// while the object is not visible and this this interaction component not operational
    bool need_entrepot_update;

    html_derouleur deroule;
    html_form_input_file repair_dest;
    html_form_input basename;
    html_form_fieldset repair_fs;
    html_form repair_form;
    html_libdar_running_popup repoxfer;

    guichet guichet_opt_repair;
    std::shared_ptr<html_options_repair> opt_repair;

    void update_entrepot();
};

#endif
