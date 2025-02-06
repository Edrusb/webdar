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

#ifndef HTML_FORM_IGNORE_AS_SYMLINK_HPP
#define HTML_FORM_IGNORE_AS_SYMLINK_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <set>

    // webdar headers
#include "html_form_dynamic_table.hpp"
#include "jsoner.hpp"
#include "actor.hpp"
#include "events.hpp"

    /// class html_form_ignore_as_symlink provide interface for the user to give a list of gnupg signatories and gnupg recipients

    /// this html components is consitued of:
    ///

    /** \verbatim
	+---html_form_dynamic_table-----------------+-------+
	|                                           |       |
	|+---------------------------------------+  |+-+    |
	||<html_form_input_file>                 |  || | del|
	|+---------------------------------------+  |+-+    |
	|+---------------------------------------+  |+-+    |
	||<html_form_input_file>                 |  || | del|
	|+---------------------------------------+  |+-+    |
	|+---------------------------------------+  |+-+    |
	||<html_form_input_file>                 |  || | del|
	|+---------------------------------------+  |+-+    |
	|+---------------------------------------+  |+-+    |
	||<html_form_input_file>                 |  || | del|
	|+---------------------------------------+  |+-+    |
	+-------------------------------------------+-------+

	+------------+
	|  add entry |
	+------------+

	\endverbatim **/

    ///
    /// \note this component should adopted by an html_form directly
    /// or not (through an html_form_fieldset for example)
    ///



class html_form_ignore_as_symlink : public body_builder,
				    public html_form_dynamic_table_object_provider,
				    public jsoner,
				    public actor,
				    public events
{
public:
    static const std::string changed;

    html_form_ignore_as_symlink();
    html_form_ignore_as_symlink(const html_form_ignore_as_symlink & ref);
    html_form_ignore_as_symlink(html_form_ignore_as_symlink && ref) noexcept = delete;
    html_form_ignore_as_symlink & operator = (const html_form_ignore_as_symlink & ref) = delete;
    html_form_ignore_as_symlink & operator = (html_form_ignore_as_symlink && ref) noexcept = delete;
    ~html_form_ignore_as_symlink() = default;

	/// whether some symlinks have been set in the list
    bool empty() const { return table.empty(); };

	/// return the recipient list filled by the webuser
    std::set<std::string> get_symlink_list() const;

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

private:
    html_form_dynamic_table table;

    std::vector<std::string> gather_content_of_type(unsigned int type) const;

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_form_ignore_as_symlink";

    static constexpr const char* jlabel_contents = "contents";

};

#endif
