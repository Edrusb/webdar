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

    // C system header files
extern "C"
{

}

    // C++ system header files
#include <new>

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

map<string, choose::record *> choose::per_user;

choose::choose(const string & user):
    page("Webdar - Choose a session"),
    table(6),
    nouvelle("/choose/new", "Create a new session"),
    form("Kill the selected session"),
    confirm("Killing a session"),
    ctable(1),
    confirmed("Confirm destruction of sessions listed above?", false)
{
    html_text tmp;
    css tmpcss;

    owner = user;
    confirm_mode = false;
    boxes.clear();

	/// setup of session table page

    page.css_background_color(COLOR_BACK);
    page.css_color(COLOR_TEXT);
    page.css_padding("1em");
    page.css_text_align(css::al_center);

    tmp.add_text(3, string("Current sessions - we are identified as user ") + owner);
    tmp.css_padding("1em");
    tmp.css_background_color(COLOR_PADBACK);
    tmp.css_color(COLOR_PADFRONT);
    tmp.css_font_weight_bold();
    tmp.css_border_width(css::bd_all, css::bd_medium);
    tmp.css_border_style(css::bd_all, css::bd_solid);
    tmp.css_border_color(css::bd_all, COLOR_PADBORD);
    page.adopt_static_html(tmp.get_body_part());

    tmpcss.css_border_width(css::bd_all, css::bd_thin, true);
    tmpcss.css_border_style(css::bd_all, css::bd_solid, true);
    tmpcss.css_border_color(css::bd_all, COLOR_TEXT, true);
    table.css_inherit_from(tmpcss);
    table.set_css_cells(tmpcss);
    tmpcss.css_background_color(COLOR_MENU_BACK_OFF);
    tmpcss.css_color(COLOR_MENU_FRONT_OFF);
    tmpcss.css_font_style_italic();
    table.set_css_cells_first_raw(tmpcss);
    table.css_border_collapsed(true);
    table.css_width("90%", true);

    form.css_margin_top("1em", true);

    div.css_width("90%", true);
    div.adopt(&table);
    form.adopt(&div);
    page.adopt(&form);
    page.adopt(&nouvelle);
    page.set_prefix(chemin("choose"));


	/// confirmation page setup

    confirm.css_background_color(COLOR_BACK);
    confirm.css_color(COLOR_TEXT, true);
    confirm.css_text_align(css::al_center);
    ctable.css_width("90%", true);
    confirmed.css_text_align(css::al_left, true);

    ctable.adopt(&confirmed);
    confirm.adopt(&ctable);
    confirm.set_prefix(chemin("choose"));
}

answer choose::give_answer(const request & req)
{
    answer ret;
    string error_msg = "";

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
	    tmp.css_color("#FF0000");
	    tmp.css_text_align(css::al_center);
	    tmp.css_font_weight_bold();
	    table.adopt_static_html(tmp.get_body_part());
	}
	if(req.get_uri().get_path() == chemin("choose/new"))
	    ret = create_new_session(req);
	else
	    ret.add_body(page.get_body_part(req.get_uri().get_path(), req));
    }

    return ret;
}

answer choose::create_new_session(const request & req)
{
    answer ret;

    html_page page("redirection to newly created session page");
    string session_ID = session::create_new(owner);

    page.set_refresh_redirection(0, session_ID);
    ret.set_status(STATUS_CODE_OK);
    ret.set_reason("new session created");
    ret.add_body(page.get_body_part(chemin("/"), req));

    return ret;
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
	check = new (nothrow) html_form_input("", html_form_input::check, "", 10);
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
    text.css_clear_attributes();
    text.add_text(3, "The following sessions are about to be destroyed");
    text.css_padding("1em");
    text.css_background_color("red");
    text.css_color(COLOR_PADFRONT);
    text.css_font_weight_bold();
    text.css_border_width(css::bd_all, css::bd_medium);
    text.css_border_style(css::bd_all, css::bd_solid);
    text.css_border_color(css::bd_all, COLOR_PADBORD);
    ctable.adopt_static_html(text.get_body_part());

    text.css_clear_attributes();
    text.css_padding_left("1em");

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


answer choose::give_answer_for(const string & user, const request & req)
{
    answer ret;

    map<string, record *>::iterator it = per_user.find(user);

    if(it == per_user.end())
    {
	    // no choose object already exists for that user,
	    // creating one and recording it in per_user table

	record *tmp = new (nothrow) record();
	if(tmp == nullptr)
	    throw exception_memory();

	try
	{
	    tmp->obj = new (nothrow) choose(user);
	    if(tmp->obj == nullptr)
		throw exception_memory();
	    ret = tmp->obj->create_new_session(req);

	    per_user[user] = tmp;
		// tmp is now managed/owned by the per_user map
	}
	catch(...)
	{
	    delete tmp;
	    throw;
	}

	it = per_user.find(user);
	if(it == per_user.end())
	    throw WEBDAR_BUG;
    }
    else // chooser already exists
    {
	if(it->second == nullptr)
	    throw WEBDAR_BUG;

	it->second->lock.lock();
	try
	{
	    if(it->second->obj == nullptr)
		throw WEBDAR_BUG;

	    ret = it->second->obj->give_answer(req);
	}
	catch(...)
	{
	    it->second->lock.unlock();
	    throw;
	}

	it->second->lock.unlock();
    }

    return ret;
}


void choose::cleanup_memory()
{
    map<string, record *>::iterator it = per_user.begin();

    try
    {

	while(it != per_user.end())
	{
	    if(it->second != nullptr)
		delete it->second;
	    ++it;
	}
    }
    catch(...)
    {
	per_user.clear();
	throw;
    }

    per_user.clear();
}

