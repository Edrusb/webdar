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

#ifndef HTML_FORM_MASK_SUBDIR_HPP
#define HTML_FORM_MASK_SUBDIR_HPP

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


    /// class html_form_mask_subdir provide mean to setup libdar::mask component for included directories or full path files

    /// this html components is consitued of:
    ///
    /// +----html_fieldset----------------------------------+
    /// |                                                   |
    /// | mask type:  +----------------+                    |
    /// |             |dropdown list   |                    |
    /// |             +----------------+                    |
    /// |                                                   |
    /// |             +--+                                  |
    /// | case sensit.|  |                                  |
    /// |             +--+                                  |
    /// |                                                   |
    /// | concerned   +----------------------------+        |
    /// |     path    |  input form                |        |
    /// |             +----------------------------+        |
    /// +---------------------------------------------------+
    ///
    /// \note: this component does must have an html_form object
    /// as parent or ancestor for validation to be processed and
    /// changes to be recorded. Avoiding including the html_form
    /// in that component let it be part of more complex masks
    /// like *and* and *or* combinasons.


class html_form_mask_subdir : public html_mask
{
public:
    html_form_mask_subdir(bool absolute_path_accepted);
    html_form_mask_subdir(const html_form_mask_subdir & ref);
    html_form_mask_subdir(html_form_mask_subdir && ref) noexcept = default;
    html_form_mask_subdir & operator = (const html_form_mask_subdir & ref) = default;
    html_form_mask_subdir & operator = (html_form_mask_subdir && ref) noexcept = default;
    ~html_form_mask_subdir() = default;

	/// inherited from html_mask
    virtual void set_root_prefix(const libdar::path & x_prefix) override { prefix = x_prefix; };

	/// inherited from html_mask
    virtual std::unique_ptr<libdar::mask> get_mask() const override;

	/// clone() implementation
    MASK_CLONER_MACRO;

protected:
	/// inherited methods from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;


private:
    bool absolute_ok;
    libdar::path prefix;
    html_form_fieldset fs;
    html_form_select mask_type;
    html_form_input casesensitivity;
    html_form_input mask_subdir;

    void init();
};

#endif
