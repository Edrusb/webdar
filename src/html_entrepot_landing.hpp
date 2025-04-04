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

#ifndef HTML_ENTREPOT_LANDING_HPP
#define HTML_ENTREPOT_LANDING_HPP

#include "my_config.h"

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "actor.hpp"
#include "events.hpp"
#include "jsoner.hpp"
#include "html_entrepot.hpp"
#include "html_form_input_file.hpp"
#include "html_form.hpp"
#include "html_libdar_running_popup.hpp"

    /// class html_entrepot_landing add a landing path to an entrepot object

class html_entrepot_landing: public body_builder,
			     public actor,
			     public events,
			     public libthreadar::thread_signal,
			     public jsoner
{
public:

	/// event triggered for any change (entrepot and / or landing_path)
    static const std::string changed;

	/// the use case is for the caller to know when to call get_entrepot()
    static const std::string entrepot_only_changed;

	/// event triggered only when the landing path changed (and nothing else changed)
	/// \note this event is triggered only if landing_path is to be enforced to backup path
    static const std::string landing_path_only_changed;

    html_entrepot_landing();
    html_entrepot_landing(const html_entrepot_landing & ref) = delete;
    html_entrepot_landing(html_entrepot_landing && ref) noexcept = delete;
    html_entrepot_landing & operator = (const html_entrepot_landing & ref) = delete;
    html_entrepot_landing & operator = (html_entrepot_landing && ref) noexcept = delete;
    ~html_entrepot_landing() { cancel(); join(); };

	/// provide an libdar entrepot object build from Web fields values
    std::shared_ptr<libdar::entrepot> & get_entrepot(std::shared_ptr<html_web_user_interaction> & webui) const;

	/// fetch current landing path for the entrepot
    const std::string & get_landing_path() const { return landing_path.get_value(); };

    	/// inherited from actor parent class
    virtual void on_event(const std::string & event_name) override;

	/// change the change event name
    void set_event_entrepot(const std::string & name);

	/// change landing_path_only_changed event name
    void set_event_landing_path(const std::string & name);

	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override;

	/// set the object to webdar defaults
    void set_to_webdar_defaults();


	/// inherited from body_builder
    virtual void bind_to_anchor(const std::string & val) override { form.bind_to_anchor(val); };

protected:

    	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

    	/// inherited from body_builder
    virtual void css_classes_have_changed() override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

    	/// inherited from libthreadar::thread
    virtual void inherited_run() override;

	/// inherited from libthreadar::thread
    virtual void signaled_inherited_cancel() override;

private:
    std::string custom_event_entrepot;
    std::string custom_event_landing_path;
    bool ignore_events;
    mutable libthreadar::mutex entrep_ctrl; ///< control write access to entrepot_changed and libdar_entrep
    mutable bool entrepot_changed; ///< whether libdar_entrep should be re-fetched
    mutable std::shared_ptr<libdar::entrepot> libdar_entrep;

	// warning, custom_event need to be declared before the html_libdar_running_popup
	// because at destruction time of this object, the html_libdar_running_popup destructor
	// may lead to an event 'this' is acting on and requires the custom_event_* string
	// object to be defined. In this order the destructor of 'this' will not
	// destroy the custom_event_* std::string objects before repoxfer below.

    html_form form;
    html_form_fieldset fs;
    html_entrepot entrep;
    html_form_input use_landing_path;
    html_form_input_file landing_path;
    html_libdar_running_popup repoxfer;

    void start_updating_landing();

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_entrepot_landing";

    static constexpr const char* jlabel_entrepot = "entrepot";
    static constexpr const char* jlabel_use_landing_path = "use-landing-path";
    static constexpr const char* jlabel_landing_path = "landing-path";

};

#endif
