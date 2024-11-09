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


    // webdar headers
#include "exceptions.hpp"
#include "webdar_css_style.hpp"
#include "tokens.hpp"

    //
#include "saisie.hpp"
#include "html_text.hpp"

using namespace std;

const string saisie::event_closing = "saisie_closing";
const string saisie::event_restore = "saisie_restore";
const string saisie::event_compare = "saisie_compare";
const string saisie::event_test    = "saisie_test";
const string saisie::event_list    = "saisie_list";
const string saisie::event_create  = "saisie_create";
const string saisie::event_isolate = "saisie_isolate";
const string saisie::event_merge   = "saisie_merge";
const string saisie::event_repair  = "saisie_repair";
const string saisie::changed_session_name = "saisie_changed_session_name";

const string saisie::css_class_text = "saisie_text";

const string saisie::menu_main = "main";
const string saisie::menu_restore = "restore";
const string saisie::menu_compare = "compare";
const string saisie::menu_test = "test";
const string saisie::menu_list = "list";
const string saisie::menu_create = "create";
const string saisie::menu_isolate = "isolate";
const string saisie::menu_merge = "merge";
const string saisie::menu_repair = "repair";
const string saisie::menu_biblio = "bibliotheque";
const string saisie::menu_sessions = "sessions";
const string saisie::menu_close = "close";

const string saisie::css_class_right = "saisie_right";
const string saisie::css_class_margin = "saisie_marge";
const string saisie::css_class_choice = "saisie_choice";
const string saisie::css_class_license = "saisie_license";
const string saisie::css_class_rightpan = "saisie_rightpan";


