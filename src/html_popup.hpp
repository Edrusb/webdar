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

#ifndef HTML_POPUP_HPP
#define HTML_POPUP_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"
#include "html_div.hpp"

    /// class html_popup is the implementation of <div> </div> as popup over rest of the the screen

    /// the object shows a full screen with partial opacity (outerbox) on top of which a fully opaque box (innerbox)
    /// sits. Object adopted by the html_popup are passed to this inner box and take place inside it
    /// There is no mechanism (button) to remove this box from the screen. This is duty of the class using
    /// or inheriting from that class to provide such mechanism (making this object invisible() or
    /// foresaken from the body_building tree for example).

class html_popup : public html_div
{
public:
    html_popup(unsigned int width_pct, unsigned int height_pct);
    html_popup(const html_popup & ref) = delete;
    html_popup(html_popup && ref) noexcept = delete;
    html_popup & operator = (const html_popup & ref) = delete;
    html_popup & operator = (html_popup && ref) noexcept = delete;
    ~html_popup() = default;

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    static const std::string outerclassname; ///< class name is global to the class
    std::string innerclassname;              ///< class name depends on the width and height given at construction time

    unsigned int width;         ///< width in percent of the inner div (innerbox)
    unsigned int height;        ///< height in percent of the inner div (innerbox)
    bool self_css;             ///< true during initial setup of css class (do not react on css_class_have_chaged()), false otherwise

	/// the outerbox  hides (or partially transparent) and interecpt interaction from the user to
	/// UI behind the popup. The innerbox shows on top of the outerbox and contains all the components
	/// the html_popup has adopted.
	/// the innerbox is us (= this)
    html_div outerbox;

};

#endif
