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
#include <new>
#include <dar/tools.hpp>

    // webdar headers
#include "html_form.hpp"
#include "html_table.hpp"
#include "html_page.hpp"
#include "html_url.hpp"
#include "html_text.hpp"
#include "html_form_fieldset.hpp"
#include "tokens.hpp"
#include "html_div.hpp"
#include "html_yes_no_box.hpp"

    //
#include "choose.hpp"

using namespace std;

const string choose::css_class_error_msg = "choose_error_msg";
const string choose::css_class_normal_text = "choose_normal_text";

choose::choose():
    page("Webdar - Choose a session"),
    disco(libdar::tools_printf("WebDar - %s", WEBDAR_VERSION)),
    owner(""),
    confirm_mode(false),
    disconnect_req(false),
    table(6),
    nouvelle("/choose/new", "Create a new session"),
    form("Kill the selected session"),
    confirm("Killing a session"),
    ctable(1),
    confirmed("Confirm destruction of sessions listed above?", false)
{
    html_text tmp;
    css tmpcss, tmpcss2;
    static const string css_class_page = "choose_page";
    static const string css_class_tmp_text = "choose_text";
    static const string css_class_table = "choose_table";
    static const string css_class_table_cells = "choose_table_cells";
    static const string css_class_table_title = "choose_table_title";
    static const string css_class_form = "choose_form";
    static const string css_class_div = "choose_div";

    boxes.clear();

	/// setup of session table page

    page.adopt(&disco);

    tmpcss.clear();
    tmpcss.css_color(COLOR_TEXT);
    tmpcss.css_text_h_align(css::al_center);
    page.define_css_class_in_library(css_class_page, tmpcss);
    page.add_css_class(css_class_page);

    tmp.add_text(3, "Current sessions");
    page.adopt_static_html(tmp.get_body_part());

    tmpcss.clear();
    tmpcss2.clear();
    tmpcss.css_border_width(css::bd_all, css::bd_thin, true);
    tmpcss.css_border_style(css::bd_all, css::bd_solid, true);
    tmpcss.css_border_color(css::bd_all, COLOR_TEXT, true);
    page.define_css_class_in_library(css_class_table_cells, tmpcss);
    table.set_css_class_cells(css_class_table_cells);
    tmpcss2.css_inherit_from(tmpcss);
    tmpcss.css_background_color(COLOR_MENU_BACK_OFF);
    tmpcss.css_color(COLOR_MENU_FRONT_OFF);
    tmpcss.css_font_style_italic();
    page.define_css_class_in_library(css_class_table_title, tmpcss);
    table.set_css_class_first_row(css_class_table_title);
    table.css_border_collapsed(true);
    tmpcss2.css_width("90%", true);
    page.define_css_class_in_library(css_class_table, tmpcss2);
    table.add_css_class(css_class_table);

    tmpcss.clear();
    tmpcss.css_margin_top("1em", true);
    page.define_css_class_in_library(css_class_form, tmpcss);
    form.add_css_class(css_class_form);

    tmpcss.clear();
    tmpcss.css_width("90%", true);
    page.define_css_class_in_library(css_class_div, tmpcss);
    div.add_css_class(css_class_div);

    	/// defining but not using yet the css_class_error_msg for html_page "page"

    tmpcss.clear();
    tmpcss.css_color(RED);
    tmpcss.css_text_h_align(css::al_center);
    tmpcss.css_font_weight_bold();
    page.define_css_class_in_library(css_class_error_msg, tmpcss);

	// setting up genealogy of body_builder objects for object page

    div.adopt(&table);
    form.adopt(&div);
    page.adopt(&form);
    page.adopt(&nouvelle);
    page.set_prefix(chemin("choose"));


	/// confirmation page setup

	// we reuse the css_class_name stored in page's csslib
	// to the confirm page which has its own csslib
	// css_class named definition do differ between html_pages
	// "page" and "confirm"!

    tmpcss.clear();
    tmpcss.css_background_color(COLOR_BACK);
    tmpcss.css_color(COLOR_TEXT, true);
    tmpcss.css_text_h_align(css::al_center);
    confirm.define_css_class_in_library(css_class_page, tmpcss);
    confirm.add_css_class(css_class_page);

    tmpcss.clear();
    tmpcss.css_width("90%", true);
    confirm.define_css_class_in_library(css_class_table, tmpcss);
    ctable.add_css_class(css_class_table);

    tmpcss.clear();
    tmpcss.css_text_h_align(css::al_left, true);
    confirm.define_css_class_in_library(css_class_form, tmpcss);
    confirmed.add_css_class(css_class_form);

	/// defining but not using yet the css_class_error_msg for html_page "confirm"

    tmpcss.clear();
    tmpcss.css_padding("1em");
    tmpcss.css_background_color("red");
    tmpcss.css_color(COLOR_PADFRONT);
    tmpcss.css_font_weight_bold();
    tmpcss.css_border_width(css::bd_all, css::bd_medium);
    tmpcss.css_border_style(css::bd_all, css::bd_solid);
    tmpcss.css_border_color(css::bd_all, COLOR_PADBORD);
    confirm.define_css_class_in_library(css_class_error_msg, tmpcss);

    tmpcss.clear();
    tmpcss.css_padding_left("1em");
    confirm.define_css_class_in_library(css_class_normal_text, tmpcss);


	// setting up genealogy of body_builder objects for confirm object

    ctable.adopt(&confirmed);
    confirm.adopt(&ctable);
    confirm.set_prefix(chemin("choose"));

	// events
    disco.record_actor_on_event(this, html_disconnect::event_disconn);
}

