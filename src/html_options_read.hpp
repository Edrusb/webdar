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

#ifndef HTML_OPTIONS_READ_HPP
#define HTML_OPTIONS_READ_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <memory>
#include <string>

    // webdar headers
#include "body_builder.hpp"
#include "html_div.hpp"
#include "html_form.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_select.hpp"
#include "html_form_input.hpp"
#include "html_form_input_file.hpp"
#include "html_crypto_algo.hpp"
#include "events.hpp"
#include "html_derouleur.hpp"
#include "html_text.hpp"
#include "html_entrepot.hpp"
#include "html_libdar_running_popup.hpp"
#include "bibliotheque.hpp"
#include "guichet.hpp"
#include "jsoner.hpp"
#include "bibliotheque_subconfig.hpp"

    /// class html_options_read implementes the html components to setup optional parameters while reading an archive

    /// most of the components are simple ones, except two couples that depends on each others:
    /// - the archive selection path that depends on the entrepot user-defined parameters
    /// - the ref_archive selection path that depends on the ref_entrepot user-defined parameters
    /// in both cases, the creation of the new entrepot requires html_web_user_interaction component and separated thread
    /// to continue displaying the status and let the user interrupt the thread:
    /// - For the first entrepot, this is left to the parent component to call get_entrepot() when we generate the
    ///   entrepot_has_changed event, providing the needed html_web_user_interaction component in argument, because the
    ///   archive selection path that depends of this entrepot is not an read option but a parameter and stored outside
    ///   html_options_read.
    /// - For external catalogue things are different as ref_archive is part of html_options_read (field ref_path) and
    ///   need to be updated as soon as the ref_entrepot changes and this is internal to this component. For that we
    ///   use our own html_web_user_interaction from the html_running_libdar_popup (ref_webui) that shows during
    ///   entrepot creation.
    /// The ref_entrepot related event is probably useless as there is not external components to read options that
    /// depends on it. But the entrepot_has_changed event is needed by the html_archive_read
    /// of which we are a component, in order to browse the new entrepot when selecting the archive basename to read.
    /// get_options() get_entrepot() and get_ref_entrepot() may also need an html_user_interaction the one used
    /// is the one provided as argument of those methods, not the one given to set_webui() which is only used
    /// when representing the html components during the configuration phase of webdar.

class html_options_read : public body_builder,
			  public actor,
			  public events,
			  public libthreadar::thread_signal,
			  public jsoner,
			  public bibliotheque_subconfig
{
public:
    static const std::string entrepot_has_changed; ///< entrepot parameter changed
    static const std::string changed;  ///< any parameter changed (including entrepot change)

    html_options_read();
    html_options_read(const html_options_read & ref) = delete;
    html_options_read(html_options_read && ref) noexcept = delete;
    html_options_read & operator = (const html_options_read & ref) = delete;
    html_options_read & operator = (html_options_read && ref) noexcept = delete;
    ~html_options_read() { cancel(); join(); };

	/// mandatory call to invoke ASAP after constructor
    void set_biblio(const std::shared_ptr<bibliotheque> & ptr);

	/// obtain the libdar archive_option_read object from the html filled fields
    libdar::archive_options_read get_options(std::shared_ptr<html_web_user_interaction> & webui) const;

	/// obtain just the entrepot object from the option fields
    std::shared_ptr<libdar::entrepot> get_entrepot(std::shared_ptr<html_web_user_interaction> webui) const
    { return entrep->get_entrepot(webui); };

	/// set min-digits field for the archive to read (not the archive of reference if any)
    void set_src_min_digits(const std::string & val);

	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override;

	/// inherited from bibliotheque_subconfig
    virtual bibliotheque::using_set get_using_set() const override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:

	// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	// inheroted from body_builder
    virtual void new_css_library_available() override;

	// inherited from class libthreadar::thread
    virtual void inherited_run() override;

	/// inherited from libthreadar::thread
    virtual void signaled_inherited_cancel() override;


private:
    static const std::string ref_entrepot_has_changed;
    static constexpr const char* sect_entrep = "entrep";
    static constexpr const char* sect_opt = "options";
    static constexpr const char* sect_ref_entrep = "ref_entrep";
    static constexpr const char* sect_extcat = "externalcat";


    html_derouleur deroule;
    html_form form_src;
    html_form_fieldset fs_src;
    html_form_fieldset fs_ref;

	// archive entrepot fields
    guichet guichet_entrep;
    std::shared_ptr<html_entrepot> entrep;

	// archive_options_read fields
    html_crypto_algo src_crypto_algo;
    html_form_input src_crypto_pass;
    html_form_input src_crypto_size;
    html_form_input src_ignore_sig_failure;
    html_form_input src_execute;
    html_form_input src_slice_min_digits;
    html_form_input info_details;
    html_form_input lax;
    html_form_input sequential_read;
    html_form_input force_first_slice;
    html_form_input multi_thread_crypto;
    html_form_input multi_thread_compress;

	// external catalogue entrepot
    guichet guichet_ref_entrep;
    std::shared_ptr<html_entrepot> ref_entrep;

    html_form_input ref_use_external_catalogue;
    html_form form_ref;
    html_form_input_file ref_path;
    html_crypto_algo ref_crypto_algo;
    html_form_input ref_crypto_pass;
    html_form_input ref_crypto_size;
    html_form_input ref_execute;
    html_form_input ref_slice_min_digits;

	// used to create ref_entrep during html interaction
    html_libdar_running_popup ref_webui;

	/// to gather many subcomponent changes into a single change event from us
    bool ignore_events;

	/// delay entrepot update when waiting inherited_get_body_part() to be executed

	/// \note not doing so may lead to run() this object which involves an html_web_user_interaction
	/// while the object is not visible and this this interaction component not operational
    bool need_ref_entrepot_update;

    void update_ref_entrepot();
    void set_defaults();
    void trigger_changed();

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_options_read";

    static constexpr const char* jlabel_entrep = "entrepot";
    static constexpr const char* jlabel_crypto_algo = "cryto-algo";
    static constexpr const char* jlabel_crypto_pass = "crypto-pass";
    static constexpr const char* jlabel_crypto_size = "crytpo-size";
    static constexpr const char* jlabel_ignore_sig_failure = "ignore-sig-failure";
    static constexpr const char* jlabel_execute = "execute";
    static constexpr const char* jlabel_slice_min_digits = "min-digits";
    static constexpr const char* jlabel_info_details = "info-details";
    static constexpr const char* jlabel_lax = "lax-mode";
    static constexpr const char* jlabel_seq_read = "sequential-read";
    static constexpr const char* jlabel_force_first_slice = "first-slice-reading";
    static constexpr const char* jlabel_thread_crypto = "cipher-threads";
    static constexpr const char* jlabel_thread_compress = "compr-threads";
    static constexpr const char* jlabel_ref_entrep = "ref-entrepot";
    static constexpr const char* jlabel_ref_used = "ref-used";
    static constexpr const char* jlabel_ref_path = "ref-path";
    static constexpr const char* jlabel_ref_crypto_algo = "ref-crypto-algo";
    static constexpr const char* jlabel_ref_crypto_pass = "ref-crypt-pass";
    static constexpr const char* jlabel_ref_crypto_size = "ref-crypto-size";
    static constexpr const char* jlabel_ref_execute = "ref-execute";
    static constexpr const char* jlabel_ref_slice_min_digits = "ref-min-digits";

};

#endif
