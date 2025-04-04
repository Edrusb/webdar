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

    // C system header files
#include "my_config.h"
extern "C"
{

}

    // C++ system header files
#include <dar/tools.hpp>

    // webdar headers
#include "webdar_css_style.hpp"

    //
#include "html_disconnect.hpp"

using namespace std;

const string html_disconnect::event_disconn = "disconnect";
const string html_disconnect::event_version = "version";

const string html_disconnect::css_global = "html_disco_global";
const string html_disconnect::css_title = "html_disco_title";
const string html_disconnect::css_title_vers = "html_disco_title_vers";
const string html_disconnect::css_title_box = "html_disco_title_box";
const string html_disconnect::css_status = "html_disco_status";
const string html_disconnect::css_status_box = "html_disco_status_box";
const string html_disconnect::css_quit_box = "html_disco_quit_box";
const string html_disconnect::css_quit_link = "html_disco_quit_lnk";

bool html_disconnect::default_basic_auth = true;

html_disconnect::html_disconnect():
    logo((chemin(STATIC_PATH_ID) + chemin(STATIC_TITLE_LOGO)).display(false), "Webdar logo"),
    title_vers(libdar::tools_printf("Version %s", WEBDAR_VERSION), event_version),
    quit("Disconnect", event_disconn)
{

	// components configuration

    logo.set_dimensions("100em", "100em");

    title.clear();
    title.add_text(0, "WEBDAR");

	// adoption tree
    title_box.adopt(&logo);
    title_box.adopt(&title);
    title_box.adopt(&title_vers);
    adopt(&title_box);
    adopt(&quit);
    status_box.adopt(&status);
    adopt(&status_box);

	// events
    quit.record_actor_on_event(this, event_disconn);
    title_vers.record_actor_on_event(this, event_version);
    register_name(event_disconn); // to be able to propagate the event

	// visible status
    if(default_basic_auth)
	quit.set_visible(false);

	// css
    add_css_class(css_global);
    logo.add_css_class(webdar_css_style::float_left);
    title.add_css_class(css_title);
    title.add_css_class(webdar_css_style::text_shadow_dark);
    webdar_css_style::normal_button(title_vers);
    title_vers.add_css_class(css_title_vers);
    title_box.add_css_class(css_title_box);
    status.add_css_class(css_status);
    status.add_css_class(webdar_css_style::text_shadow_dark);
    status_box.add_css_class(css_status_box);
    quit.add_css_class(css_quit_box);
    quit.url_add_css_class(css_quit_link);
}

void html_disconnect::set_username(const std::string & username)
{
    status.clear();
    if(!username.empty())
	status.add_text(0, libdar::tools_printf("Connected as user %s", username.c_str()));
}

void html_disconnect::on_event(const string & event_name)
{
    if(event_name == event_disconn)
	act(event_disconn); // propagate the event
    else if(event_name == event_version)
    {
	    // fire the version popup
    }
    else
	throw WEBDAR_BUG;
}

void html_disconnect::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);

    if(!csslib->class_exists(css_global))
    {
	css tmp;
	css_class btn_class(css_quit_link);
	string small_text = "0.8em";

	    // top bar (the html_disconnect (html_div) itself)
	tmp.clear();

	tmp.css_background_color(COLOR_BACK);
	tmp.css_overflow(css::ov_hidden);
	tmp.css_width("100%", true);
	tmp.css_background_color(COLOR_TOPBAR_BACK);

	csslib->add(css_global, tmp);

	    // for the html_text title
	tmp.clear();

	tmp.css_font_weight_bold();
	tmp.css_font_size("2em");
	tmp.css_margin_left("1em");
	tmp.css_color(COLOR_MENU_BORDER_OFF);

	csslib->add(css_title, tmp);

	    // for the html_text title versions
	tmp.clear();

	tmp.css_font_size(small_text);
	tmp.css_float(css::fl_left);
	tmp.css_margin_left("1em");

	csslib->add(css_title_vers, tmp);


	    // the transparent title_box
	tmp.clear();

	tmp.css_float(css::fl_left);
	tmp.css_width("60%", false);

	csslib->add(css_title_box, tmp);


	    // for the html_text status
	tmp.clear();

	tmp.css_font_size(small_text);
	tmp.css_font_weight_bold();
	tmp.css_font_style_italic();
	tmp.css_margin("0.2em");
	tmp.css_text_v_align(css::al_middle);
	tmp.css_margin_right("1em");
	tmp.css_color(WHITE);

	csslib->add(css_status, tmp);

	    // the transparent status_box
	tmp.clear();

	tmp.css_margin("0.2em");
	tmp.css_float(css::fl_right);

	csslib->add(css_status_box, tmp);


	    // the html_button (disconnect): for the englobing box

	tmp.clear();

	tmp.css_font_size(small_text);
	tmp.css_background_color(COLOR_MENU_BACK_OFF);
	tmp.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF);
	tmp.css_border_style(css::bd_all, css::bd_solid);
	tmp.css_border_width(css::bd_all, css::bd_medium);
	tmp.css_padding_right("0.5em");
	tmp.css_padding_left("0.5em");
	tmp.css_margin("0.2em");
	tmp.css_float(css::fl_right);

	csslib->add(css_quit_box, tmp);

	    // the html_button (disconnect): for the url inside the box
	btn_class.clear_value();
	tmp.clear();

	tmp.css_color(COLOR_MENU_FRONT_OFF);
	tmp.css_font_weight_bold();
	tmp.css_font_style_italic();
	tmp.css_text_decoration(css::dc_none);
	tmp.css_text_h_align(css::al_center);
	tmp.css_text_v_align(css::al_middle);

	btn_class.set_selector(css_class::link, tmp);
	btn_class.set_selector(css_class::visited, tmp);
	csslib->add(btn_class);
    }
}
