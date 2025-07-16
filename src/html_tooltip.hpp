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

#ifndef HTML_TOOLTIP_HPP
#define HTML_TOOLTIP_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"

    /// class html_tooltip is a partial body_builder implementation for inherited class to implement tooltips

    /// class html_tooltip is not a full independant body_builder object, the
    /// class should be the base class for inheritance, to provides the set_tooltip()
    /// method and its get_body_part() method should be called from within the body_builder::get_body_part(*,*)
    /// method of the inherited class or owner class within the <X> </X> structure to provide
    /// tooltip for. (replace X by label or legend for example)
    /// also note that the new_css_class_library_available() should be called from the inherited class
    /// for the css_ibrary to get fed by the tooltip related CSS definitions

class html_tooltip : public body_builder
{
public:
	/// \param[in] x_tooltip the helper text that should popup upon mouse hover
    html_tooltip() {};
    html_tooltip(const html_tooltip & ref) = default;
    html_tooltip(html_tooltip && ref) noexcept = delete;
    html_tooltip & operator = (const html_tooltip & ref) = default;
    html_tooltip & operator = (html_tooltip && ref) noexcept = delete;
    ~html_tooltip() = default;

	/// get the helper string to show as popup
    const std::string & get_tooltip() const { return tooltip; };

	/// set the helper to the tooltip (helper popup)
    void set_tooltip(const std::string & helper);

protected:

 	/// inherited from body_builder
    virtual void new_css_library_available() override;

	/// inherited from body builder

	/// \note we forbid any adoption throwing an exception from this call
    virtual void has_adopted(body_builder *obj) { throw WEBDAR_BUG; };


	/// \note body_builder::get_body_part(*) is left undefined and thus stays a pure virtual method
    std::string get_tooltip_body_part() const;


private:
    std::string tooltip;

    static constexpr const char* css_tooltip = "tooltip";
    static constexpr const char* css_tooltiptext = "tooltiptext";
};

#endif