saisie::saisie():
    archread(""),
    licensing((chemin(STATIC_PATH_ID) + chemin(STATIC_OBJ_LICENSING)).display(false), "Webdar is released under the GNU Public License v3"),
    session_name("Session name",
		 html_form_input::text,
		 "",
		 "20"),
    about_fs(""),
    about_form("Change"),
    go_extract("Restore", event_restore),
    go_compare("Compare", event_compare),
    go_test("Test", event_test),
    go_list("List", event_list),
    go_create("Create", event_create),
    go_isolate("Isolate", event_isolate),
    go_merge("Merge", event_merge),
    go_repair("Repair", event_repair),
    close("Do you really want to close this session?", false)
{
    status = st_idle;
    html_text text; // used to build static text content

	// configuration of "choice"
    choice.add_entry("Main Page", menu_main);
    select.add_section(menu_main, "");
    choice.add_entry("", "");
    select.add_section("sep1", "");
    choice.add_entry("Creation", menu_create);
    select.add_section(menu_create, "");
    choice.add_entry("Testing", menu_test);
    select.add_section(menu_test, "");
    choice.add_entry("Comparison", menu_compare);
    select.add_section(menu_compare, "");
    choice.add_entry("Listing", menu_list);
    select.add_section(menu_list, "");
    choice.add_entry("Isolation", menu_isolate);
    select.add_section(menu_isolate, "");
    choice.add_entry("Merging", menu_merge);
    select.add_section(menu_merge, "");
    choice.add_entry("Repairing", menu_repair);
    select.add_section(menu_repair, "");
    choice.add_entry("Restoration", menu_restore);
    select.add_section(menu_restore, "");
    choice.add_entry("", "");
    select.add_section("sep2", "");
    choice.add_entry("Configuration", menu_biblio);
    select.add_section(menu_biblio, "");
    choice.add_entry("", "");
    select.add_section("sep3", "");
    choice.add_entry("Other Sessions", menu_sessions);
    select.add_section(menu_sessions, "");
    choice.add_entry("", "");
    select.add_section("sep4", "");
    choice.add_entry("Close Session", menu_close);
    select.add_section(menu_close, "");
    adopt(&choice);

	// Configuring archive_show

    static const char* sect_archshow = "show";

    archive_show.add_section(sect_archshow, "Backup to Read");
    archive_show.set_active_section(0);

    archive_show.adopt_in_section(sect_archshow, &archread);
    right_pan.adopt(&archive_show);

	// configuration of the sub-pages brought by "select"

	//  configuration of the about sub-page
    text.clear();
    text.add_text(1,"WEBDAR");
    text.add_paragraph();
    text.add_text(4,string("version ") + WEBDAR_VERSION);
    text.add_paragraph();
    text.add_text(0, "by Denis CORBIN");
    text.add_nl();
    text.add_nl();
    text.add_text(0, "to my daughters");
    text.add_nl();
    text.add_text(0, "Capucine and C&eacute;lestine");
    text.add_nl();
    text.add_paragraph();
    text.add_css_class(css_class_text);
    around_licensing.adopt_static_html(text.get_body_part());
    around_licensing.adopt(&licensing);
    around_licensing.adopt_static_html(html_text(0,"").add_paragraph().get_body_part());
    select.adopt_in_section(menu_main, &around_licensing);
    about_fs.adopt(&session_name);
    about_form.adopt(&about_fs);
    select.adopt_in_section(menu_main, &about_form);
    select.adopt_in_section(menu_main, &demo);

	// configuration of the restore sub-page
    select.adopt_in_section(menu_restore, &extract);
    select.adopt_in_section(menu_restore, &go_extract);

	// comparison sub-page
    select.adopt_in_section(menu_compare, &compare);
    select.adopt_in_section(menu_compare, &go_compare);

	// testing sub-page
    static const char* sect_test_params = "tparams";
    test_params.add_section(sect_test_params, "Testing parameters");
    test_params.set_active_section(sect_test_params);

    test_params.adopt_in_section(sect_test_params, &test);
    select.adopt_in_section(menu_test, &test_params);
    select.adopt_in_section(menu_test, &go_test);

	// listing sub-page
    select.adopt_in_section(menu_list, &go_list);

	// creation sub-page
    select.adopt_in_section(menu_create, &create);
    select.adopt_in_section(menu_create, &go_create);

	// isolation sub-page
    select.adopt_in_section(menu_isolate, &isolate);
    select.adopt_in_section(menu_isolate, &go_isolate);

	// merging sub-page
    select.adopt_in_section(menu_merge, &merge);
    select.adopt_in_section(menu_merge, &go_merge);

	//repair sub-page
    select.adopt_in_section(menu_repair, &repair);
    select.adopt_in_section(menu_repair, &go_repair);

	// configuration sub-page
    select.adopt_in_section(menu_biblio, &biblio);

	// other sessions sub_page
	// nothing to add "saisie" is not involved in that context

    	// close session sub_page
    select.adopt_in_section(menu_close, &close);

	/// configuration of "select"
    select.set_active_section(menu_main);
    right_pan.adopt(&select);
    adopt(&right_pan);

	// define the closing event for this
    register_name(event_closing);

	// attaching the "changed" event of the menu "choice" to "this" saisie object
    choice.record_actor_on_event(this);
    on_event(""); // manually triggering the event for the initial setup

    go_extract.record_actor_on_event(this, event_restore);
    go_compare.record_actor_on_event(this, event_compare);
    go_test.record_actor_on_event(this, event_test);
    go_list.record_actor_on_event(this, event_list);
    go_create.record_actor_on_event(this, event_create);
    go_isolate.record_actor_on_event(this, event_isolate);
    go_merge.record_actor_on_event(this, event_merge);
    go_repair.record_actor_on_event(this, event_repair);

    session_name.set_change_event_name(changed_session_name); // using the same event name as the we one we will trigger upon session name change
    session_name.record_actor_on_event(this, changed_session_name);

	// other event to register
    register_name(event_restore);
    register_name(event_compare);
    register_name(event_test);
    register_name(event_list);
    register_name(event_create);
    register_name(event_isolate);
    register_name(event_merge);
    register_name(event_repair);
    register_name(changed_session_name);

	// css
    webdar_css_style::normal_button(archive_show, true);
    webdar_css_style::normal_button(go_extract);
    webdar_css_style::normal_button(go_compare);
    webdar_css_style::normal_button(go_test);
    webdar_css_style::normal_button(go_list);
    webdar_css_style::normal_button(go_create);
    webdar_css_style::normal_button(go_isolate);
    webdar_css_style::normal_button(go_merge);
    webdar_css_style::normal_button(go_repair);
    go_extract.add_css_class(css_class_right);
    go_compare.add_css_class(css_class_right);
    go_test.add_css_class(css_class_right);
    go_list.add_css_class(css_class_right);
    go_create.add_css_class(css_class_right);
    go_isolate.add_css_class(css_class_right);
    go_merge.add_css_class(css_class_right);
    go_repair.add_css_class(css_class_right);
    webdar_css_style::normal_button(test_params, true);
    choice.add_css_class(css_class_choice);
    around_licensing.add_css_class(css_class_license);
    select.add_css_class(css_class_margin);
}

