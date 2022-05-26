/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2022 Denis Corbin
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

#ifndef HTML_IMAGE_HPP
#define HTML_IMAGE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"
#include "static_body_builder.hpp"


class html_image : public body_builder, public static_body_builder
{
public:
    html_image(const std::string & url, const std::string & alt);
    html_image(const html_image & ref) = default;
    html_image(html_image && ref) noexcept = default;
    html_image & operator = (const html_image & ref) = default;
    html_image & operator = (html_image && ref) noexcept = default;
    ~html_image() = default;

    void set_dimensions(const std::string x, const std::string y) { dim_x = x; dim_y = y; };

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req) override { return get_body_part(); };

	/// inherited from static_body_builder
    virtual std::string get_body_part() const override;

private:
    std::string x_url;
    std::string x_alt;
    std::string dim_x;
    std::string dim_y;
};


#endif
