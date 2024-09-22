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

#ifndef HTML_FORM_OVERWRITE_CHAIN_CELL_HPP
#define HTML_FORM_OVERWRITE_CHAIN_CELL_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_overwrite_action.hpp"
#include "html_form_overwrite_action.hpp"
#include "html_hr.hpp"

    /// html component holding an element in an overwriting policy chain (class html_form_overwrite_chain_action)

    /// the only objective of this class is to add an <hr> element above the cell content
    /// to separate the elements of the chain of actions.

class html_form_overwrite_chain_cell: public html_overwrite_action
{
public:
    html_form_overwrite_chain_cell(std::unique_ptr<html_form_overwrite_action> & insert);
    html_form_overwrite_chain_cell(const html_form_overwrite_chain_cell & ref) = default;
    html_form_overwrite_chain_cell(html_form_overwrite_chain_cell && ref) noexcept = default;
    html_form_overwrite_chain_cell & operator = (const html_form_overwrite_chain_cell & ref) = default;
    html_form_overwrite_chain_cell & operator = (html_form_overwrite_chain_cell && ref) noexcept = default;
    ~html_form_overwrite_chain_cell() = default;

	/// obtain the crit_chain_cell object for libdar option
    virtual std::unique_ptr<libdar::crit_action> get_overwriting_action() const override;


protected:

	/// inherited from body_builder from html_overwrite_action
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    html_hr line;
    std::unique_ptr<html_form_overwrite_action> incell;

};

#endif
