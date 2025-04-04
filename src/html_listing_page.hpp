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

#ifndef HTML_LISTING_PAGE_HPP
#define HTML_LISTING_PAGE_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "html_page.hpp"
#include "html_dir_tree.hpp"
#include "html_button.hpp"
#include "html_div.hpp"
#include "archive_init_list.hpp"
#include "html_focus.hpp"

    /// html_listing_page is the page that shows when listing an existing archive content

    /// it is used by class user_interface once the archive_init_list object it has
    /// created has completed the dar archive openning at which time it passed this object
    /// using the set_source() method and this html_page (html_listing_page) becomes the
    /// one receiving html request and provides html content thanks to its get_body_part()
    /// inherited method.

class html_listing_page : public html_page, public actor, public events
{
public:
    static const std::string event_close;

    html_listing_page();
    html_listing_page(const html_listing_page & ref) = delete;
    html_listing_page(html_listing_page && ref) noexcept = delete;
    html_listing_page & operator = (const html_listing_page & ref) = delete;
    html_listing_page & operator = (html_listing_page && ref) noexcept = delete;
    ~html_listing_page() = default;


	/// set title of the listing page given the session name
    void set_session_name(const std::string & session_name);

	/// mandatory call before calling get_body_part !!!
    void set_source(const archive_init_list *ref) { tree.set_source(ref); tree.go_expand(); tree.go_show(); };

	/// clear informations about previously read archive
    void clear() { tree.clear(); };

	// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:
	/// inherited from html_div/body_builder
    virtual void new_css_library_available() override;


private:
    html_dir_tree tree;
    html_div title;
    html_focus focus;
    html_button close;
};


#endif
