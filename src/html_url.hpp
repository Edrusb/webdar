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

#ifndef HTML_URL_HPP
#define HTML_URL_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include "body_builder.hpp"
#include "static_body_builder.hpp"

    // webdar headers

    /// html component implementing the usual hyperlink component

class html_url : public body_builder
{
public:
    html_url(const std::string & url);
    html_url(const html_url & ref) = default;
    html_url(html_url && ref) noexcept = delete;
    html_url & operator = (const html_url & ref) = default;
    html_url & operator = (html_url && ref) noexcept = delete;
    ~html_url() = default;

    void change_url(const std::string & newurl);
    const std::string & get_url() const { return x_url; };

	/// inherited from body_builder
    virtual void bind_to_anchor(const std::string & val) override { anchor_to = val; };

	/// whether to download or display the URL target
    void set_download(bool mode) { download = mode; };

	/// change the filename to create on client/browser side if download is set to true
	/// \note providing a empty string get to the default behavior (no filename specified in URL)
    void set_filename(const std::string & name) { filename = name; };

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;


private:
    std::string anchor_to;
    std::string x_url;
    bool download;
    std::string filename;
};


#endif
