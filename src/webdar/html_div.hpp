/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013 Denis Corbin
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

#ifndef HTML_DIV_HPP
#define HTML_DIV_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_level.hpp"

    /// class html_div is the implementation of <div> </div>
    ///
    /// use the body_builder::give() and html_level::give_static_html() to add
    /// data into the <div> </div>. Apply css_*() methods to it

class html_div : public html_level
{
public:

    virtual std::string get_body_part(const chemin & path,
				      const request & req);

};

#endif