string saisie::inherited_get_body_part(const chemin & path,
				       const request & req)
{
    chemin sub_path = path;
    string ret;

    if(!sub_path.empty())
	sub_path.pop_front();

    status = st_idle;
    ignore_body_changed_from_my_children(false);

	// now we can generate in return the whole HTML code for "this" object
    if(choice.get_current_tag() == menu_close && close.get_value())
    {
	act(event_closing);
	set_title(webdar_tools_get_title(get_session_name(), "Session closed"));
	set_refresh_redirection(0, "/");
	ret = html_page::inherited_get_body_part(path, req);
    }
    else
    {
	if(choice.get_current_tag() == menu_sessions)
	{
	    ignore_body_changed_from_my_children(true);
	    try
	    {
		set_title(webdar_tools_get_title(get_session_name(), "Redirection to all sessions page"));
		set_refresh_redirection(0, "/");     /// we redirect to the root path -> a chooser object answers to this URL
		ret = html_page::inherited_get_body_part(path, req);
		choice.set_current_mode(choice.get_previous_mode());
		    // this is needed, else, when getting back to this session
		    // we would be redirected again to the session listing
		    // page.
		set_refresh_redirection(0, "");
		    // we also disable refresh for the next time we
		    // get to this page to not redirect to the chooser.
		    // Note: the change of refresh mode does not lead
		    // to body change, as implemented in html_page.
	    }
	    catch(...)
	    {
		ignore_body_changed_from_my_children(false);
		throw;
	    }
	    ignore_body_changed_from_my_children(false);
	}
	else
	    ret = html_page::inherited_get_body_part(path, req);
    }

    if(status != st_idle)
	ignore_body_changed_from_my_children(true);
	// avoiding re-evaluation which is not need if status != st_idle
	// as an action has just been fired from us and re-evaluation
	// would reset the status to st_idle and would lose the nature
	// of the action we just fired.

    return ret;
}

void saisie::on_event(const string & event_name)
{
    if(event_name == html_menu::changed
       || event_name == html_form_input::changed
       || event_name == "")
    {
	// menu "choice" changed

	set_title(webdar_tools_get_title(get_session_name(), choice.get_current_label()));
	if(choice.get_current_tag() == menu_restore
	   || choice.get_current_tag() == menu_compare
	   || choice.get_current_tag() == menu_test
	   || choice.get_current_tag() == menu_list
	   || choice.get_current_tag() == menu_isolate
	   || choice.get_current_tag() == menu_merge
	   || choice.get_current_tag() == menu_repair)
	    archive_show.set_visible(true);
	else
	    archive_show.set_visible(false);
	select.set_active_section(choice.get_current_mode());

	    // not necessary to call my_body_part_has_changed()
	    // as "choice" is adopted and will trigger my_body_part_has_changed()
    }
    else if(event_name == event_restore
	    || event_name == event_compare
	    || event_name == event_test
	    || event_name == event_list
	    || event_name == event_create
	    || event_name == event_isolate
	    || event_name == event_merge
	    || event_name == event_repair)
    {
	if(event_name == event_restore)
	    status = st_restore;
	else if(event_name == event_compare)
	    status = st_compare;
	else if(event_name == event_test)
	    status = st_test;
	else if(event_name == event_list)
	    status = st_list;
	else if(event_name == event_create)
	    status = st_create;
	else if(event_name == event_isolate)
	    status = st_isolate;
	else if(event_name == event_merge)
	    status = st_merge;
	else if(event_name == event_repair)
	    status = st_repair;
	else
	    throw WEBDAR_BUG;
	act(event_name); // propagate the event to the subscribers

	// must not call my_body_part_has_changed()!
	// this would trigger the inherited_get_body_part()
	// which would reset the status to st_idle
	// and the user_interface thread (see go_test() for example)
	// would not be able to gather information from this saisie
	// object
	// In other words, the fact the status changed
	// does not change at all the output returned by
	// inherited_get_body_part() thus my_body_part_has_changed()
	// has not to be invoked.
    }
    else if(event_name == changed_session_name)
    {
	set_title(webdar_tools_get_title(get_session_name(), choice.get_current_label()));
	act(changed_session_name);
	// propagating the event

	my_body_part_has_changed();
    }
    else
	throw WEBDAR_BUG;
}

