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

#ifndef arriere_boutique_HPP
#define arriere_boutique_HPP

    // C system header files
#include "my_config.h"

extern "C"
{

}

    // C++ system header files
#include <dar/tools.hpp>
#include <string>
#include <deque>

    // webdar headers
#include "body_builder.hpp"
#include "actor.hpp"
#include "bibliotheque.hpp"
#include "html_text.hpp"
#include "html_form.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_input.hpp"
#include "html_form_radio.hpp"
#include "html_double_button.hpp"
#include "jsoner.hpp"
#include "bibliotheque_subconfig.hpp"
#include "webdar_css_style.hpp"
#include "html_div.hpp"
#include "tokens.hpp"
#include "html_yes_no_box.hpp"

    /// class arriere_boutique provides mean to add/load a given component type to/from a bibliotheque object

    /// this template receive as type the class to manipulate, which must:
    /// - inherit from a jsoner
    /// - inherit from body_builder
    /// - inherit from events and provide a event name triggered upon component changes
    /// - in option, it must inherit from bibliotheque_subconfig if its configuration depends on other configuration parts
    ///
    /** \verbatim

	                                                                    +--<listing_form>--+
	       [optional <warning_message>]                                 | +--<listing>---+ |
	  +------------------<wrapped>----------------------------------+   | | selectable   | |
	  | <adopted component of type T >                              |   | | of available | |
	  +-------------------------------------------------------------+   | | components   | |
	+--------------------<config_form>---------------------------------+| | for that type| |
        |+--------------------<config_fs>---------------------------------+|| |              | |
        || configuration name <config_name>                               ||| |              | |
	|+----------------------------------------------------------------+|| |              | |
	+-<save/save as>---------------------------------------------------+| |              | |
	                                                                    | +--------------+ |
									    +-<load selected>--+

                                                                            +-----------------+
                                                                            | delete loaded cf|
                                                                            +-----------------+


	\endverbatim **/

template <class T> class arriere_boutique: public body_builder, public actor
{
public:
    arriere_boutique(const std::shared_ptr<bibliotheque> & ptr, ///< bibliotheque object to read/save json config to
		     bibliotheque::category cat,      ///< category to use in the ptr pointed to bibliotheque
		     std::unique_ptr<T> & obj,        ///< the child object to load and save from json data
		     const std::string ch_event_name, ///< event name on obj triggered upon object changes
		     bool add_form_around             ///< whether to add a html_form around obj
	);
    arriere_boutique(const arriere_boutique & ref) = delete;
    arriere_boutique(arriere_boutique && ref) noexcept = delete;
    arriere_boutique & operator = (const arriere_boutique & ref) = delete;
    arriere_boutique & operator = (arriere_boutique && ref) noexcept = delete;
    ~arriere_boutique() = default;

	/// inherited from class actor
    virtual void on_event(const std::string & event_name) override;


protected:

	/// inherited from class body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;


	/// inherited from class body_builder
    virtual void new_css_library_available() override;


private:
    static constexpr const char* event_delete = "delete_conf";
    static constexpr const char* event_clear = "clear_cur_conf";
    static constexpr const char* css_float = "arriere_boutique_float";
    static constexpr const char* css_warn = "arriere_boutique_warn";
    static constexpr const char* css_margin_above = "arriere_boutique_margin";

    std::string currently_loaded;
    std::shared_ptr<bibliotheque> biblio;
    bibliotheque::category categ;
    std::string change_event_name;
    bool ignore_events;

    std::unique_ptr<T> wrapped;
    jsoner* wrapped_jsoner;
    body_builder* wrapped_body_builder;
    events* wrapped_events;
    bibliotheque_subconfig* wrapped_subconfig;

    html_text warning_message;
    html_text need_saving;
    html_form_fieldset config_fs;
    html_form config_form;
    html_form_input config_name;
    html_form listing_form;
    html_form_fieldset listing_fs;
    html_form_radio listing;
    html_double_button delete_selected;
    html_double_button clear_cur_config;
    html_div floteur;
    html_yes_no_box confirm;

    html_form_fieldset around_fs;
    html_form around_form;

    void load_listing(); ///< fills the listing from existing config in biblio and triggers html_form_radio::changed event
    void set_warning(const std::string & wm);
    void clear_warning() { warning_message.set_visible(false); };
    void silently_update_config_name(const std::string & val);
};


