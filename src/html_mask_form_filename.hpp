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

#ifndef HTML_MASK_FORM_FILENAME_HPP
#define HTML_MASK_FORM_FILENAME_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "body_builder.hpp"
#include "actor.hpp"
#include "events.hpp"
#include "html_form_dynamic_table.hpp"
#include "jsoner.hpp"
#include "html_form.hpp"
#include "html_mask.hpp"
#include "bibliotheque_subconfig.hpp"
#include "html_form_mask_bool.hpp"
#include "html_form_mask_expression.hpp"

    /// class html_mask_form_filename let user define mask applied to strings (filename, extended attributes, ...)

    /// at the difference of the html_form_* classes which are component to be included into html_form
    /// this class is a full html_form dedicated to the specific case of mask for filename filtering

class html_mask_form_filename : public body_builder,
				public actor,
				public events,
				public html_form_dynamic_table_object_provider,
				public jsoner,
				public html_mask,
				public bibliotheque_subconfig
{
public:
    static const std::string changed;

	/// constructor

	/// \param[in] subject is the name of the type of object we filter like "file name" or "extended attributes"
	/// this string is used to build the summary phrase describing the action the filter will perform
    html_mask_form_filename(const std::string & subject);
    html_mask_form_filename(const html_mask_form_filename & ref) = default;
    html_mask_form_filename(html_mask_form_filename && ref) noexcept = delete;
    html_mask_form_filename & operator = (const html_mask_form_filename & ref) = delete;
    html_mask_form_filename & operator = (html_mask_form_filename && ref) noexcept = delete;
    ~html_mask_form_filename() = default;

	/// change the value provided at construction time
    void change_subject(const std::string & subject);

	/// inherited from html_mask
    virtual std::unique_ptr<libdar::mask> get_mask() const override { return root.get_mask(); };

	/// inherited from html_form_dynamic_table
    virtual std::unique_ptr<body_builder> provide_object_of_type(unsigned int num,
								 const std::string & context) const override;

	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override;

	/// inherited from bibliotheque_subconfig
    virtual bibliotheque::using_set get_using_set() const override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    html_form form;
    html_form_mask_bool root;
    std::shared_ptr< std::deque<std::string> >labels;
    std::shared_ptr<std::string> sujet; ///< shared with provided objects

    void init_bool_obj(html_form_mask_bool & obj) const;
    void update_labels();
    void check_ptr() const;

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* jlabel_bool_config = "bool-config";

};

#endif
