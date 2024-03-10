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

#ifndef HTML_ENTREPOT_HPP
#define HTML_ENTREPOT_HPP

    // C system header files
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

    /// class html_entrepot let user define an entrepot parameters

class html_entrepot: public body_builder, public actor, public events, public libthreadar::thread
{
public:
    static const std::string changed; ///< triggered when the entrepot parameters has changed
	// the use case is for the caller to know when to call get_entrepot()

    html_entrepot();
    html_entrepot(const html_entrepot & ref) = delete;
    html_entrepot(html_entrepot && ref) noexcept = delete;
    html_entrepot & operator = (const html_entrepot & ref) = delete;
    html_entrepot & operator = (html_entrepot && ref) noexcept = delete;
    ~html_entrepot() = default;

	/// run the entrepot building in a dedicated thread and display/control this thread

	/// from the provided html_web_user_interaction object, which must run from another
	/// thread than the one calling this method for the user to be able to interrupt the operation
    std::shared_ptr<libdar::entrepot> get_entrepot(std::shared_ptr<html_web_user_interaction> & webui) const;

	/// inherited from actor parent class
    virtual void on_event(const std::string & event_name) override;

	/// change the change event name
    void set_event_name(const std::string & name);

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from libthreadar::thread
    virtual void inherited_run() override;

private:
    static constexpr const char* type_local = "local";
    static constexpr const char* type_ftp   = "ftp";
    static constexpr const char* type_sftp  = "sftp";
    static constexpr const char* auth_pass = "password";
    static constexpr const char* auth_key  = "keyfile";

    bool got_inner_event; // set to true by on_event()

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

    std::string custom_event_name;

    mutable bool has_changed;                 ///< whether the repo has changed after a form update
    mutable bool change_event_triggered;      ///< we want to trigger the change event only once until get_entrepot is called
    mutable unsigned int mem_type;            ///< previous value of repo_type
    mutable std::string mem_login;            ///< previous value of login
    mutable std::string mem_pass;             ///< previous value of pass
    mutable std::string mem_host;             ///< previous value of host
    mutable std::string mem_port;             ///< previous value of port
    mutable bool mem_auth_from_file;          ///< previous value of auth_from_file
    mutable std::string mem_pub_keyfile;      ///< previous value of pub_keyfile
    mutable std::string mem_prv_keyfile;      ///< previous value of prv_keyfile
    mutable bool mem_knownhosts_check;        ///< previous value of known_hosts_check
    mutable std::string mem_known_hosts_file; ///< previous value of knwonw_hosts_file
    mutable libdar::U_I mem_wait_time;        ///< previous value of wait_time

    mutable std::shared_ptr<libdar::user_interaction> dialog; ///< used by inherited_run
    mutable std::shared_ptr<libdar::entrepot> entrep;         ///< set by inherited_run

    void update_visible();
    bool check_if_changed() const;
    void record_changed_values() const;
};

#endif
