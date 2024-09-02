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

#ifndef HTML_FORM_OVERWRITE_CHAIN_ACTION_HPP
#define HTML_FORM_OVERWRITE_CHAIN_ACTION_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "actor.hpp"
#include "html_overwrite_action.hpp"
#include "html_form_overwrite_action.hpp"
#include "html_form_fieldset.hpp"
#include "html_table.hpp"
#include "html_button.hpp"
#include "html_form_input.hpp"

    /// html component to be adopted by an html_form that implements a chain of actions for overwriting policies

class html_form_overwrite_chain_action: public html_overwrite_action, public actor
{
public:
    html_form_overwrite_chain_action(const std::string & label);
    html_form_overwrite_chain_action(const html_form_overwrite_chain_action & ref) = default;
    html_form_overwrite_chain_action(html_form_overwrite_chain_action && ref) noexcept = default;
    html_form_overwrite_chain_action & operator = (const html_form_overwrite_chain_action & ref) = default;
    html_form_overwrite_chain_action & operator = (html_form_overwrite_chain_action && ref) noexcept = default;
    ~html_form_overwrite_chain_action() = default;

	/// obtain the crit_chain_action object for libdar option
    virtual std::unique_ptr<libdar::crit_action> get_overwriting_action() const override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:

	/// inherited from body_builder from html_overwrite_action
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;


private:
    static constexpr const char* event_add = "addinchain";

    struct entry
    {
	entry() { action.reset(); del.reset(); };

	std::unique_ptr<html_form_overwrite_action> action;
	std::unique_ptr<html_form_input> del;
    };

    std::list<entry> table_content; ///< owner of the html components adopted by "table"
    unsigned int event_del_count;   ///< used to build the next event name for each new "delete" component
    std::map<std::string, std::list<entry>::iterator> del_event_to_content; ///< find the entry of given event_name
    std::deque<std::string> events_to_delete; ///< list of events pointing to entry to be deleted

    html_form_fieldset fs;
    html_table table;
    html_button adder;

    void del_action(const std::string & event_name);
    void purge_to_delete();
};

#endif