template <class T> arriere_boutique<T>::arriere_boutique(const std::shared_ptr<bibliotheque> & ptr,
							 bibliotheque::category cat,
							 std::unique_ptr<T> & obj,
							 const std::string ch_event_name,
							 bool add_form_around):
    currently_loaded(""),
    categ(cat),
    config_form("Save/Save as"),
    config_fs(""),
    config_name("Configuration name", html_form_input::text, "", "", webdar_css_style::width_100vw),
    listing_form("Load selected"),
    listing_fs("Available configurations"),
    delete_selected("Delete loaded config", event_delete),
    clear_cur_config("Clear", event_clear),
    change_event_name(ch_event_name),
    around_fs(""),
    around_form("Update"),
    ignore_events(false)
{
	// non html field settup
    biblio = ptr;
    if(!biblio)
	throw WEBDAR_BUG;

    wrapped = std::move(obj);
    if(wrapped.get() == nullptr)
	throw WEBDAR_BUG;

    wrapped_jsoner = dynamic_cast<jsoner*>(wrapped.get());
    if(wrapped_jsoner == nullptr)
	throw WEBDAR_BUG; // should be object inheriting from class jsoner

    wrapped_body_builder = dynamic_cast<body_builder*>(wrapped.get());
    if(wrapped_body_builder == nullptr)
	throw WEBDAR_BUG; // should be object inheriting from class body_builder

    wrapped_events = dynamic_cast<events*>(wrapped.get());
    if(wrapped_events == nullptr)
	throw WEBDAR_BUG; // shold also be object inheriting from class events

    wrapped_subconfig = dynamic_cast<bibliotheque_subconfig*>(wrapped.get());
	// it is ok if wrapped_subconfig is nullptr, wrapped may not implement
	// the bibliotheque_subconfig interface

    if(change_event_name == html_form_input::changed)
	throw WEBDAR_BUG; // event name collides with our own components
    if(change_event_name == event_delete)
	throw WEBDAR_BUG; // event name collides with our own components
    if(change_event_name == html_form_radio::changed)
	throw WEBDAR_BUG; // event name collides with our own components
    if(change_event_name == bibliotheque::changed(categ))
	throw WEBDAR_BUG; // event name collides with our own components
    if(change_event_name == html_form::changed)
	throw WEBDAR_BUG; // event name collides with our own components


	// html components setup
    clear_warning();
    load_listing();
    need_saving.clear();
    need_saving.add_text(0, "configuration not saved");
    need_saving.set_visible(false);

	// adoption tree
    listing_fs.adopt(&listing);
    listing_form.adopt(&listing_fs);
    floteur.adopt(&listing_form);

    floteur.adopt(&delete_selected);
    floteur.adopt(&clear_cur_config);
    adopt(&floteur);

    adopt(&warning_message);

    around_fs.adopt(wrapped_body_builder);
    if(add_form_around)
    {
	around_form.adopt(&around_fs);
	adopt(&around_form);
    }
    else
	adopt(&around_fs);

    config_fs.adopt(&config_name);
    config_fs.adopt(&need_saving);
    config_form.adopt(&config_fs);
    adopt(&config_form);
    adopt(&confirm);

	// events and actors
    wrapped_events->record_actor_on_event(this, change_event_name);
    config_form.record_actor_on_event(this, html_form::changed);
    delete_selected.record_actor_on_event(this, event_delete);
    clear_cur_config.record_actor_on_event(this, event_clear);
    confirm.record_actor_on_event(this, html_yes_no_box::answer_yes);

    listing.record_actor_on_event(this, html_form_radio::changed);
    ptr->record_actor_on_event(this, bibliotheque::changed(categ));

	// css
    webdar_css_style::normal_button(delete_selected);
    delete_selected.add_css_class(webdar_css_style::spacing_vertical);
    webdar_css_style::normal_button(clear_cur_config);
    clear_cur_config.add_css_class(webdar_css_style::spacing_vertical);
    floteur.add_css_class(css_float);
    warning_message.add_css_class(css_warn);
    need_saving.add_css_class(css_warn);
    config_form.add_css_class(css_margin_above);
}

