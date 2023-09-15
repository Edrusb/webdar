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

#ifndef CSS_PROPERTY_HPP
#define CSS_PROPERTY_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers

    /// class css_property hold the value of a single css property

    ///.  it does not only record the value but also the absence of value
    /// when a property is not defined.
    ///.  Last the css_property class add the notion of inheritance:
    ///   A css_property is inheritable or not. The inheritance is used
    ///   when calling inherit_from() method, this is a way to selectively
    ///   copy a value from another css_property

class css_property
{
public:
    css_property() { clear(); };

    void clear();
    void set_inheritance(bool val) { inheritable = val; };
    bool get_inheritance() const { return inheritable; };
    void set_value(const std::string & val);
    const std::string & get_value() const { return value; };
    bool is_unset() const { return unset; };

	/// copy the value from ref if unset and ref has inheritance set
	///
	/// \param[in] ref is the property to copy from
	/// \param|in] any_inheritance, if false, copy is done only if ref has inheritance set, if set to true, copy is done in any case
	/// \param[in] even_if_set, if false, copy is not done if "this" is set, if set to true, the current value is overwritten
    void inherit_from(const css_property & ref, bool any_inheritance=false, bool even_if_set=false);

private:
    std::string value;
    bool inheritable;
    bool unset;

};


#endif