string saisie::get_archive_path() const
{
    switch(status)
    {
    case st_idle:
	throw WEBDAR_BUG;
    case st_restore:
    case st_compare:
    case st_test:
    case st_list:
    case st_isolate:
    case st_repair:
    case st_merge:
	return archread.get_archive_path();
    case st_create:
	return create.get_archive_path();
    default:
	throw WEBDAR_BUG;
    }
}

string saisie::get_archive_basename() const
{
    switch(status)
    {
    case st_idle:
	throw WEBDAR_BUG;
    case st_restore:
    case st_compare:
    case st_test:
    case st_list:
    case st_isolate:
    case st_repair:
    case st_merge:
	return archread.get_archive_basename();
    case st_create:
	return create.get_archive_basename();
    default:
	throw WEBDAR_BUG;
    }
}

libdar::archive_options_read saisie::get_read_options(shared_ptr<html_web_user_interaction> dialog) const
{
    if(status != st_restore
       && status != st_compare
       && status != st_test
       && status != st_list
       && status != st_isolate
       && status != st_merge
       && status != st_repair)
	throw WEBDAR_BUG;

    return archread.get_read_options(dialog);
}

const string & saisie::get_fs_root() const
{
    switch(status)
    {
    case st_idle:
	throw WEBDAR_BUG;
    case st_restore:
	return extract.get_fs_root();
    case st_compare:
	return compare.get_fs_root();
    case st_test:
	throw WEBDAR_BUG;
    case st_list:
	throw WEBDAR_BUG;
    case st_create:
	return create.get_fs_root();
    case st_isolate:
	throw WEBDAR_BUG;
    case st_merge:
	throw WEBDAR_BUG;
    case st_repair:
	throw WEBDAR_BUG;
    default:
	throw WEBDAR_BUG;
    }
}

const libdar::archive_options_extract saisie::get_extraction_options() const
{
    if(status != st_restore)
	throw WEBDAR_BUG;

    return extract.get_options();
}

const libdar::archive_options_diff saisie::get_comparison_options() const
{
    if(status != st_compare)
	throw WEBDAR_BUG;

    return compare.get_options();
}

const libdar::archive_options_test saisie::get_testing_options() const
{
    if(status != st_test)
	throw WEBDAR_BUG;

    return test.get_options();
}

libdar::archive_options_create saisie::get_creating_options(shared_ptr<html_web_user_interaction> dialog) const
{
    if(status != st_create)
	throw WEBDAR_BUG;

    return create.get_options_create(dialog);
}

libdar::archive_options_isolate saisie::get_isolating_options(shared_ptr<html_web_user_interaction> dialog) const
{
    if(status != st_isolate)
	throw WEBDAR_BUG;

    return isolate.get_options_isolate(dialog);
}

libdar::archive_options_merge saisie::get_merging_options(shared_ptr<html_web_user_interaction> dialog) const
{
    if(status != st_merge)
	throw WEBDAR_BUG;

    return merge.get_options_merge(dialog);
}

libdar::archive_options_repair saisie::get_repairing_options(shared_ptr<html_web_user_interaction> dialog) const
{
    return repair.get_options(dialog);
}

void saisie::new_css_library_available()
{
    css tmp;
    string css_class_right = "saisie_right";
    string css_class_margin = "saisie_marge";
    string css_class_choice = "saisie_choice";
    string css_class_license = "saisie_license";
    string css_class_rightpan = "saisie_rightpan";
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    tmp.clear();
    tmp.css_margin_left("9.4em");
    csslib->add(css_class_rightpan, tmp);
    right_pan.add_css_class(css_class_rightpan);

    tmp.clear();
    tmp.css_float(css::fl_right);
    csslib->add(css_class_right, tmp);

    tmp.clear();
    tmp.css_float(css::fl_left);
    csslib->add(css_class_choice, tmp);

    tmp.clear();
    tmp.css_width("90%", true);
    tmp.css_margin_bottom("1em");
    tmp.css_margin_top("1em");
    tmp.css_text_h_align(css::al_center);
    csslib->add(css_class_license, tmp);

    tmp.clear();
    tmp.css_text_h_align(css::al_center, true);
    tmp.css_margin("2em", true);
    csslib->add(css_class_text, tmp);

    tmp.clear();
    tmp.css_margin_left("9.4em");
    csslib->add(css_class_margin, tmp);

    webdar_css_style::update_library(*csslib);
}
