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

#ifndef HTML_LEGEND_HPP
#define HTML_LEGEND_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"

    /// class html_legend implements HTML legend feature

    /// used within html_form_fieldset, mainly

class html_legend : public body_builder
{
public:
    html_legend(const std::string & label) { x_label = label; };
    html_legend(const html_legend & ref) = default;
    html_legend(html_legend && ref) noexcept = default;
    html_legend & operator = (const html_legend & ref) = default;
    html_legend & operator = (html_legend && ref) noexcept = default;
    ~html_legend() = default;

    void change_label(const std::string & label);
    const std::string get_label() const { return x_label; };

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    std::string x_label;

};

#endif
