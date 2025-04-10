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

#ifndef HTML_STATIC_URL_HPP
#define HTML_STATIC_URL_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include "static_body_builder.hpp"
#include "html_url.hpp"
#include "html_text.hpp"
#include "central_report.hpp"

    // webdar headers

    /// html component implementing the usual hyperlink component

class html_static_url : public html_url, public static_body_builder
{
public:
    html_static_url(const std::string & url, const std::string & label);
    html_static_url(const html_static_url & ref) = default;
    html_static_url(html_static_url && ref) noexcept = delete;
    html_static_url & operator = (const html_static_url & ref) = default;
    html_static_url & operator = (html_static_url && ref) noexcept = delete;
    ~html_static_url() = default;

    void change_label(const std::string & newlabel);
    const std::string & get_label() const { return x_label; };

	/// inherited from static_body_builder
    virtual std::string get_body_part() const override;

protected:
    using body_builder::adopt; // forbidding object to adopt other body_builder objects

private:
    std::string x_label;
    html_text child;
    std::shared_ptr<central_report_stdout> faked;
};

#endif
