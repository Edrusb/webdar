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

#ifndef HTML_FORM_GNUPG_LIST_HPP
#define HTML_FORM_GNUPG_LIST_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_form_dynamic_table.hpp"
#include "jsoner.hpp"
#include "events.hpp"

    /// class html_form_gnupg_list provide interface for the user to give a list of gnupg signatories and gnupg recipients

    /// this html components is consitued of:
    ///
    /** \verbatim
	+---html_form_dynamic_table-----------------+-------+
	|                                           |       |
	|+---------------------------------------+  |+-+    |
	||<html_form_input::text>                |  || | del|
	|+---------------------------------------+  |+-+    |
	|+---------------------------------------+  |+-+    |
	||<html_form_input::text>                |  || | del|
	|+---------------------------------------+  |+-+    |
	|+---------------------------------------+  |+-+    |
	||<html_form_input::text>                |  || | del|
	|+---------------------------------------+  |+-+    |
	|+---------------------------------------+  |+-+    |
	||<html_form_input::text>                |  || | del|
	|+---------------------------------------+  |+-+    |
	+-------------------------------------------+-------+

	+------------+
	|  add entry |
	+------------+
	\endverbatim **/

    /// \note this component should adopted by an html_form directly
    /// or not (through an html_form_fieldset for example)
    ///



class html_form_gnupg_list : public body_builder,
			     public html_form_dynamic_table_object_provider,
			     public jsoner,
			     public events,
			     public actor
{
public:
    static const std::string changed;

    html_form_gnupg_list();
    html_form_gnupg_list(const html_form_gnupg_list & ref);
    html_form_gnupg_list(html_form_gnupg_list && ref) noexcept = delete;
    html_form_gnupg_list & operator = (const html_form_gnupg_list & ref) = delete;
    html_form_gnupg_list & operator = (html_form_gnupg_list && ref) noexcept = delete;
    ~html_form_gnupg_list() = default;

	/// return the recipient list filled by the webuser
    std::vector<std::string> get_gnupg_recipients() const;

    	/// return the list of signatories filled by the webuser
    std::vector<std::string> get_gnupg_signatories() const;

	/// inherited from html_form_dynamic_table_object_provider
    virtual std::unique_ptr<body_builder> provide_object_of_type(unsigned int num,
								 const std::string & context,
								 std::string & changed_event) const override;

	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override;


	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:
	/// inherited methods from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    std::vector<std::string> gather_content_of_type(unsigned int type) const;

    html_form_dynamic_table table;

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_form_gnupg_list";

    static constexpr const char* jlabel_contents = "contents";
};

#endif
