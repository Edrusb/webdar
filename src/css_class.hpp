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

#ifndef CSS_CLASS_HPP
#define CSS_CLASS_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>
#include <map>


    // webdar headers
#include "css.hpp"

    /// class css_class
    ///
    /// associate a name to a css object
    /// in other words contains what HTML standard
    /// defines as a css class: a name associated
    /// to a list of CSS properties.
    /// this class also include selectors like :hover and so on

class css_class
{
public:

	/// selector_type is ordered for :hover being defined after :link and :visited
    enum selector_type
    {
	active,
	checked,
	disabled,
	enabled,
	focus,
	hover,
	link,
	in_range,
	invalid,
	out_of_range,
	read_only,
	read_write,
	root,
	target,
	valid,
	visited
    };

    css_class(const std::string & name);
    css_class(const std::string & name, const css & ref);
    css_class(const css_class & ref) = default;
    css_class(css_class && ref) noexcept = default;
    css_class & operator = (const css_class & ref) = default;
    css_class & operator = (css_class && ref) noexcept = default;
    virtual ~css_class() = default;

	/// change css_class name
    void change_name(const std::string & newname) { class_name = newname; };

	/// get the css_class name
    const std::string & get_name() const { return class_name; };

	/// defines or overwirte the css_class value from a css object
    void set_value(const css & ref) { class_value = ref.css_get_raw_string(); };

	/// defines the value for a given css_selector on that class
    void set_selector(selector_type sel, const css & ref);

	/// remove definition for the given selector type
    void clear_selector(selector_type sel);

	/// clear css_class value and all selector values
    void clear() { class_value = ""; };

	/// returns the css class definition
    std::string get_definition() const;

private:
    std::string class_name;
    std::string class_value;
    std::map<selector_type, std::string> selectors;

    static std::string get_selector_name(selector_type sel);
};

#endif
