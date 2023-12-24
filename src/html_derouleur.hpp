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

#ifndef HTML_DEROULEUR_HPP
#define HTML_DEROULEUR_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>
#include <list>
#include <map>

    // webdar headers
#include "body_builder.hpp"
#include "actor.hpp"
#include "html_button.hpp"

    /// class html_derouleur is a pure virtual class
    ///
    /// it groups adopted objets under several labeled sections
    /// which can be expanded and shrinked, still the section
    /// title stay visible, but only one section content is visible
    /// at a any given time


class html_derouleur : public body_builder, public actor
{
public:
    static constexpr const signed int noactive = -1;

    html_derouleur() { clear(); };
    html_derouleur(const html_derouleur & ref) = delete;
    html_derouleur(html_derouleur && ref) noexcept = delete;
    html_derouleur & operator = (const html_derouleur & ref) = delete;
    html_derouleur & operator = (html_derouleur && ref) noexcept = delete;
    ~html_derouleur() { selfcleaning = true; };

	/// clear all adopted data and remove all sections
    void clear();

	/// add a new sections

	/// \param[in] name is the name of the section, not two sections
	/// can have the same name, this is code internal information which
	/// can be used to be referred.
	/// \param[in] title is what shows in the section title, may be the
	/// the same title of another section, can also be tranlated for
	/// localization, code should not assume anything based in the value
	/// of this field.
    void add_section(const std::string & name, const std::string & title);

	/// adopt another objet in the section which name is provided
    void adopt_in_section(const std::string & section_name, body_builder *obj);

	/// foresake all adopted objets in the given section
    void clear_section(const std::string & section_name);

	/// foresake objects and remove section of that name
    void remove_section(const std::string & section_name);

    	/// manually set the expanded section number (numbered by ordre or addition)

	/// \note to shrink all sections pass noactive static filed as argument
	/// \note section shrinkign and expansion is also driven clicking on each
	/// section title
    void set_active_section(signed int num);

	/// set css of URL titles
    void url_clear_css_classes() { css_url.clear_css_classes(); };
    void url_add_css_class(const std::string & name);
    void url_add_css_class(const css_class_group & name);

	// from actor
    virtual void on_event(const std::string & event_name) override;

protected:

	// inherited from body_builder
    virtual void will_foresake(body_builder *obj) override;

	/// inheroted from body_builder
    virtual void css_classes_have_changed() override;

    	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	// hiding adopt() as it is replaced by adopt_in_section()
    using body_builder::adopt;

private:
    static const std::string shrink_event;

    struct section
    {
	html_button* title;
	html_button* shrinker;
	std::list<body_builder*> adopted;

	    // by default shrinker is hidden and title is
	    // visible (when the section is not expanded)
	    // clicking on title expands the section
	    // and leads the shrinker button to show while
	    // title get invisible, and so on...

	section() { title = nullptr; shrinker = nullptr; };
	section(const section & arg) = delete;
	section(section && arg) = default;
	section & operator = (const section & arg) = delete;
	section & operator = (section && arg) = default;
	~section() { if(title != nullptr) delete title; if(shrinker != nullptr) delete shrinker; };
    };

    std::deque<std::string> order;              ///< holds section name in order
    std::map<std::string, section> sections;    ///< map section name to its content
    std::map<body_builder*, std::string> obj_to_section; ///< maps back an object to it section name
    signed int active_section; ///< set to noactive if no section is expanded
    bool selfcleaning;         ///< when true, ignore will_foresake() calls, we are at the root of this
    bool ignore_events;        ///< disable the on_event() actions
    css_class_group css_url;   ///< css classes to apply to title urls


	/// used from inherited_get_body_part to avoid duplcated code
    std::string generate_html(const chemin & path,
			      const request & req);

};

#endif
