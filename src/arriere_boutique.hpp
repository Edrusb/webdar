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
#include "webdar_css_style.hpp"
#include "html_div.hpp"
#include "tokens.hpp"

    /// class arriere_boutique provides mean to add/load a given component type to/from a bibliotheque object

    /// this template receive as type the class to manipulate, which must be from a jsoner *and* a
    /// body_builder *and* events inherited classes.
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
    arriere_boutique(const std::shared_ptr<bibliotheque> & ptr,
		     bibliotheque::category cat,
		     std::unique_ptr<T> & obj);
    arriere_boutique(const arriere_boutique & ref) = delete;
    arriere_boutique(arriere_boutique && ref) noexcept = delete;
    arriere_boutique & operator = (const arriere_boutique & ref) = delete;
    arriere_boutique & operator = (arriere_boutique && ref) noexcept = delete;
    ~arriere_boutique() = default;

	/// refresh displayed status from the referred bibliotheque object
    void refresh();

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
    static constexpr const char* css_float = "arriere_boutique_float";
    static constexpr const char* css_warn = "arriere_boutique_warn";

    std::string currently_loaded;
    std::shared_ptr<bibliotheque> biblio;
    bibliotheque::category categ;

    html_text warning_message;
    html_text need_saving;
    html_form_fieldset config_fs;
    html_form config_form;
    html_form_input config_name;
    html_form listing_form;
    html_form_fieldset listing_fs;
    html_form_radio listing;
    html_double_button delete_selected;
    html_div floteur;

    std::unique_ptr<T> wrapped;
    jsoner* wrapped_jsoner;
    body_builder* wrapped_body_builder;
    events* wrapped_events;

    bool ignore_events;

    void load_listing(); ///< fills the listing from existing config in biblio
    void set_warning(const std::string & wm);
    void clear_warning() { warning_message.set_visible(false); };
};


template <class T> arriere_boutique<T>::arriere_boutique(const std::shared_ptr<bibliotheque> & ptr,
							 bibliotheque::category cat,
							 std::unique_ptr<T> & obj):
    currently_loaded(""),
    categ(cat),
    config_form("Save/Save as"),
    config_fs(""),
    config_name("Configuration name", html_form_input::text, "", "50"),
    listing_form("Load selected"),
    listing_fs("Available configurations"),
    delete_selected("Delete loaded config", event_delete),
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
    adopt(&floteur);

    adopt(&warning_message);

    adopt(wrapped_body_builder);
    config_fs.adopt(&config_name);
    config_fs.adopt(&need_saving);
    config_form.adopt(&config_fs);
    adopt(&config_form);

	// events and actors
    wrapped_events->record_actor_on_event(this, T::changed);
    config_name.record_actor_on_event(this, html_form_input::changed);
    delete_selected.record_actor_on_event(this, event_delete);
    listing.record_actor_on_event(this, html_form_radio::changed);

	// css
    webdar_css_style::normal_button(delete_selected);
    floteur.add_css_class(css_float);
    warning_message.add_css_class(css_warn);
    need_saving.add_css_class(css_warn);
}

template <class T> void arriere_boutique<T>::refresh()
{
    config_name.set_value("");
    currently_loaded = "";
    need_saving.set_visible(false);
    clear_warning();
    load_listing();
}

template <class T> void arriere_boutique<T>::on_event(const std::string & event_name)
{
    if(ignore_events)
	return;

    ignore_events = true;
    try
    {
	if(event_name == T::changed)
	{
	    need_saving.set_visible(true);
	    clear_warning();
	}
	else if(event_name == html_form_input::changed)
	{
	    need_saving.set_visible(true);
	    if(config_name.get_value().empty())
		set_warning("Cannot save a configuration without a name");
	    else
	    {
		try
		{
		    if(config_name.get_value() != currently_loaded)
		    {
			biblio->add_config(categ, config_name.get_value(), wrapped_jsoner->save_json());
			currently_loaded = config_name.get_value();
		    }
		    else
			biblio->update_config(categ, currently_loaded,  wrapped_jsoner->save_json());
		    need_saving.set_visible(false);
		    clear_warning();
		    load_listing();
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
	    std::string todelete;

	    if(listing.is_selected())
	    {
		todelete = listing.get_selected_id();
		biblio->delete_config(categ, todelete);
		refresh();
	    }
	    else
		set_warning("Select and load first the configuration to be deleted");
	}
	else if(event_name == html_form_radio::changed)
	{
	    if(listing.is_selected())
	    {
		currently_loaded = listing.get_selected_id();
		config_name.set_value(currently_loaded);
		wrapped_jsoner->load_json(biblio->fetch_config(categ, currently_loaded));
		need_saving.set_visible(false);
		clear_warning();
	    }
	    else
		set_warning("Select first the configuration to load");
	}
	else
	    throw WEBDAR_BUG;
    }
    catch(...)
    {
	ignore_events = false;
	throw;
    }
    ignore_events = false;
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
	tmp.css_border_width(css::bd_all, css::bd_medium, true);
	tmp.css_float(css::fl_right);
	tmp.css_margin_left("1em", true);
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

    webdar_css_style::update_library(*csslib);
}

template <class T> void arriere_boutique<T>::set_warning(const std::string & wm)
{
    warning_message.clear();
    warning_message.add_text(0, wm);
    warning_message.set_visible(true);
}


#endif