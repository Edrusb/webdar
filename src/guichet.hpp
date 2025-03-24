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

#ifndef GUICHET_HPP
#define GUICHET_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"
#include "jsoner.hpp"
#include "bibliotheque.hpp"
#include "bibliotheque_subconfig.hpp"
#include "actor.hpp"
#include "html_form.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_select.hpp"
#include "html_double_button.hpp"
#include "html_form_input.hpp"
#include "html_text.hpp"

    /// class guichet let one fetch config from a bibliotheque, save as or setup manually for a given component type

    /// the given component is adopted by the guichet and is shown only in manual mode, else it is hidden, but
    /// it always updated by the config from the bibliotheque and used to fetch libdar formated options set.
    /// The guichet object adds a drop-down list of available configurations for that objects (found in the
    /// provided bibliotheque), beside several buttons:
    /// - edit : which lead the component to show and the dropbox to be set to manual mode
    /// - save-as : in manual mode to save the current configuration as a new one in the bibliotheque
    ///   (the save-as button is associated to a box to provide a name)
    /** \verbatim

	+--------<form>-----------------------------------+
	| +----------------------------------+            |
        | | <drop down> list (incl. "manual")|            |
	| +----------------------------------+            |
	|                                                 |
	+---<update>--------------------------------------+

	(to_adopt object show only in "manual" mode)
	+-----------<optional form around to_adopt>-------+
	| < to_adopt>                                     |
	|                                                 |
	+---<update>--------------------------------------+

	(edit shows if non-"manual" choice is selected here above)
	(clear shows only in "manual" mode)
	+---------------+ +---------------+
	| edit config   | | clear config  |
	+---------------+ +---------------+

	(the below form only shows if to_adopt is visible ("manual" mode))
	+---<save-as-form>--------------------------------+
	|                     +-------------------------+ |
	| configuration name  |                         | |
	|                     +-------------------------+ |
	|                                                 |
	+---<save as>-------------------------------------+

	\endverbatim **/

class guichet: public body_builder, public jsoner, public bibliotheque_subconfig, public actor
{
public:

	/// constructor

	/// \param[in] subcomponent has to be set to true when it is used as indirect (or direct) subcomponent
	/// of another mask object (for example another guichet object) and used to refer existing configurations
	/// from a bibliotheque. This avoids the form to be used, the edit button to show, and other cosmetic things.
	/// By oposition, it should be left to false, when used directly in non-configuration components.
    guichet(bool subcomponent = false);

    guichet(const guichet & ref) = default;
    guichet(guichet && ref) noexcept = default;
    guichet & operator = (const guichet & ref) = default;
    guichet & operator = (guichet && ref) noexcept = default;
    virtual ~guichet();


	/// mandatory call to provide the object to adopt and use to display configurations

	/// \param[in] ptr the bibliotheque object where to fetch from configurations
	/// \param[in] cat the category for configurations to use in the bibliotheque
	/// \param[in] to_adopt a body_builder and a jsoner inherited object to adopt and apply configurations to.
	/// Important: the pointed to object by to_adopt is not expected to be changed during the
	/// lifetime of this guichet object!
	/// \param[in] add_form_around adopt to_adopt in a form in turn adopted by "this" (needed for
	/// to update components manually when they do not include a form themselves).
	/// \note the to_adopt pointed to object is interrogated directly by owner of guichet to fetch the
	/// specific object properties, guichet only uses its jsoner interface, it is also expected to
	/// be a body_builder inherited object for it can be adopted by "this", and of course it must
	/// not have already been adopted. This object will not be visible when using an existing
	/// configuration in the bibliotheque, or visible when setup manually.
	/// \note the object that owns the guichet should not fetch json stuff from the to_adopt object
	/// but should pass through this guichet object to either get the to_adopted json config (wrapped
	/// with some metadata provided by guichet) or a guichet structure pointing to the configuration
	/// name to use in the bibliotheque.
    void set_child(const std::shared_ptr<bibliotheque> & ptr,
		   bibliotheque::category cat,
		   const std::shared_ptr<body_builder> & to_adopt,
		   bool add_form_around);


	/// inherited from jsoner (used when this object is used as subconfig of another object)
    virtual void load_json(const json & source) override;

	/// inherited from jsoner (used when this object is used as subconfig of another object)
    virtual json save_json() const override;

	/// inherited from jsoner (used when this object is used as subconfig of another object)
    virtual void clear_json() override;

	/// inherited from bibliotheque_subconfig
    virtual bibliotheque::using_set get_using_set() const override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// load a config of given name (in the category defined by set_child)*

	/// \note exception thrown if the requested configuration does not exist
    void load_from_bibliotheque(const std::string & name) { select.set_selected_id(name); };

	/// inherited from body_builder
    virtual void bind_to_anchor(const std::string & val);


protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    std::string anchor_to;
    bibliotheque::category categ;
    std::shared_ptr<bibliotheque> biblio;
    std::shared_ptr<body_builder> adopted;
    jsoner* adopted_jsoner;
    bibliotheque_subconfig* adopted_subconfig; ///< may be null if adopted does not implement bibliotheque_subconfig interface
    bool ignore_events;
    body_builder* adopted_frame;   ///< which wrapper object to change visibility of
    bool clear_adopted;     ///< used to differentiate manual-mode selection (false) from clear button selection (true)
    bool is_sub;     ///< true when guichet is used inside a config, false when it is the top level of a config recordable into a bibliotheque
    std::string currently_locked; ///< name of the config we locked in the bibliotheque for the categ category (empty string = no lock)

    html_div box;
    html_form select_form;
    html_form_fieldset select_fs;
    html_form_select select;
    html_form around_adopted;
    html_form_fieldset around_adopted_fs;
    html_double_button edit;
    html_double_button clear;
    html_form saveas_form;
    html_text saveas_text;
    html_form_fieldset saveas_fs;
    html_form_input saveas_name;

    void update_selected();
    void set_adopted();
    void set_visibility();

    void check_adopted() const { if(!adopted) throw WEBDAR_BUG; };

    static constexpr const char* event_select = "select";
    static constexpr const char* event_edit = "edit";
    static constexpr const char* event_clear = "clear";
    static constexpr const char* event_saveas = "saveas";

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "guichet";

    static constexpr const char* class_id = "guichet";
    static constexpr const char* jlabel_mode = "manual-mode"; // boolean (either categ+name or config)
    static constexpr const char* jlabel_categ = "category";  // string: config category
    static constexpr const char* jlabel_name = "configname"; // string: config name
    static constexpr const char* jlabel_conf = "config";   ; // json; object configuration

    static constexpr const char* css_left_margin = "guichet_left_margin";
    static constexpr const char* css_below_margin = "guichet_below_margin";
    static constexpr const char* css_above_margin = "guichet_above_margin";
    static constexpr const char* css_border_left = "guichet_border_left";

    static constexpr const char* fs_title_subcomp = "Matches configuration named";

};


#endif
