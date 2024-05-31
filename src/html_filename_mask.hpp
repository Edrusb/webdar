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

#ifndef HTML_FILENAME_MASK_HPP
#define HTML_FILENAME_MASK_HPP

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


    /// class html_filename_mask provide mean to setup libdar::mask component for filename filtering

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
    /// \note: this component does not include the html_form
    /// but must have such component as parent or ancestor for
    /// validation to be processed. In particular if used inside
    /// other components like *and*, *not* or *or* components.


class html_filename_mask : public html_mask
{
public:
    html_filename_mask();
    html_filename_mask(const html_filename_mask & ref) = default;
    html_filename_mask(html_filename_mask && ref) noexcept = default;
    html_filename_mask & operator = (const html_filename_mask & ref) = default;
    html_filename_mask & operator = (html_filename_mask && ref) noexcept = default;
    ~html_filename_mask() = default;

	/// inherited from html_mask
    std::unique_ptr<libdar::mask> get_mask() const override;

protected:
	/// inherited methods from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    html_form_fieldset fs;
    html_form_select mask_type;
    html_form_input negate;
    html_form_input casesensitivity;
    html_form_input mask_expression;
};

#endif
