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

#ifndef HTML_FORM_MASK_EXPRESSION_HPP
#define HTML_FORM_MASK_EXPRESSION_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <memory>

    // webdar headers
#include "html_mask.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_input.hpp"
#include "html_form_select.hpp"
#include "actor.hpp"


    /// class html_form_mask_expression provide mean to setup libdar::mask component for filename filtering

    /// this html components is consitued of:
    ///
    /// +----html_fieldset----------------------------------+
    /// |                                                   |
    /// | mask type:  +----------------+                    |
    /// |             |dropdown list   |                    |
    /// |             +----------------+                    |
    /// |                                                   |
    /// |             +--+                                  |
    /// | negate      |  |                                  |
    /// |             +--+                                  |
    /// |             +--+                                  |
    /// | case sensit.|  |                                  |
    /// |             +--+                                  |
    /// |                                                   |
    /// | mask expr.  +----------------------------+        |
    /// |             |  input form                |        |
    /// |             +----------------------------+        |
    /// +---------------------------------------------------+
    ///
    /// \note: this component does must have an html_form object
    /// as parent or ancestor for validation to be processed and
    /// changes to be recorded. Avoiding including the html_form
    /// in that component let it be part of more complex masks
    /// like *and* and *or* combinasons.


class html_form_mask_expression : public html_mask, public actor
{
public:
    html_form_mask_expression();
    html_form_mask_expression(const html_form_mask_expression & ref);
    html_form_mask_expression(html_form_mask_expression && ref) noexcept = default;
    html_form_mask_expression & operator = (const html_form_mask_expression & ref) = default;
    html_form_mask_expression & operator = (html_form_mask_expression && ref) noexcept = default;
    ~html_form_mask_expression() = default;

	/// inherited from html_mask
    std::unique_ptr<libdar::mask> get_mask() const override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;


protected:
	/// inherited methods from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    html_form_fieldset fs;
    html_form_select mask_type;
    html_form_input negate;
    html_form_input casesensitivity;
    html_form_input mask_expression;

    void init();
    std::string tell_action() const;
};

#endif
