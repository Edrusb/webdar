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

#ifndef CSS_CLASS_HPP
#define CSS_CLASS_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <string>
#include <map>


    // webdar headers
#include "css.hpp"

    /// class css_class gather css attributed under a given label

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

    enum pseudo_element_type
    {
	first_line,
	first_letter,
	before,
	after,
	marker,
	selection
    };

    css_class(const std::string & name = "");
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
    void set_value(const css & ref) { class_value = ref; };

	/// obtain the current value
    const css & get_value() const { if(class_name.empty()) throw WEBDAR_BUG; return class_value; };

	/// clear css_class value and all selector values
    void clear_value() { class_value.clear(); };

	/// defines the value for a given css_selector on that class (or pseudo-class in CSS parlance)

	/// \param[in] sel the selector to be defined
	/// \param[in] ref the css definition to give to that selector (= pseudo-class)
	/// \param[in] descendant not an empty string, the definition applies to the specified
	/// descendant components (html type, like "p" for paragraph, "div" or other) or subcomponent with the
	/// provided class name.
	/// \note a descendant component is any component specified inside the current component, for example
	/// in the following html string "<div> demo <p> something </p> something else</div>" "<p>"
	/// is a descendant of "<div>". If one wants a specific css definition for
	/// "<p>" inside "<div>", the descendant string here should be "p" and the current css_class object
	/// be used (or rather referred by its name) by an html_div component. if the descendant is to be
	/// a class name rather than an HTML component name, it should be preceeded by a dot like ".myclassname".
	/// combining HTML component name with class is possible: "p.myclassname" and restricts the
	/// css definition to only "<p>" descendant having the "class=myclassname" attributes.
    void set_selector(selector_type sel,
		      const css & ref,
		      const std::string & descendant = "");

	/// remove definition for the given selector type
    void clear_selector(selector_type sel,
			const std::string & descendant = "");

	/// defines the value of a given pseudo-element

	/// \note same behavior of set_selector() above, but for pseudo-elements instead of pseudo-classes
    void set_pseudo_element(pseudo_element_type pe,
			    const css & val,
			    const std::string & descendant = "");

	/// remove definition for the given pseudo-element
    void clear_pseudo_element(pseudo_element_type pe,
			      const std::string & descendant = "");

	/// clear all css definition, including those provided with selector, only the css name is kept
    void clear() { class_value.clear(); selectors.clear(); };

	/// returns the css class definition
    std::string get_definition() const;

private:
    template<class T> struct sujet
    {
	T type;
	std::string subcomp;

	sujet(T t): type(t) { subcomp.clear(); };
	sujet(T t, const std::string & sub): type(t), subcomp(sub) {};
	sujet(const sujet & ref) = default;
	sujet(sujet && ref) = default;
	sujet & operator = (const sujet & ref) = default;
	sujet & operator = (sujet && ref) = default;
	~sujet() = default;

	    // to be used a index of std::map
	bool operator < (const sujet & ref) const;
    };

    std::string class_name;
    std::string descendant;
    css class_value;
    std::map<sujet<selector_type>, css> selectors;
    std::map<sujet<pseudo_element_type>, css> pseudo_elements;

    static std::string get_selector_name(selector_type sel);
    static std::string get_pseudo_element_name(pseudo_element_type pe);
};

template<class T> bool css_class::sujet<T>::operator < (const sujet & ref) const
{
    if(type < ref.type)
	return true;
    else if(type == ref.type)
	return subcomp < ref.subcomp;
    else
	return false;
}


#endif
