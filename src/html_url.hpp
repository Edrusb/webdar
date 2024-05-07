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

#ifndef HTML_URL_HPP
#define HTML_URL_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include "body_builder.hpp"
#include "static_body_builder.hpp"

    // webdar headers

class html_url : public body_builder, public static_body_builder
{
public:
    html_url(const std::string & url, const std::string & label) { x_url = url; x_label = label; };
    html_url(const html_url & ref) = default;
    html_url(html_url && ref) noexcept = default;
    html_url & operator = (const html_url & ref) = default;
    html_url & operator = (html_url && ref) noexcept = default;
    ~html_url() = default;

    void change_url(const std::string & newurl);
    void change_label(const std::string & newlabel);

    const std::string & get_label() const { return x_label; };
    const std::string & get_url() const { return x_url; };

	/// inherited from static_body_builder
    virtual std::string get_body_part() const override;

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override { return get_body_part(); };


private:
    std::string x_url;
    std::string x_label;
};


#endif
