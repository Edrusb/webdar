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

#ifndef HTML_SLICING_HPP
#define HTML_SLICING_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>
#include <string>

    // webdar headers
#include "body_builder.hpp"
#include "actor.hpp"
#include "events.hpp"
#include "html_form.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_input.hpp"
#include "html_form_input_unit.hpp"
#include "jsoner.hpp"

    /// html component used for the user to define a has algorithm

class html_slicing : public body_builder,
		     public actor,
		     public events,
		     public jsoner
{
public:
    static const std::string changed;

    html_slicing();
    html_slicing(const html_slicing & ref) = delete;
    html_slicing(html_slicing && ref) noexcept = delete;
    html_slicing & operator = (const html_slicing & ref) = delete;
    html_slicing & operator = (html_slicing && ref) noexcept = delete;
    ~html_slicing() = default;

    void set_slicing(const libdar::infinint & x_slice_size,
		     const libdar::infinint & x_first_slice_size);
    void set_permission(const std::string & perm);
    void set_user_ownership(const std::string & username);
    void set_group_ownership(const std::string & groupname);
    void set_min_digits(const libdar::infinint & val);

    void get_slicing(libdar::infinint & x_slice_size,
		     libdar::infinint & x_first_slice_size) const;
    libdar::infinint get_min_digits() const;
    std::string get_permission() const;
    std::string get_user_ownership() const;
    std::string get_group_ownership() const;

	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override;

	/// inherited from class actor
    virtual void on_event(const std::string & event_name) override;

	/// set the object to webdar defaults
    void set_to_webdar_defaults();

	/// inherited from body_builder
    virtual void bind_to_anchor(const std::string & val) override { form_slicing.bind_to_anchor(val); };


protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void css_classes_have_changed() override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    html_form form_slicing;
    html_form_fieldset slicing_fs;
    html_form_input slicing;
    html_form_input_unit slice_size;
    html_form_input different_first_slice;
    html_form_input_unit first_slice_size;
    html_form_input slice_permission;
    html_form_input slice_user_ownership;
    html_form_input slice_group_ownership;
    html_form_input slice_min_digits;

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_slicing";

    static constexpr const char* jlabel_slicing = "slicing"; // bool
    static constexpr const char* jlabel_diff_first = "specific-first-slice"; // bool
    static constexpr const char* jlabel_slice_size = "slice-size"; // infinint
    static constexpr const char* jlabel_first_slice_size = "first-slice-size"; // infinint
    static constexpr const char* jlabel_slice_permission = "permission"; // string
    static constexpr const char* jlabel_user_owner = "user-ownership"; // string
    static constexpr const char* jlabel_group_owner = "group-ownership"; // string
    static constexpr const char* jlabel_min_digits = "min-digits"; // num
};

#endif
