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
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <memory>

    // webdar headers
#include "html_mask.hpp"
#include "actor.hpp"
#include "jsoner.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_input.hpp"
#include "html_form_select.hpp"


    /// class html_form_mask_subdir provide mean to setup libdar::mask component for included directories or full path files

    /// this html components is consitued of:
    ///
    /** \verbatim
	+----html_fieldset----------------------------------+
	|                                                   |
	| mask type:  +----------------+                    |
	|             |dropdown list   |                    |
	|             +----------------+                    |
	|                                                   |
	|             +--+                                  |
	| case sensit.|  |                                  |
	|             +--+                                  |
	|                                                   |
	| concerned   +----------------------------+        |
	|     path    |  input form                |        |
	|             +----------------------------+        |
	+---------------------------------------------------+

	\endverbatim **/

    /// \note: this component does must have an html_form object
    /// as parent or ancestor for validation to be processed and
    /// changes to be recorded. Avoiding including the html_form
    /// in that component let it be part of more complex masks
    /// like *and* and *or* combinasons.


class html_form_mask_subdir : public html_mask,
			      public actor,
			      public jsoner
{
public:
    html_form_mask_subdir(bool absolute_path_accepted);
    html_form_mask_subdir(const html_form_mask_subdir & ref);
    html_form_mask_subdir(html_form_mask_subdir && ref) noexcept = default;
    html_form_mask_subdir & operator = (const html_form_mask_subdir & ref) = default;
    html_form_mask_subdir & operator = (html_form_mask_subdir && ref) noexcept = default;
    ~html_form_mask_subdir() = default;

	/// reset object to default
    void clear();

	/// inherited from html_mask
    virtual void set_root_prefix(const libdar::path & x_prefix) override;

	/// inherited from html_mask
    virtual std::unique_ptr<libdar::mask> get_mask() const override;

	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override { clear(); };


	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:
	/// inherited methods from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    bool absolute_ok;
    libdar::path prefix;
    html_form_fieldset fs;
    html_form_select mask_type;
    html_form_input casesensitivity;
    html_form_input regex;
    html_form_input mask_subdir;

    void init();
    std::string tell_action() const;

    static constexpr const unsigned int format_version = 1;

    static constexpr const char* jlabel_absolute = "absolute";
    static constexpr const char* jlabel_prefix = "root";
    static constexpr const char* jlabel_type = "type";
    static constexpr const char* jlabel_casesensit = "casesensit";
    static constexpr const char* jlabel_regex = "regex";
    static constexpr const char* jlabel_mask = "mask";

};

#endif
