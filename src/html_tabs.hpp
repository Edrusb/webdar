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

#ifndef HTML_TABS_HPP
#define HTML_TABS_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <string>
#include <deque>

    // webdar headers
#include "body_builder.hpp"
#include "html_div.hpp"
#include "html_button.hpp"
#include "html_aiguille.hpp"
#include "css.hpp"
#include "html_hr.hpp"

    /// class html_tabs implements tabs

    /// this class implements tabs. Tabs are created
    /// calling add_tab(). Below the tab bar
    /// the rest of the area shows the object adopted for the
    /// selected tab. A new tab must first be created before
    /// being able to adopt a new child.

class html_tabs: public body_builder, public actor
{
public:
    html_tabs();
    html_tabs(const html_tabs & ref) = delete;
    html_tabs(html_tabs && ref) noexcept = delete;
    html_tabs & operator = (const html_tabs & ref) = delete;
    html_tabs & operator = (html_tabs && ref) noexcept = delete;
    virtual ~html_tabs();

	/// add a new tab to the tab bar

	/// \param[in] label shows to the user and is translatable
	/// \param[in] tag is not translatable and does not show to the user
	/// but can be used programmatically (see adopt_in_section() below)
    void add_tab(const std::string & label, const std::string & tag);

	/// inherited from actor class
    virtual void on_event(const std::string & event_name) override;

	/// adopt_in_section replaces body_builder::adopt()
    void adopt_in_section(const std::string & tag, body_builder *obj);

	/// adopt in section replaces body_builder::adopt()
    void adopt_in_section(signed int num, body_builder* obj);


protected:

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

	/// hiding adopt() as it is replaced by adopt_in_section()
    using body_builder::adopt;

private:
    std::map<std::string, unsigned int> corres; ///< link eventname to tab number
    std::deque<html_button*> tabs;

	/// adoption tree
	///
	/** /verbatim

	    this (adopter)
	    |
	    +---- tab_bar
	    |        |
	    |        +--- html_buttons in tabs deque
	    |
	    +---- line
	    |
	    +---- content_wrapper
	            |
		    +---- content
		             |
			     +--- adopted_in_section() objects

	    /endverbatim **/

    html_div tab_bar;
    html_hr line;
    html_div content_wrapper;
    html_aiguille content;
    unsigned int current_mode;

    void set_css_on(html_button & ref);
    void set_css_off(html_button & ref);

    void set_mode(unsigned int mode);

    static const char* tab_shape_on;
    static const char* tab_shape_off;
    static const char* menu_bar;
    static const char* tab_sep;

    static const char* css_content_wrapper;
};

#endif
