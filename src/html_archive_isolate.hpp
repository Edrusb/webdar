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
#ifndef HTML_ARCHIVE_ISOLATE_HPP
#define HTML_ARCHIVE_ISOLATE_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "html_form_input_file.hpp"
#include "html_options_isolate.hpp"
#include "body_builder.hpp"
#include "html_derouleur.hpp"
#include "actor.hpp"
#include "html_libdar_running_popup.hpp"
#include "guichet.hpp"

    /// html component used to let the user provide parameters for an isolation operation

class html_archive_isolate: public body_builder, public libthreadar::thread_signal, public actor
{
public:
    html_archive_isolate();
    html_archive_isolate(const html_archive_isolate & ref) = delete;
    html_archive_isolate(html_archive_isolate && ref) noexcept = delete;
    html_archive_isolate & operator = (const html_archive_isolate & ref) = delete;
    html_archive_isolate & operator = (html_archive_isolate && ref) noexcept = delete;
    ~html_archive_isolate() { cancel(); join(); };

	/// mandatory call to invoke ASAP after constructor
    void set_biblio(const std::shared_ptr<bibliotheque> & ptr);


    const std::string & get_archive_path() const { return sauv_path.get_value(); };
    const std::string & get_archive_basename() const;
    libdar::archive_options_isolate get_options_isolate(std::shared_ptr<html_web_user_interaction> & webui) const { return options->get_options(webui); };

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
    html_derouleur deroule;
    html_form form;
    html_form_fieldset fs;
    html_form_input_file sauv_path;
    html_form_input basename;
    html_libdar_running_popup repoxfer;

    guichet guichet_options;
    std::shared_ptr<html_options_isolate> options;


	/// delay entrepot update when waiting inherited_get_body_part() to be executed

	/// \note not doing so may lead to run() this object which involves an html_web_user_interaction
	/// while the object is not visible and this this interaction component not operational
    bool need_entrepot_update;

    void update_entrepot();
};

#endif
