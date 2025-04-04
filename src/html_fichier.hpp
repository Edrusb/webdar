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

#ifndef HTML_FICHIER_HPP
#define HTML_FICHIER_HPP

#include "my_config.h"

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"


    /// class html_fichier is a body_builder object that only shows if the request path is exactly its path

class html_fichier: public body_builder
{
public:
    html_fichier() = default;
    html_fichier(const html_fichier & ref) = default;
    html_fichier(html_fichier && ref) noexcept = default;
    html_fichier & operator = (const html_fichier & ref) = default;
    html_fichier & operator = (html_fichier && ref) noexcept = default;
    ~html_fichier() = default;

	/// define the content to return upon request
    void set_data(const std::string & arg) { data = arg; };

	/// clear content
    void clear() { data.clear(); };

	/// whether some data is present
    bool is_empty() const { return data.empty(); };

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override
    {
	return data;
    };

private:
    std::string data;

};


#endif
