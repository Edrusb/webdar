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

#ifndef HTML_FORM_FIELDSET_HPP
#define HTML_FORM_FIELDSET_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"
#include "html_legend.hpp"

    /// class html_form_fieldset implements HTML fieldset feature

    /// it is expected to be adopted by an html_form and to
    /// adopt itself some html_form_field objects like html_form_input
    /// html_comparison_field, html_form_radio, html_datetime
    /// Visually an html_form_fieldset object surrounds the objects
    /// it has adopted by a line and show a title on top, which is
    /// the argument provided to its constructor (the label)

class html_form_fieldset : public body_builder
{
public:
    html_form_fieldset(const std::string & label):legend(label) { adopt(&legend); };
    html_form_fieldset(const html_form_fieldset & ref) = default;
    html_form_fieldset(html_form_fieldset && ref) noexcept = delete;
    html_form_fieldset & operator = (const html_form_fieldset & ref) = default;
    html_form_fieldset & operator = (html_form_fieldset && ref) noexcept = delete;
    ~html_form_fieldset() = default;

    void change_label(const std::string & label) { legend.change_label(label); };
    const std::string get_label() const { return legend.get_label(); };

	/// css is only applied css to the title of the form fieldset
    void add_label_css_class(const std::string & name) { legend.add_css_class(name); };

	/// css is only applied css to the title of the form fieldset
    void remove_label_css_class(const std::string & name) { legend.remove_css_class(name); };

	/// clear and destroy previously added objects
    void clear() { orphan_all_children(); adopt(&legend); };

	/// inherited from body_builder
    virtual void bind_to_anchor(const std::string & val) override { update_anchor(val); };

	/// surfacing the tooltip feature of html_legend
    void set_tooltip(const std::string & val) { legend.set_tooltip(val); };


protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void has_adopted(body_builder *obj) override;


private:
    html_legend legend;
    std::string bounded_anchor;

    void update_anchor(const std::string & val);


};

#endif
