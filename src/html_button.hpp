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

#ifndef HTML_BUTTON_HPP
#define HTML_BUTTON_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "events.hpp"
#include "html_url.hpp"
#include "html_div.hpp"
#include "css_class_group.hpp"

    /// simple html component providing the usual button feature of GUIs

    /// \note upon user 'click' on the button a event is generated that
    /// can be used for actors that registered to it, to perform some action
class html_button : public html_div, public events
{
public:
    html_button(const std::string & label, const std::string & x_event_name);
    html_button(const html_button & ref);
    html_button(html_button && ref) noexcept = delete;
    html_button & operator = (const html_button & ref) = delete;
    html_button & operator = (html_button && ref) noexcept = delete;
    ~html_button() = default;

        /// the inherited add_css_class() applies to the html_div around, this one to the url inside
    void url_add_css_class(const std::string & name) { inside.add_css_class(name); };

	/// add a several CSS classes at once for the url inside the button
    void url_add_css_class(const css_class_group & cg) { inside.add_css_class(cg); };

        /// the inherited add_css_class() applies to the html_div around, this one to the url inside
    void url_remove_css_class(const std::string & name) { inside.remove_css_class(name); };

        /// the inherited add_css_class() applies to the html_div around, this one to the url inside
    void url_clear_css_classes() { inside.clear_css_classes(); };

        /// the label is the text show for the URL
    void change_label(const std::string & label) { inside.change_label(label); };

    std::string get_label() const { return inside.get_label(); };

	/// get the URL that will lead this button to be clicked
    std::string get_url() const { return inside.get_url(); };

	/// change the name of the event generating by a click on the html_button

	/// all registered actor are kept on the renamed event
    void change_event_name_to(const std::string & x_event_name) { rename_name(event_name, x_event_name); event_name = x_event_name; };

	/// get the name of the event generated by this object
    const std::string & get_event_name() const { return event_name; };

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

        /// inherited from body_builder
    virtual void path_has_changed() override;

private:
    html_url inside;
    std::string event_name;

    void init();

    static const std::string action;
};

#endif
