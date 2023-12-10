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

    /// class html_derouleur is a pure virtual class
    ///
    /// it groups adopted objets under several labeled sections
    /// which can be expanded and shrinked, still the section
    /// title stay visible, but only one section content is visible
    /// at a any given time


class html_derouleur : protected body_builder
{
public:
    html_derouleur() { clear(); };
    html_derouleur(const html_derouleur & ref) = delete;
    html_derouleur(html_derouleur && ref) noexcept = delete;
    html_derouleur & operator = (const html_derouleur & ref) = delete;
    html_derouleur & operator = (html_derouleur && ref) noexcept = delete;
    ~html_derouleur() = default;

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

	// --- redirect to the protected inherited body_builder class
	// having proteected inheritance allow hiding adopt() and foresake()
	// which are replaced by the per section methodes defined above

	/*
    void set_prefix(const chemin & prefix) { body_builder::set_prefix(prefix); };
    void set_visible(bool mode) { body_builder::set_visible(mode); };
    bool get_visible() const { return body_builder::get_visible(); };
    bool get_next_visible() const { return body_builder::get_next_visible(); };
    void add_css_class(const std::string & name) { body_uilder::add_css_class(name); };
    void add_css_class(const css_class_group & cg) { body_builder::add_css_class(cg); };
    bool has_css_class(const std::string & name) { return body_builder::has_css_class(name); };
    void remove_css_class(const std::string & name) { body_builder::remove_css_class(name); };
    void remove_css_class(const css_class_group & cg) { body_builder::remove_css_class(cg); };
    void clear_css_classes() { body_builder::clear_css_classes(); };
    const std::set<std::string> & get_css_clases_as_a_set() const { return body_builder::get_css_classes_as_a_set(); };
    css_class_group get_css_class_group() const { return body_builder::get_css_class_group(); };
    std::string get_css_classes() const { return body_builder::get_css_classes(); };
    void define_css_class_in_library(const css_class & csscl) { body_builder::define_css_class_jn_library(csscl); };
    void define_css_class_in_library(const std::string & name, const css & cssdef) { body_builder::define_css_class_in_library(name, cssdef); };
    bool is_css_class_defined_in_library(const std::string & name) const { body_builder::is_css_class_defined_in_library(name); };
    std::string get_body_part(const chemin & path,
			      const request & req) { return body_builder::get_body_part(path, req); };
    void set_no_CR() { body_builder::set_no_CR() };
	*/

    using body_builder::set_prefix;
    using body_builder::set_visible;
    using body_builder::get_visible;
    using body_builder::get_next_visible;
    using body_builder::add_css_class;
    using body_builder::has_css_class;
    using body_builder::remove_css_class;
    using body_builder::clear_css_classes;
    using body_builder::get_css_classes_as_a_set;
    using body_builder::get_css_class_group;
    using body_builder::get_css_classes;
    using body_builder::define_css_class_in_library;
    using body_builder::is_css_class_defined_in_library;
    using body_builder::get_body_part;
    using body_builder::set_no_CR;

protected:

	// inherited from body_builder
    virtual void will_foresake(body_builder *obj) override;

    	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    static constexpr const signed int noactive = -1;

    struct section
    {
	std::string title;
	std::list<body_builder*> adopted;
    };

    std::deque<std::string> order;              ///< holds section name in order
    std::map<std::string, section> sections;    ///< map section name to its content
    std::map<body_builder*, std::string> obj_to_section; ///< maps back an object to it section name
    signed int active_section; ///< set to noactive if no section is expanded
    bool selfcleaning;
};

#endif
