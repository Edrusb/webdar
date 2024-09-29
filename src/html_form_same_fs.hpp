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

#ifndef HTML_FORM_SAME_FS_HPP
#define HTML_FORM_SAME_FS_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_form_dynamic_table.hpp"

    /// class html_form_same_fs provide interface for the user to filter base on mounted filesystem

    /// this html components is consitued of:
    ///
    /// +----html_form_dynamic_table---------------+-------+
    /// |                                          |       |
    /// +---------------------------------------+  |+-+    |
    /// |<html_form_input::text>                |  || | del|
    /// |---------------------------------------+  |+-+    |
    /// +---------------------------------------+  |+-+    |
    /// |<html_form_input::text>                |  || | del|
    /// |---------------------------------------+  |+-+    |
    /// +---------------------------------------+  |+-+    |
    /// |<html_form_input::text>                |  || | del|
    /// |---------------------------------------+  |+-+    |
    /// +---------------------------------------+  |+-+    |
    /// |<html_form_input::text>                |  || | del|
    /// |---------------------------------------+  |+-+    |
    /// +------------------------------------------+-------+
    ///
    /// +------------+
    /// |  add entry |
    /// +------------+
    ///
    ///
    /// \note this component should adopted by an html_form directly
    /// or not (through an html_form_fieldset for example)
    ///

class html_form_same_fs : public body_builder, public html_form_dynamic_table_object_provider
{
public:
    html_form_same_fs();
    html_form_same_fs(const html_form_same_fs & ref);
    html_form_same_fs(html_form_same_fs && ref) noexcept = delete;
    html_form_same_fs & operator = (const html_form_same_fs & ref) = delete;
    html_form_same_fs & operator = (html_form_same_fs && ref) noexcept = delete;
    ~html_form_same_fs() = default;

	/// return the included fs
    std::vector<std::string> get_included_fs_path() const;

	/// return the excluded fs
    std::vector<std::string> get_excluded_fs_path() const;

	/// inherited from html_form_dynamic_table_object_provider
    virtual std::unique_ptr<body_builder> provide_object_of_type(unsigned int num,
								 const std::string & context) const override;


protected:
	/// inherited methods from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    html_form_dynamic_table table;

    std::vector<std::string> gather_content_of_type(unsigned int type) const;
};

#endif