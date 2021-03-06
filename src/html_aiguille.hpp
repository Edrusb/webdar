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

#ifndef HTML_AIGUILLE_HPP
#define HTML_AIGUILLE_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"

    /// class html_aiguille behave as only one of its adopted children

    /// the set_mode() method which child is used (or is active)
    /// the argument is the number of the child starting at zero in the
    /// they have been adopted

class html_aiguille : public body_builder
{
public:
    html_aiguille() { mode = 0; };
    html_aiguille(const html_aiguille & ref) = default;
    html_aiguille(html_aiguille && ref) noexcept = default;
    html_aiguille & operator = (const html_aiguille & ref) = default;
    html_aiguille & operator = (html_aiguille && ref) noexcept = default;
    ~html_aiguille() = default;

    void set_mode(unsigned int m);

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req) override;

protected:

	/// inherited from css grand-parent
    virtual void css_updated(bool inherited) override;

    	/// inherited from body_builder
    virtual void has_been_adopted(body_builder *obj) override;


private:
    unsigned int mode;

};


#endif
