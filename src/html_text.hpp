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

#ifndef HTML_TEXT_HPP
#define HTML_TEXT_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "body_builder.hpp"
#include "static_body_builder.hpp"

    /// class html_text manage text and header in html document

    /// \note this class allow user to add by step, at each addition
    /// the level tells whether the given text to add is normal text (level 0)
    /// or header text (1 for h1, 2 for h2, and so on)
    /// some additional methods add new paragraph (<p/> markup)
    /// or newline (<br />)
    /// header markup (<h*>) receive the css_class assigned to this object

class html_text : public body_builder, public static_body_builder
{
public:
    html_text() { txt = ""; };
    html_text(unsigned int level, const std::string & text) { add_text(level, text); };
    html_text(const html_text & ref) = default;
    html_text(html_text && ref) noexcept = default;
    html_text & operator = (const html_text & ref) = default;
    html_text & operator = (html_text && ref) noexcept = default;
    ~html_text() = default;

	/// adding text
	///
	/// \param[in] level set to 1 for <h1>text</h1> etc. and zero for normal text
	/// \param[in] text is the text to add under the provided level
    void add_text(unsigned int level, const std::string & text);
    html_text & add_nl() { txt += "<br />"; my_body_part_has_changed(); return *this; };
    html_text & add_paragraph() { txt += "<p />"; my_body_part_has_changed(); return *this; };

    void clear() { txt = ""; my_body_part_has_changed(); };

	/// inherited from static_body_builder
    virtual std::string get_body_part() const override;

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    std::string txt;

};


#endif
