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

#ifndef HTML_FSA_SCOPE_HPP
#define HTML_FSA_SCOPE_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/libdar.hpp>

    // webdar headers
#include "body_builder.hpp"
#include "html_form.hpp"
#include "html_form_fieldset.hpp"
#include "html_form_input.hpp"
#include "jsoner.hpp"
#include "actor.hpp"
#include "events.hpp"

    /// html component used for the user to define a has algorithm

class html_fsa_scope : public body_builder,
		       public jsoner,
		       public actor,
		       public events
{
public:
    static const std::string changed;

    html_fsa_scope();
    html_fsa_scope(const html_fsa_scope & ref) = delete;
    html_fsa_scope(html_fsa_scope && ref) noexcept = delete;
    html_fsa_scope & operator = (const html_fsa_scope & ref) = delete;
    html_fsa_scope & operator = (html_fsa_scope && ref) noexcept = delete;
    ~html_fsa_scope() = default;

	/// obtain the fsa_scope resulting from the web interface
    libdar::fsa_scope get_scope() const;

	/// inherited from jsoner
    virtual void load_json(const json & source) override;

	/// inherited from jsoner
    virtual json save_json() const override;

	/// inherited from jsoner
    virtual void clear_json() override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// inherited from body_builder
    virtual void bind_to_anchor(const std::string & val) override { form.bind_to_anchor(val); };

protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    html_form form;
    html_form_fieldset fs;
    html_form_input hfs_family;
    html_form_input ext_family;

    static constexpr const unsigned int format_version = 1;
    static constexpr const char* myclass_id = "html_fsa_scope";

    static constexpr const char* jlabel_hfs = "hfs";
    static constexpr const char* jlabel_ext = "ext";

};

#endif
