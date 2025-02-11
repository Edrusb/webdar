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

#ifndef HTML_ENTREPOT_HPP
#define HTML_ENTREPOT_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <memory>
#include <string>

    // webdar headers
#include "body_builder.hpp"
#include "html_form.hpp"
#include "html_form_input.hpp"
#include "html_form_input_file.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_select.hpp"
#include "actor.hpp"
#include "events.hpp"
#include "html_web_user_interaction.hpp"
#include "jsoner.hpp"

    /// class html_entrepot let user define an entrepot parameters

class html_entrepot: public body_builder, public actor, public events, public libthreadar::thread_signal, public jsoner
{
public:
	/// event triggered when any entrepot parameters has changed *except landing_path* changes

	// the use case is for the caller to know when to call get_entrepot()
    static const std::string changed;

	/// event triggered only when the landing path changed (and nothing else changed)
    static const std::string landing_path_changed;

    html_entrepot();
    html_entrepot(const html_entrepot & ref) = delete;
    html_entrepot(html_entrepot && ref) noexcept = delete;
    html_entrepot & operator = (const html_entrepot & ref) = delete;
    html_entrepot & operator = (html_entrepot && ref) noexcept = delete;
    ~html_entrepot() { cancel(); join(); };

	/// run the entrepot building in a dedicated thread and display/control this thread

	/// from the provided html_web_user_interaction object, which must run from another
	/// thread than the one calling this method for the user to be able to interrupt the operation
    std::shared_ptr<libdar::entrepot> get_entrepot(std::shared_ptr<html_web_user_interaction> & webui) const;

	/// fetch current landing path for the entrepot
    const std::string & get_landing_path() const { return landing_path.get_value(); };

	/// inherited from actor parent class
    virtual void on_event(const std::string & event_name) override;

	/// change the change event name
    void set_event_name(const std::string & name);

	/// change landing_path_changed event name
    void set_event_landing_path(const std::string & name);

	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override;

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void css_classes_have_changed() override;

	/// inherited from libthreadar::thread
    virtual void inherited_run() override;

	/// inherited from libthreadar::thread
    virtual void signaled_inherited_cancel() override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;


private:
    static constexpr const char* type_local = "local";
    static constexpr const char* type_ftp   = "ftp";
    static constexpr const char* type_sftp  = "sftp";
    static constexpr const char* auth_pass = "password";
    static constexpr const char* auth_key  = "keyfile";
    static constexpr const char* css_warning = "html_entrepot_warn";

    static const std::string repo_type_changed;

    html_form form;
    html_form_fieldset fs;
    html_form_select repo_type; // local, ftp or sftp
    html_form_input host;      // ftp and sftp
    html_form_input port;      // ftp and sftp
    html_form_input login;     // ftp and sftp
    html_form_select auth_type; // (sftp only) login or keyfile
    html_form_input pass;      // ftp and sftp
    html_form_input auth_from_file;  // ftp and sftp
    html_form_input_file pub_keyfile; // sftp only
    html_form_input_file prv_keyfile; // sftp only
    html_form_input knownhosts_check; // sftp only
    html_form_input_file known_hosts_file; // sftp only
    html_form_input wait_time; // ftp and sftp
    html_form_input verbose;   // ftp and sftp
    html_form_input use_landing_path; // checkbox to show or not the landing_path (if not, no landing_path_changed event is triggered)
    html_form_input landing_path; // will chdir there at initial connection

    std::string custom_event_name;
    std::string custom_event_landing_path;
    bool ignore_events;
    mutable std::shared_ptr<libdar::user_interaction> dialog; ///< used by inherited_run
    mutable std::shared_ptr<libdar::entrepot> entrep;         ///< set by inherited_run
    mutable bool entrep_type_has_changed;                     ///< variable local to inherited_get_body_part() and methods called from there
    mutable bool entrep_need_update;

    void update_visible();
    void trigger_changed_event();
    void clear_form();
    void reset_ssh_files();

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_entrepot";

    static constexpr const char* jlabel_type = "type";
    static constexpr const char* jlabel_host = "host";
    static constexpr const char* jlabel_port = "port";
    static constexpr const char* jlabel_authtype = "authtype";
    static constexpr const char* jlabel_login = "login";
    static constexpr const char* jlabel_pass = "pass";
    static constexpr const char* jlabel_auth_from_file = "auth_file";
    static constexpr const char* jlabel_pubkey = "pubkey";
    static constexpr const char* jlabel_prvkey = "prvkey";
    static constexpr const char* jlabel_knownhosts = "knownhosts";
    static constexpr const char* jlabel_knownhosts_file = "knownhosts_file";
    static constexpr const char* jlabel_waittime = "wait";
    static constexpr const char* jlabel_verbose = "verbose";
    static constexpr const char* jlabel_use_landing_path = "use-landing-path";
    static constexpr const char* jlabel_landing_path = "landing-path";

    static constexpr const char* default_waittime = "30";
    static constexpr const char* default_verbose = "";
};

#endif
