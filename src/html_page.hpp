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

#ifndef HTML_PAGE_HPP
#define HTML_PAGE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>
#include <vector>

    // webdar headers
#include "html_level.hpp"
#include "exceptions.hpp"


class html_page : public html_level
{
public:
    html_page(const std::string & title = "") { x_title = title; store_css_library(); };
    html_page(const html_page & ref) = default;
    html_page(html_page && ref) noexcept = default;
    html_page & operator = (const html_page & ref) = default;
    html_page & operator = (html_page && ref) noexcept = default;
    ~html_page() = default;

	/// change page title
    void set_title(const std::string & title) { x_title = title; };

	/// get current title
    const std::string & get_title() const { return x_title; };

	/// activate HTML redirection
	///
	/// \note if url is set to an empty string redirection is disabled
    void set_refresh_redirection(unsigned int seconds, const std::string & url);


	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req) override;

protected:
    std::string get_body_part_given_the_body(const chemin & path,
					     const request & req,
					     const std::string & body);

	/// \note: for inherited classes implementation, as the css_library is created
	/// from the constructor of html_page class, the new_css_library_available()
        /// overriden method will not be called as expected from the consequence of setting
	/// a css_library. To solve this restriction
	/// what seems the best solution is to manually call the
	/// inherited class's new_css_library_available() from the inherited
	/// class constructors.

private:
    std::string x_title;
    std::string redirect;
};


#endif
