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

#ifndef HTML_SUMMARY_PAGE_HPP
#define HTML_SUMMARY_PAGE_HPP

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

    /// html_summary_page is the page that shows the summary of an existing archive

    /// it is used by class user_interface once the archive_init_list object it has
    /// created has completed the dar archive openning at which time it passed this object
    /// using the set_source() method and this html_page (html_summary_page) becomes the
    /// one receiving html request and provides html content thanks to its get_body_part()
    /// inherited method.

class html_summary_page : public html_page, public actor, public events
{
public:
    static const std::string event_close;

    html_summary_page();
    html_summary_page(const html_summary_page & ref) = delete;
    html_summary_page(html_summary_page && ref) noexcept = delete;
    html_summary_page & operator = (const html_summary_page & ref) = delete;
    html_summary_page & operator = (html_summary_page && ref) noexcept = delete;
    ~html_summary_page() = default;


	/// set title of the summary page given the session name
    void set_session_name(const std::string & session_name);

	/// mandatory call before calling get_body_part !!!
    void set_source(const archive_init_list *ref);

	/// clear informations about previously read archive
    void clear();

	// inherited from actor
    virtual void on_event(const std::string & event_name) override;

protected:
	/// inherited from html_div/body_builder
    virtual void new_css_library_available() override;


private:
    static constexpr const char* css_close = "html_sum_page_close";
    static constexpr const char* css_table_top = "html_sum_page_table_top";
    static constexpr const char* css_table_left = "html_sum_page_table_left";
    static constexpr const char* css_table_rest = "html_sum_page_table_rest";
    static constexpr const char* css_div_float = "html_sum_page_div";
    static constexpr const char* css_about_box = "html_sum_page_about";
    static constexpr const char* css_about_text = "html_sum_page_about_text";

    html_div about;
    html_text about_text;

    html_div format_float;
    html_text format_title;
    html_table format_table;

    html_div slice_float;
    html_text slice_title;
    html_table slice_table;

    html_div overall_float;
    html_text overall_title;
    html_table overall_table;

    html_div content_float;
    html_text content_title;
    html_table content_table;

    html_div saved_float;
    html_text saved_title;
    html_table saved_table;

    html_button close;
};


#endif