template <class T> void arriere_boutique<T>::on_event(const std::string & event_name)
{
    if(ignore_events)
	return;

    if(!biblio)
	throw WEBDAR_BUG;

    if(event_name == change_event_name)
    {
	    // the hosted object configuration has been changed by the user

	need_saving.set_visible(true);
	clear_warning();
    }
    else if(event_name == html_form::changed)
    {

	    // save as form has changed (-> asked to save as)

	need_saving.set_visible(true);
	if(config_name.get_value().empty())
	    set_warning("Cannot save a configuration without a name");
	else
	{
		// we use a config temporary variable here
		// to be sure the save_json() method is called
		// before the get_using_set() one, as the first
		// may have impact on the second, some components
		// making some cleanup (forgetting the unselected
		// alternatives) within save_json()
	    json config = wrapped_jsoner->save_json();

	    try
	    {
		if(config_name.get_value() != currently_loaded)
		{
		    currently_loaded = config_name.get_value();

		    if(wrapped_subconfig == nullptr)
			biblio->add_config(categ,
					   config_name.get_value(),
					   config);
		    else
			biblio->add_config(categ,
					   config_name.get_value(),
					   config,
					   wrapped_subconfig->get_using_set());
		}
		else
		{
		    if(wrapped_subconfig == nullptr)
			biblio->update_config(categ,
					      currently_loaded,
					      config);
		    else
			biblio->update_config(categ,
					      currently_loaded,
					      config,
					      wrapped_subconfig->get_using_set());
		}
		    // these previous alternatives all trigger bibliotheque::changed(categ) event
		need_saving.set_visible(false);
		clear_warning();
	    }
	    catch(exception_range & e)
	    {
		set_warning(libdar::tools_printf("Failed saving configuration as %s: %s",
						 config_name.get_value().c_str(),
						 e.get_message().c_str()));
	    }
	}
    }
    else if(event_name == event_delete)
    {
	    // user asked to delete the selected configuration

	    // asking confirmation
	confirm.ask_question("Are you sure to delete the loaded configuration?", false);
    }
    else if(event_name == html_yes_no_box::answer_yes)
    {
	    // user confirm configuration deletion

	if(listing.is_selected())
	{
	    std::string todelete = listing.get_selected_id();

	    silently_update_config_name(currently_loaded);
	    biblio->delete_config(categ, todelete);
		// this triggers bibliotheque::changed(categ) event
	    currently_loaded = "";
	    need_saving.set_visible(true);
	    clear_warning();
	}
	else
	    set_warning("Select and load first the configuration to be deleted");
    }
    else if(event_name == event_clear)
    {
	currently_loaded = "";
	config_name.set_value("");
	wrapped_jsoner->clear_json();
	need_saving.set_visible(true);
	listing.unset_selected();
	clear_warning();
    }
    else if(event_name == html_form_radio::changed)
    {
	    // user selected a configuration to load

	ignore_events = true;
	try
	{
	    if(listing.is_selected())
	    {
		currently_loaded = listing.get_selected_id();
		silently_update_config_name(currently_loaded);
		wrapped_jsoner->load_json(biblio->fetch_config(categ, currently_loaded));
		need_saving.set_visible(false);
		clear_warning();
	    }
	    else
		set_warning("Select first the configuration to load");
	}
	catch(...)
	{
	    ignore_events = false;
	    throw;
	}
	ignore_events = false;
    }
    else if(event_name == bibliotheque::changed(categ))
    {
	ignore_events = true;
	try
	{
	    load_listing();

		// repositionning the selected object to the currently loaded config if any
	    if(!currently_loaded.empty())
	    {
		if(biblio->has_config(categ, currently_loaded))
		    listing.set_selected_id(currently_loaded);
		else
		{
		    currently_loaded = "";
		    silently_update_config_name(currently_loaded);
		}
	    }
	}
	catch(...)
	{
	    ignore_events = false;
	    throw;
	}
	ignore_events = false;
    }
    else
	throw WEBDAR_BUG;
}

template <class T> std::string arriere_boutique<T>::inherited_get_body_part(const chemin & path,
									    const request & req)
{
    return get_body_part_from_all_children(path, req);
}

template <class T> void arriere_boutique<T>::load_listing()
{
    std::deque<std::string> content = biblio->listing(categ);

    listing.clear();

    for(std::deque<std::string>::iterator it = content.begin(); it != content.end(); ++it)
	listing.add_choice(*it, *it);

    listing.unset_selected();
}

template <class T> void arriere_boutique<T>::new_css_library_available()
{
    css tmp;
    std::unique_ptr<css_library> & csslib = lookup_css_library();

    if(!csslib)
	throw WEBDAR_BUG;

    if(!csslib->class_exists(css_float))
    {
	tmp.clear();
	tmp.css_border_width(css::bd_all, css::bd_medium);
	tmp.css_float(css::fl_right);
	tmp.css_margin_left("1em");
	csslib->add(css_float, tmp);
    }

    if(!csslib->class_exists(css_warn))
    {
	tmp.clear();
	tmp.css_color(RED);
	tmp.css_font_weight_bold();
	tmp.css_text_shadow("0.1em", "0.1em", "0.1em", "#888888");
	csslib->add(css_warn, tmp);
    }

    if(!csslib->class_exists(css_margin_above))
    {
	tmp.clear();

	tmp.css_margin_top("2em");

	csslib->add(css_margin_above, tmp);
    }


    webdar_css_style::update_library(*csslib);
}

template <class T> void arriere_boutique<T>::set_warning(const std::string & wm)
{
    warning_message.clear();
    warning_message.add_text(0, wm);
    warning_message.set_visible(true);
}

template <class T> void arriere_boutique<T>::silently_update_config_name(const std::string & val)
{
    bool original_status = ignore_events;

    ignore_events = true;
    try
    {
	config_name.set_value(val);
	    // without ignore_events, this would
	    // trigger the html_form_input::changed event
	    // leading to save the configuration
    }
    catch(...)
    {
	ignore_events = original_status;
	throw;
    }
    ignore_events = original_status;
}

#endif
