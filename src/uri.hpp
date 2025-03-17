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

#ifndef URI_HPP
#define URI_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <string>
#include <vector>

    // webdar headers
#include "chemin.hpp"

    /// uri type holds the splitted list of the scheme / hostname / path # anchor

class uri
{
public:
	/// constructors
    uri() { clear(); };
    uri(const std::string & res) { read(res); };
    uri(const uri & ref) = default;
    uri(uri && ref) noexcept = default;
    uri & operator = (const uri & ref) = default;
    uri & operator = (uri && ref) noexcept = default;
    ~uri() = default;

	/// the anchor part of the path is not used for comparison
    bool operator == (const uri & arg) const;

	/// convert an uri from a string
    void read(const std::string & res);

	/// clear the uri (empty uri)
    void clear() { path.clear(); hostname = scheme = anchor = ""; };

	/// clear the path part only
    void trim_path() { path.clear(); };

	/// obtain the URI scheme (http, https, ftp, etc.)
    const std::string & get_scheme() const { return scheme; };

	/// obtain the URI host part
    const std::string & get_hostname() const { return hostname; };

	/// retrieve a path of the uri
    const chemin & get_path() const { return path; };

	/// retrieve the anchor previously assigned to this uri (empty string if none)
    const std::string & get_anchor() const { return anchor; };

	/// assign/replace an anchor to this uri (or remove it "" is provided)
    void set_anchor_to(const std::string & val) const { anchor = val; };

	/// rebuid the uri as a single string
    const std::string get_string() const;

	/// add members to the uri
    void add(const std::string & suppath) { path.push_back(suppath); };
    void add(const chemin & suppath) { path += suppath; };

private:
    std::string scheme;
    std::string hostname;
    chemin path;
    mutable std::string anchor;
};

#endif
