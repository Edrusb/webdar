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

#ifndef HTML_AIGUILLE_HPP
#define HTML_AIGUILLE_HPP

    /// \file html_aiguille.hpp
    ///
    ///  html_aiguille class is a per section adoption tool that shows objects adopted
    ///  in one section at a time (or no section at all), thanks to it set_active_section() method

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
#include "actor.hpp"
#include "html_void.hpp"

    /// class html_aiguille is a pure virtual class

    /// it groups adopted objets under several labeled sections
    /// at most one section is visible at a time and can be changed
    /// calling set_active_section(). If given "noactive" to set_active_section()
    /// no section will show. By default no section is created, one has
    /// to be created calling add_section() for the object to be able to
    /// adopt other objects.
    /// adopt() method from body_builder is replaced by adopt_in_section()

class html_aiguille : public body_builder,
		      public html_void_parent_notifier
{
public:
    static constexpr const signed int noactive = -1;

    html_aiguille() { clear(); };
    html_aiguille(const html_aiguille & ref) = delete;
    html_aiguille(html_aiguille && ref) noexcept = delete;
    html_aiguille & operator = (const html_aiguille & ref) = delete;
    html_aiguille & operator = (html_aiguille && ref) noexcept = delete;
    ~html_aiguille() = default;

	/// clear all adopted data and remove all sections
    void clear();

	/// add a new sections

	/// \param[in] name is the name of the section, not two sections
	/// can have the same name, this is code internal information which
	/// can be used to be referred to.
	/// \param[in] title is what can shows as a section title, may be the
	/// the same title of another section, can also be tranlated for
	/// localization, code should not assume anything based in the value
	/// of this field.
    void add_section(const std::string & name, const std::string & title);

	/// adopt another objet in the section which name is provided

	/// \note adopt_in_section replaces body_builder::adopt() but forsake()
	/// is still available do revent this action.
    void adopt_in_section(const std::string & section_name, body_builder* obj);

	/// adopt an object in seection which number is provided
    void adopt_in_section(signed int num, body_builder* obj);

	/// foresake all adopted objets in the given section
    void clear_section(const std::string & section_name);

	/// foresake all adopted objects in the given section
    void clear_section(signed int num);

	/// foresake objects and remove section of that name
    void remove_section(const std::string & section_name);

	/// foresake objects and remove section at that index
    void remove_section(signed int num);

	/// manually set the visible section, based on section name
    void set_active_section(const std::string & name);

	/// manually set the visible section number (numbered by ordre or addition, starting at zero)
    void set_active_section(signed int num);

	/// get the current active section
    signed int get_active_section() const { return active_section; };

	/// return the total number of section
    unsigned int size() const { return order.size(); };

	/// return the index of the provided section name
    unsigned int section_name_to_num(const std::string & name) const;

	/// return the name of the section knowing its index
    std::string num_to_section_name(unsigned int num) const { return order[num]; };

	/// inherited from html_void_parent_notifier
    virtual void void_child_will_foresake(body_builder* voidobj, body_builder* obj) override;

protected:

	// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	// hiding adopt() as it is replaced by adopt_in_section()
    using body_builder::adopt;

	// for inherited class to act upon new section addition
    virtual void section_added(const std::string & name, const std::string & title) {};

	// for inherited class to act upon section removal
    virtual void section_removed(const std::string & name) {};


private:

	/// used to store the objects adopted in each section
    struct section
    {
	std::string title;
	html_void global_visibility; // parent of all objects from the adopted list, and child of "this"
	std::list<body_builder*> adopted;
    };

    std::deque<std::string> order;              ///< holds section name in order
    std::map<std::string, section> sections;    ///< map section name to its content
    std::map<body_builder*, std::string> obj_to_section; ///< maps back an object to it section name
    signed int active_section; ///< set to noactive if no section is expanded
    bool selfcleaning; ///< when true, ignore will_foresake() calls, we are at the root of this

    void set_visibility(signed int section_num, bool visible);
};

#endif
