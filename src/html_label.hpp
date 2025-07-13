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

#ifndef HTML_LABEL_HPP
#define HTML_LABEL_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"
#include "html_text.hpp"

    /// class html_label implements label and tooltips (helper string showing when mouse hovering)

    /// it is not expect to adopt any child, but to be adopted

class html_label : public body_builder, public static_body_builder
{
public:
	/// \param[in] x_label the text to show as label
    html_label(const std::string & x_label);

	/// \param[in] x_label the text to show as label
	/// \param[in] x_tooltips the helper text that should popup upon mouse hover
    html_label(const std::string & x_label, const std::string & x_tooltip);
    html_label(const html_label & ref) = default;
    html_label(html_label && ref) noexcept = delete;
    html_label & operator = (const html_label & ref) = default;
    html_label & operator = (html_label && ref) noexcept = delete;
    ~html_label() = default;

	/// change the label value from the value provided at construction time
    void set_label(const std::string & val) { label = val; };

	/// get the current value of the label
    const std::string & get_label() const { return label; };

	/// add a tooltip to the label (helper popup)
    void set_tooltip(const std::string & helper) { tooltip = helper; };

	/// set the label in bold or equivalent method to emphase it compare to the others
    void set_emphase(bool em) { emphase = em; };

	/// define the 'for' field to link this label to other CSS structure
    void set_for_field(const std::string & val) { for_field = val; };

	/// read the current 'for' field value
    const std::string & get_for_field() const { return for_field; };

	/// inherited from static_body_builder
    virtual std::string get_body_part() const override;

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

 	/// inherited from body_builder
    virtual void new_css_library_available() override;

	/// inherited from body builder

	/// \note we forbid any adoption throwing an exception from this call
    virtual void has_adopted(body_builder *obj) { throw WEBDAR_BUG; };


private:
    std::string label;
    std::string tooltip;
    std::string for_field;
    bool emphase;

    static constexpr const char* css_tooltip = "tooltip";
    static constexpr const char* css_tooltiptext = "tooltiptext";
};

#endif
