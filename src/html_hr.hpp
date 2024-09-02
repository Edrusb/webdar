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

#ifndef HTML_HR_HPP
#define HTML_HR_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"

    /// class html_hr is the implementation of <hr /> html component

    /// \note, this object is not expected to have any children

class html_hr : public body_builder
{
public:
    html_hr() = default;
    html_hr(const html_hr & ref) = default;
    html_hr(html_hr && ref) noexcept = default;
    html_hr & operator = (const html_hr & ref) = default;
    html_hr & operator = (html_hr && ref) noexcept = default;
    ~html_hr() = default;

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void has_adopted(body_builder *obj) override { throw WEBDAR_BUG; };
};

#endif
