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

#ifndef HTML_MASK_FORM_PATH_HPP
#define HTML_MASK_FORM_PATH_HPP

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
#include "bibliotheque_subconfig.hpp"
#include "html_form.hpp"
#include "html_form_mask_bool.hpp"


    /// class html_mask_form_path let user define mask on paths

    /// at the difference of the html_form_* classes which are component to be included into html_form
    /// this class is a full html_form dedicated to the specific case of mask for path filtering

class html_mask_form_path : public body_builder,
			    public actor,
			    public events,
			    public html_form_dynamic_table_object_provider,
			    public jsoner,
			    public bibliotheque_subconfig
{
public:
    static const std::string changed;

	/// constructor

	/// \param[in] allow_absolute_paths determines if path masks we will generate
	/// will allow or not absolute paths.
    html_mask_form_path(bool allow_absolute_paths);
    html_mask_form_path(const html_mask_form_path & ref) = default;
    html_mask_form_path(html_mask_form_path && ref) noexcept = delete;
    html_mask_form_path & operator = (const html_mask_form_path & ref) = delete;
    html_mask_form_path & operator = (html_mask_form_path && ref) noexcept = delete;
    ~html_mask_form_path() = default;

    void set_fs_root(const std::string & prefix);

	/// change the value given at construction
    void set_allow_absolute_paths(bool val);

	/// inherited from html_mask
    std::unique_ptr<libdar::mask> get_mask() const { return root.get_mask(); };

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
    std::deque<std::string> labels;
    std::shared_ptr<bool> allow_abs_paths;
	// the use of a shared_ptr is done here because this
	// information will be shared with a possibly wide
	// range of objects we will provide (as object
	// provider) stored in a possibly rich tree-like structure
	// (html_form_mask_bool) and this info may be needed
	// to be updated from time to time.

    void init_bool_obj(html_form_mask_bool & obj) const;
};

#endif
