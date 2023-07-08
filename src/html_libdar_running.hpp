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

#ifndef HTML_LIBDAR_RUNNING_HPP
#define HTML_LIBDAR_RUNNING_HPP

    // C system header files
extern "C"
{

}

    // C++ system header files

    // webdar headers
#include "html_page.hpp"
#include "events.hpp"
#include "html_div.hpp"
#include "html_web_user_interaction.hpp"
#include "html_statistics.hpp"
#include "html_button.hpp"

class html_libdar_running : public html_page, public events, public actor
{
public:
	// this class generates the following events:
    static const std::string ask_end_libdar;      //< ask_close button has been pressed (graceful ending requested)
    static const std::string force_end_libdar;    //< force_close button has been pressed (immediate ending requested)
    static const std::string kill_libdar_thread;  //< kill_close button has been pressed (kill thread requested)
    static const std::string close_libdar_screen; //< finish button has been pressed


	/// constructor
    html_libdar_running();
    html_libdar_running(const html_libdar_running & ref) = default;
    html_libdar_running(html_libdar_running && ref) noexcept = default;
    html_libdar_running & operator = (const html_libdar_running & ref) = default;
    html_libdar_running & operator = (html_libdar_running && ref) noexcept = default;
    ~html_libdar_running() = default;

	/// clear logs and reset counters
    void clear();

	/// be informed of the end of libdar thread
    void libdar_has_finished() { set_mode(finished); set_title(webdar_tools_get_title(sessname, "Webdar - Libdar has ended")); };

	/// provide objects for libdar execution
    web_user_interaction & get_user_interaction() { return web_ui.get_user_interaction(); };
    html_statistics & get_statistics() { return stats; };

	/// inherited from body_builder
    virtual std::string get_body_part(const chemin & path,
				      const request & req) override;

	/// inherited from actor
    virtual void on_event(const std::string & event_name) override;

	/// defines the name of the session
    void set_session_name(const std::string & name) { sessname = name; };

protected:
	// inherited from body_builder
    virtual void new_css_library_available() override;

private:
    enum mode_type
    {
	normal,        //< should display web_user_interface, progressive_report and ask_close button
	end_asked,     //< should display web_user_interface, progressive_report and force_close button
	end_forced,    //< should display web_user_interface, progressive_report and kill_close button
	kill_forced,   //< should display web_user_interface, progressive_report (no button)
	finished       //< should display web_user_interface, progressive_report, close button
    };

    static const std::string class_global;
    static const std::string class_web;
    static const std::string class_button;

    std::string sessname;
    mode_type mode;
    html_div global;
    html_web_user_interaction web_ui;
    html_statistics stats;
    html_button ask_close;
    html_button force_close;
    html_button kill_close;
    html_button finish;
    bool visibility_has_changed; //< true whether a html component had its visibility changed

    void set_mode(mode_type m);
};


#endif

