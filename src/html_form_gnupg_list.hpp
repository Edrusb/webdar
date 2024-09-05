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

#ifndef HTML_FORM_TEXT_LIST_HPP
#define HTML_FORM_TEXT_LIST_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_form_dynamic_table.hpp"

    /// class html_form_text_list provide interface for the user to give a list of strings

    /// this html components is consitued of:
    ///
    /// +----html_table-----------------------------+-------+
    /// |                                           |       |
    /// |+---------------------------------------+  |+-+    |
    /// ||<html_form_input::text>                |  || | del|
    /// |+---------------------------------------+  |+-+    |
    /// |+---------------------------------------+  |+-+    |
    /// ||<html_form_input::text>                |  || | del|
    /// |+---------------------------------------+  |+-+    |
    /// |+---------------------------------------+  |+-+    |
    /// ||<html_form_input::text>                |  || | del|
    /// |+---------------------------------------+  |+-+    |
    /// |+---------------------------------------+  |+-+    |
    /// ||<html_form_input::text>                |  || | del|
    /// |+---------------------------------------+  |+-+    |
    /// +-------------------------------------------+-------+
    ///
    /// +------------+
    /// |  add entry |
    /// +------------+
    ///
    ///
    /// \note this component includes an html_form by default
    /// (see constructor argument), but should not when used
    /// recursively from another html_form_text_list


class html_form_text_list : public body_builder
{
public:
    html_form_text_list();
    html_form_text_list(const html_form_text_list & ref);
    html_form_text_list(html_form_text_list && ref) noexcept = delete;
    html_form_text_list & operator = (const html_form_text_list & ref) = delete;
    html_form_text_list & operator = (html_form_text_list && ref) noexcept = delete;
    ~html_form_text_list() = default;

	/// return the recipient list filled by the webuser
    std::vector<std::string> get_gnupg_recipients() const;

    	/// return the list of signatories filled by the webuser
    std::vector<std::string> get_gnupg_signatories() const;

protected:
	/// inherited methods from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    html_form_dynamic_table table;

    std::vector<std::string> gather_content_of_type(unsigned int type) const;
};

#endif
