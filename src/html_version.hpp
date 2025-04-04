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

#ifndef HTML_VERSION_HPP
#define HTML_VERSION_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_popup.hpp"
#include "actor.hpp"
#include "html_button.hpp"
#include "html_table.hpp"
#include "html_div.hpp"

    /// class html_version is a popup displaying version information


class html_version: public html_popup, public actor
{
public:
    html_version();
    html_version(const html_version & ref) = default;
    html_version(html_version && ref) noexcept = default;
    html_version & operator = (const html_version & ref) = default;
    html_version & operator = (html_version && ref) noexcept = default;
    ~html_version() = default;

    	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:

    	/// inherited from body_builder
    virtual void new_css_library_available() override;


private:
    html_text title;
    html_div title_box;
    html_table webdar_table;
    html_table libthreadar_table;
    html_table libdar_table;
    html_button close;

    void fill_libthreadar(html_table & table);
    void fill_libdar(html_table & table);

    static constexpr const char* event_close = "close";

    static constexpr const char* css_close = "html_version_close";
    static constexpr const char* css_table = "html_version_table";
    static constexpr const char* css_table_top = "html_version_top";
    static constexpr const char* css_table_left = "html_version_left";
    static constexpr const char* css_table_cells = "html_version_cells";
};

#endif
