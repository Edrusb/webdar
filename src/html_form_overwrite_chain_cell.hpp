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

#ifndef HTML_FORM_OVERWRITE_CHAIN_CELL_HPP
#define HTML_FORM_OVERWRITE_CHAIN_CELL_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_overwrite_action.hpp"
#include "html_form_overwrite_action.hpp"
#include "html_hr.hpp"
#include "jsoner.hpp"
#include "events.hpp"
#include "actor.hpp"

    /// html component holding an element in an overwriting policy chain (class html_form_overwrite_chain_action)

    /// the only objective of this class is to add an <hr> element above the cell content
    /// to separate the elements of the chain of actions.

class html_form_overwrite_chain_cell: public html_overwrite_action,
				      public jsoner,
				      public events,
				      public actor
{
public:
    static const std::string changed;

    html_form_overwrite_chain_cell(std::unique_ptr<html_form_overwrite_action> & insert);
    html_form_overwrite_chain_cell(const html_form_overwrite_chain_cell & ref) = delete;
    html_form_overwrite_chain_cell(html_form_overwrite_chain_cell && ref) noexcept = delete;
    html_form_overwrite_chain_cell & operator = (const html_form_overwrite_chain_cell & ref) = delete;
    html_form_overwrite_chain_cell & operator = (html_form_overwrite_chain_cell && ref) noexcept = delete;
    ~html_form_overwrite_chain_cell() = default;

    html_form_overwrite_action & get_cell_action();

	/// obtain the crit_chain_cell object for libdar option
    virtual std::unique_ptr<libdar::crit_action> get_overwriting_action() const override;

	/// inherited from jsoner
    virtual void load_json(const json & source) override { incell->load_json(source); };

	/// inherited from jsoner
    virtual json save_json() const override { return incell->save_json(); };

	/// inherited from jsoner
    virtual void clear_json() override { incell->clear_json(); };

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

private:
    html_hr line;
    std::unique_ptr<html_form_overwrite_action> incell;

};

#endif
