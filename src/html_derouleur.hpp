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

#ifndef HTML_DEROULEUR_HPP
#define HTML_DEROULEUR_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <string>
#include <list>
#include <map>

    // webdar headers
#include "body_builder.hpp"
#include "html_aiguille.hpp"
#include "actor.hpp"
#include "html_button.hpp"

    /// class html_derouleur is a pure virtual class

    /// it groups adopted objets under several labeled sections
    /// which can be expanded and shrinked, still the section
    /// title stay visible, but only one section content is visible
    /// at a any given time


class html_derouleur : public body_builder,
		       public actor
{
public:
    html_derouleur() { adopt(&switcher); clear(); };
    html_derouleur(const html_derouleur & ref) = delete;
    html_derouleur(html_derouleur && ref) noexcept = delete;
    html_derouleur & operator = (const html_derouleur & ref) = delete;
    html_derouleur & operator = (html_derouleur && ref) noexcept = delete;
    ~html_derouleur() = default;

	/// clear all adopted data and remove all sections
    void clear();

    void add_section(const std::string & name, const std::string & title);

    void adopt_in_section(const std::string & section_name, body_builder* obj);

    void adopt_in_section(signed int num, body_builder* obj);

    void clear_section(const std::string & section_name) { switcher.clear_section(section_name); };

    void clear_section(signed int num) { switcher.clear_section(num); };

    void remove_section(const std::string & section_name);

    void set_active_section(const std::string & name) { switcher.set_active_section(name); };

    void set_active_section(signed int num) { switcher.set_active_section(num); };

    unsigned int size() const { return switcher.size(); };

	/// hide/unhide a section (Warning! this is not the same thing as shrinking/expanding a section!)

	/// \note invisible section not only shrink (their content is no more visible) but the
	/// section title itself disapears and the section does not show at all to the user
    void section_set_visible(const std::string & name, bool visible);

	/// set css of URL titles
    void url_clear_css_classes() { css_url.clear_css_classes(); };
    void url_add_css_class(const std::string & name);
    void url_add_css_class(const css_class_group & name);

	// from actor
    virtual void on_event(const std::string & event_name) override;

protected:

	// inherited from body_builder
    virtual void css_classes_have_changed() override;

	// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    static const std::string shrink_event;

	/// struct used to store for each section the title components and status
    struct section
    {
	html_button* title;
	html_button* shrinker;
	bool visible;    ///< a section can be expanded or shrinked or totally hidden (visible == false)

	    // by default shrinker is hidden and title is
	    // visible (when the section is not expanded)
	    // clicking on title expands the section
	    // and leads the shrinker button to show while
	    // title get invisible, and so on...

	section() { title = nullptr; shrinker = nullptr; visible = true; };
	section(const section & arg) = delete;
	section(section && arg) = default;
	section & operator = (const section & arg) = delete;
	section & operator = (section && arg) = default;
	~section() { if(title != nullptr) delete title; if(shrinker != nullptr) delete shrinker; };
    };

    css_class_group css_url;                    ///< css classes to apply to title bar urls
    std::map<std::string, section> sections;    ///< map section name to its content and provide the title bars above each

    html_aiguille switcher;                     ///< holds the objects adopted inside the different sections


	/// used from inherited_get_body_part to avoid duplicated code
    std::string generate_html(const chemin & path,
			      const request & req);

};

#endif
