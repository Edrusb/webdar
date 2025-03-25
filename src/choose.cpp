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

const string choose::css_class_normal_text = "choose_normal_text";
const string choose::url_new_session = "/choose/new";

choose::choose():
    page("Webdar - Choose a session"),
    owner(""),
    disconnect_req(false),
    table(6),
    nouvelle(url_new_session, "Create a new session"),
    form("Kill the selected session(s)")
{
    html_text tmp;
    css tmpcss, tmpcss2;
    static const string css_class_page = "choose_page";
    static const string css_class_text = "choose_text";
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
    page.define_css_class_in_library(css_class_page, tmpcss);
    page.add_css_class(css_class_page);

    tmpcss.clear();
    tmpcss.css_text_h_align(css::al_center);
    page.define_css_class_in_library(css_class_text, tmpcss);
    tmp.add_css_class(css_class_text);
    tmp.add_text(3, "Current sessions");
    page.adopt_static_html(tmp.get_body_part());

    tmpcss.clear();
    tmpcss2.clear();
    tmpcss.css_border_width(css::bd_all, css::bd_thin, true);
    tmpcss.css_border_style(css::bd_all, css::bd_solid, true);
    tmpcss.css_border_color(css::bd_all, COLOR_TEXT, true);
    tmpcss.css_text_h_align(css::al_center);
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
    tmpcss.css_text_h_align(css::al_center);
    page.define_css_class_in_library(css_class_form, tmpcss);
    form.add_css_class(css_class_form);

    tmpcss.clear();
    tmpcss.css_text_h_align(css::al_center);
    tmpcss.css_width("90%", true);
    tmpcss.css_padding_bottom("1em");
    page.define_css_class_in_library(css_class_div, tmpcss);
    div.add_css_class(css_class_div);
    box_nouvelle.add_css_class(css_class_div);

	// setting up genealogy of body_builder objects for object page

    div.adopt(&table);
    form.adopt(&div);
    page.adopt(&form);
    box_nouvelle.adopt(&nouvelle);
    page.adopt(&box_nouvelle);
    page.adopt(&confirmed);
    page.set_prefix(chemin("choose"));

	// events
    form.record_actor_on_event(this, html_form::changed);
    disco.record_actor_on_event(this, html_disconnect::event_disconn);
    confirmed.record_actor_on_event(this, html_yes_no_box::answer_yes);
    confirmed.record_actor_on_event(this, html_yes_no_box::answer_no);

    regenerate_table_page();
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

	// sanity checks

    if(owner.empty())
	throw WEBDAR_BUG;

	// update the form fields when request is a POST

    if(req.get_method() == "POST")
	(void)page.get_body_part(req.get_uri().get_path(), req);

	// generate response HTML page

    if(req.get_uri().get_path() == chemin(url_new_session))
    {
	if(!session::create_new_session(owner,
					false, // not espetially an initial session (some other may already exist for that user
					req,
					ret)) // the response to return
	    throw WEBDAR_BUG;
	    // with initial set to false the
	    // call should either succeed or
	    // throw an exception

	regenerate_table_page();
    }
    else
    {
	ret.set_status(STATUS_CODE_OK);
	ret.set_reason("ok");
	regenerate_table_page();
	ret.add_body(page.get_body_part(req.get_uri().get_path(), req));
    }

    return ret;
}

void choose::on_event(const std::string & event_name)
{
    if(event_name == html_form::changed)
    {
	string target_sessions;

	for(unsigned int i = 0;
	    i < boxes.size();
	    ++i)
	{
	    if(boxes[i] == nullptr)
		throw WEBDAR_BUG;

	    if(boxes[i]->get_value_as_bool())
	    {
		if(! target_sessions.empty())
		    target_sessions += ", ";
		target_sessions += sess[i].session_name;
	    }
	}

	if(!target_sessions.empty())
	{
	    if(!confirmed.get_visible())
	    {
		target_sessions = "Confirm destruction of the following sessions: " + target_sessions;
		confirmed.ask_question(target_sessions, false);
	    }
	}
    }
    else if(event_name == html_disconnect::event_disconn)
	disconnect_req = true;
    else if(event_name == html_yes_no_box::answer_yes)
    {
	kill_selected_sessions();
	regenerate_table_page();
    }
    else if(event_name == html_yes_no_box::answer_no)
    {
	regenerate_table_page();
    }
    else
	throw WEBDAR_BUG;
}

void choose::regenerate_table_page()
{
    html_form_input *check = nullptr;
    set<string> selected_ids;

	// recording session which box was checked
    for(unsigned int i = 0; i < boxes.size(); ++i)
    {
	if(boxes[i] != nullptr && boxes[i]->get_value_as_bool())
	    selected_ids.insert(sess[i].session_ID);
    }

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
	check = new (nothrow) html_form_input("", html_form_input::check, "", "", "");
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
	if(selected_ids.find(it->session_ID) != selected_ids.end())
	    check->set_value_as_bool(true); // box was checked for this session ID
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


