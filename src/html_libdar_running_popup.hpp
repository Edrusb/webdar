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

#ifndef HTML_LIBDAR_RUNNING_POPUP_HPP
#define HTML_LIBDAR_RUNNING_POPUP_HPP

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <libthreadar/libthreadar.hpp>

    // webdar headers
#include "html_popup.hpp"
#include "events.hpp"
#include "actor.hpp"
#include "html_web_user_interaction.hpp"

    /// html_libdar_running_popup wraps up an html_web_user_interaction in a html_popup component

    /// also handle page title and for that need to know the session name

class html_libdar_running_popup : public html_popup, public events, public actor
{
public:
    	// this class generates the following events:
    static const std::string libdar_has_finished; ///< libdar execution has ended and user has acknoledged it

	// constructor
    html_libdar_running_popup();
    html_libdar_running_popup(const html_libdar_running_popup & ref) = delete;
    html_libdar_running_popup(html_libdar_running_popup && ref) noexcept = delete;
    html_libdar_running_popup & operator = (const html_libdar_running_popup & ref) = delete;
    html_libdar_running_popup & operator = (html_libdar_running_popup && ref) noexcept = delete;
    ~html_libdar_running_popup() = default;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// propagate request to web_ui
    std::shared_ptr<html_web_user_interaction> get_html_user_interaction() { return web_ui; };

	/// propagate request to web_ui
    void run_and_control_thread(libthreadar::thread* arg) { web_ui->run_and_control_thread(arg); };

	/// whether a libdar thread is running under "this" management
    bool is_libdar_running() const { return web_ui->is_libdar_running(); };

	/// whether libdar thread has been aborted (to be checked by the caller upon libdar_has_finished event)
    bool has_libdar_been_aborted() const { return web_ui->has_libdar_been_aborted(); };

protected:
	/// inherited from body_builder
    virtual std::string inherited_get_body_part(const chemin & path,
						const request & req) override;

	/// inherited from body_builder
    virtual void my_visibility_has_changed() override;

private:
    std::shared_ptr<html_web_user_interaction> web_ui;
    bool enable_refresh;    ///< refresh mode status

};

#endif
