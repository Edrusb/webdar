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

#ifndef HTML_FOCUS_HPP
#define HTML_FOCUS_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <string>

    // webdar headers
#include "body_builder.hpp"

    /// html_focus class provides a mean to temporarily have an object show at a particular place in a page
    ///
    /// the html_focus is adopted as any body_builder object but does not show anything
    /// until a body_builder object is given_for_temporary_adoption(). Any previous object
    /// given that way are foresaken and replaced by the new one.
    /// \note that css properties assigned to a html_focus get passed assigned to the under focus element,
    /// if that element had already got css properties assigned, they are overwritten by the one set to html_focus
    /// the css properties assigned to html_focus would not have any use else.

class html_focus: public body_builder
{
public:
    html_focus() { adopted = nullptr; };
    html_focus(const html_focus & ref) = default;
    html_focus(html_focus && ref) noexcept = default;
    html_focus & operator = (const html_focus & ref) = default;
    html_focus & operator = (html_focus && ref) noexcept = default;
    ~html_focus() = default;

	/// define what to display under the focus
	///
	/// \param[in] obj is the object to be displayed
	/// \note object given stay under responsbility of the caller for memory management
    void given_for_temporary_adoption(body_builder *obj);

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req) override;

protected:
    virtual void has_been_foresaken(body_builder *obj) override { if(obj == adopted) adopted = nullptr; };

private:
    body_builder *adopted; //< the object thas has been temporarily adopted
};





#endif
