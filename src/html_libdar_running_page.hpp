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

#ifndef HTML_LIBDAR_RUNNING_PAGE_HPP
#define HTML_LIBDAR_RUNNING_PAGE_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_page.hpp"
#include "events.hpp"
#include "actor.hpp"
#include "html_web_user_interaction.hpp"

    /// html_libdar_running_page wraps up an html_web_user_interaction in a html_page

    /// also handle page title and for that need to know the session name

class html_libdar_running_page : public html_page, public events, public actor
{
public:
    	// this class generates the following events:
    static const std::string libdar_has_finished; ///< libdar execution has ended and user has acknoledged it

	// constructor
    html_libdar_running_page();
    html_libdar_running_page(const html_libdar_running_page & ref) = delete;
    html_libdar_running_page(html_libdar_running_page && ref) noexcept = delete;
    html_libdar_running_page & operator = (const html_libdar_running_page & ref) = delete;
    html_libdar_running_page & operator = (html_libdar_running_page && ref) noexcept = delete;
    ~html_libdar_running_page() = default;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// defines the name of the session
    void set_session_name(const std::string & name) { sessname = name; set_title(webdar_tools_get_title(sessname, "Libdar is running")); };

	/// propagate request to web_ui
    std::shared_ptr<html_web_user_interaction> get_html_user_interaction() { return web_ui; };

	/// propagate request to web_ui
    void run_and_control_thread(libthreadar::thread* arg) { web_ui->run_and_control_thread(arg); };

	/// propagate request to web_ui
    void join_controlled_thread() { web_ui->join_controlled_thread(); };

	/// whether a libdar thread is running under "this" management
    bool is_libdar_running() const { return web_ui->is_libdar_running(); };

	/// whether libdar thread has been aborted (to be checked by the caller upon libdar_has_finished event)
    bool has_libdar_been_aborted() const { return web_ui->has_libdar_been_aborted(); };


protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

private:
    std::string sessname;
    std::shared_ptr<html_web_user_interaction> web_ui;
    bool enable_refresh;    ///< whether page refresh should be set
};

#endif
