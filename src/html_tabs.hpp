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
extern "C"
{

}

    // C++ system header files
#include <string>
#include <deque>

    // webdar headers
#include "html_div.hpp"
#include "html_button.hpp"
#include "html_aiguille.hpp"
#include "html_table.hpp"
#include "css.hpp"

    /// class html_tabs implements tabs
    ///
    /// this class implements tabs. Tabs are created
    /// calling add_tab(). Below the tab bar
    /// the rest of the area shows the object adopted for the
    /// selected tab. A new tab must first be created before
    /// being able to adopt a new child.

class html_tabs: public html_table, public actor
{
public:
    html_tabs();
    html_tabs(const html_tabs & ref) = delete;
    html_tabs(html_tabs && ref) noexcept = delete;
    html_tabs & operator = (const html_tabs & ref) = delete;
    html_tabs & operator = (html_tabs && ref) noexcept = delete;
    virtual ~html_tabs();

	/// add a new tab to the tab bar
    void add_tab(const std::string & label);

	/// inherited from actor class
    virtual void on_event(const std::string & event_name) override;

	/// adopt_in_section replaces body_builder::adopt()
    void adopt_in_section(const std::string & tab_label, body_builder *obj);

	/// adopt in section replaces body_builder::adopt()
    void adopt_in_section(signed int num, body_builder* obj);


protected:
	/// inherited from body_builder
    virtual void has_adopted(body_builder *obj) override;

	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void new_css_library_available() override;

	/// hiding adopt() as it is replaced by adopt_in_section()
    using body_builder::adopt;

private:
    html_div tab_bar;
    std::map<std::string, unsigned int> corres; ///< link eventname to tab number
    std::deque<html_button*> tabs;
    html_aiguille content;
    unsigned int current_mode;

    void set_mode(unsigned int mode);

    static const char* tab_on;
    static const char* tab_on_url;
    static const char* tab_off;
    static const char* tab_off_url;
    static const char* menu_bar;
    static const char* tab_sep;
};

#endif