void choose::set_owner(const std::string & user)
{
    if(user.empty())
	throw WEBDAR_BUG;
    else
    {
	owner = user;
	disco.set_username(user);
    }
}

answer choose::give_answer(const request & req)
{
    answer ret;
    string error_msg = "";

	// sanity checks
    if(owner.empty())
	throw WEBDAR_BUG;

	// update the form fields when request is a POST

    if(req.get_method() == "POST")
    {
	if(confirm_mode)
	{
		// updating form fields
	    (void)confirm.get_body_part(req.get_uri().get_path(), req);

	    if(confirmed.get_value()) // kill confirmed
	    {
		try
		{
		    kill_selected_sessions();
		}
		catch(exception_bug & e)
		{
		    throw;
		}
		catch(libthreadar::exception_base & e)
		{
		    e.push_message("Error met while killing a session");
		    error_msg = e.get_message(": ");
		}
		catch(exception_base & e)
		{
		    error_msg = string("Error met while killing a session: ") + e.get_message();
		}
	    }
	    confirm_mode = false;
	}
	else
	{
	    vector<html_form_input *>::iterator itb = boxes.begin();

		// updating form fields
	    (void)page.get_body_part(req.get_uri().get_path(), req);

	    while(itb != boxes.end() && (*itb) != nullptr && (*itb)->get_value() == "")
		++itb;

	    if(itb != boxes.end() && (*itb) == nullptr)
		throw WEBDAR_BUG;

	    if(itb != boxes.end()) // at least one box has been checked
		confirm_mode = true;
	}
    }

	// generate response HTML page

    ret.set_status(STATUS_CODE_OK);
    ret.set_reason("ok");

    if(confirm_mode) // confirmation requested for session kill
    {
	regenerate_confirm_page();
	ret.add_body(confirm.get_body_part(req.get_uri().get_path(), req));
    }
    else // normal table display
    {
	regenerate_table_page();
	if(error_msg != "")
	{
	    html_text tmp;

	    tmp.add_text(1, error_msg);
	    tmp.add_css_class(css_class_error_msg);
	    table.adopt_static_html(tmp.get_body_part());
	}
	if(req.get_uri().get_path() == chemin("choose/new"))
	{
	    if(!session::create_new_session(owner,
					    false, // not espetially an initial session (some other may already exist for that user
					    req,
					    ret)) // the response to return
		throw WEBDAR_BUG;
		// with initial set to false the
		// call should either succeed or
		// throw an exception
	}
	else
	    ret.add_body(page.get_body_part(req.get_uri().get_path(), req));
    }

    return ret;
}

void choose::on_event(const std::string & event_name)
{
    if(event_name == html_disconnect::event_disconn)
	disconnect_req = true;
    else
	throw WEBDAR_BUG;
}

void choose::regenerate_table_page()
{
    html_form_input *check = nullptr;

	// releasing old objects of the table

    release_boxes();
    table.clear();

	// rebuilding the table based on existing sessions

    sess = session::get_summary();

    table.adopt_static_html("Session ID");
    table.adopt_static_html("owner");
    table.adopt_static_html("Locked");
    table.adopt_static_html("Libdar");
    table.adopt_static_html("Closing");
    table.adopt_static_html("Kill Session");

    for(vector<session::session_summary>::iterator it = sess.begin();
	it != sess.end();
	++it)
    {
	if(it->owner == owner)
	{
	    string label = it->session_name;
	    if(label == "")
		label = it->session_ID;
	    table.adopt_static_html(html_url(string("/") + it->session_ID,
					     label).get_body_part());
	}
	else
	    table.adopt_static_html(it->session_ID);
	table.adopt_static_html(it->owner);
	table.adopt_static_html(it->locked ? "locked" : " ");
	table.adopt_static_html(it->libdar_running ? "running" : " ");
	table.adopt_static_html(it->closing ? "closing" : " ");
	check = new (nothrow) html_form_input("", html_form_input::check, "", "10");
	if(check == nullptr)
	    throw exception_memory();
	try
	{
	    boxes.push_back(check);
	}
	catch(...)
	{
		// we assume the check pointed to object
		// could not be passed under the responsibility
		// of the boxes
	    delete check;
	    throw;
	}
	if(it->owner != owner)
	    check->set_enabled(false);
	table.adopt(check);
    }
}

void choose::release_boxes()
{
    for(vector<html_form_input *>::iterator it = boxes.begin();
	it != boxes.end();
	++it)
    {
	if(*it != nullptr)
	{
	    delete (*it);
	    *it = nullptr;
	}
    }
    boxes.clear();
}

void choose::regenerate_confirm_page()
{
    html_text text;
    string tmp;

    ctable.clear();
    text.clear();
    text.add_text(3, "The following sessions are about to be destroyed");
    text.add_css_class(css_class_error_msg);
    ctable.adopt_static_html(text.get_body_part());

    text.clear_css_classes();
    text.add_css_class(css_class_normal_text);

    if(boxes.size() != sess.size())
	throw WEBDAR_BUG;
    for(unsigned int i = 0; i < boxes.size(); ++i)
    {
	if(boxes[i] == nullptr)
	    throw WEBDAR_BUG;
	if(boxes[i]->get_value() != "")
	{
	    text.clear();
	    tmp = sess[i].session_name;
	    if(tmp == "")
		tmp = sess[i].session_ID;
	    text.add_text(0,
			  html_url(chemin(sess[i].session_ID).display(false),
				   tmp).get_body_part());
	    ctable.adopt_static_html(text.get_body_part());
	}
    }

    ctable.adopt(&confirmed);
    confirmed.set_value(false);
}

void choose::kill_selected_sessions() const
{
    if(boxes.size() != sess.size())
	throw WEBDAR_BUG;

    for(unsigned int i = 0;
	i < boxes.size();
	++i)
    {
	if(boxes[i] == nullptr)
	    throw WEBDAR_BUG;
	if(boxes[i]->get_value() != "") // session to be killed
	    (void)session::close_session(sess[i].session_ID);
    }
}


