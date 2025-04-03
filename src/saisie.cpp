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
#include "exceptions.hpp"
#include "webdar_css_style.hpp"
#include "tokens.hpp"
#include "environment.hpp"

    //
#include "saisie.hpp"
#include "html_text.hpp"

using namespace std;

const string saisie::event_closing = "saisie_closing";
const string saisie::event_restore = "saisie_restore";
const string saisie::event_compare = "saisie_compare";
const string saisie::event_test    = "saisie_test";
const string saisie::event_list    = "saisie_list";
const string saisie::event_summary = "saisie_summ";
const string saisie::event_create  = "saisie_create";
const string saisie::event_isolate = "saisie_isolate";
const string saisie::event_merge   = "saisie_merge";
const string saisie::event_repair  = "saisie_repair";
const string saisie::changed_session_name = "saisie_changed_session_name";
const string saisie::event_disconn = "saisie_disconn";
const string saisie::event_download = "saisie_download";

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

const string saisie::css_class_logo = "saisie_logo";
const string saisie::css_class_margin = "saisie_marge";
const string saisie::css_class_choice = "saisie_choice";
const string saisie::css_class_license = "saisie_license";
const string saisie::css_class_rightpan = "saisie_rightpan";
const string saisie::css_class_float_clear = "saisie_floatclear";

const string saisie::event_demo = "demo";


saisie::saisie():
    archread(""),
    licensing((chemin(STATIC_PATH_ID) + chemin(STATIC_OBJ_LICENSING)).display(false), "Webdar is released under the GNU Public License v3"),
    session_name("Session name",
		 html_form_input::text,
		 "",
		 "",
		 webdar_css_style::width_60vw),
    about_fs(""),
    about_form("Change"),
    show_demo("Mini tuto", event_demo),
    webdar_logo((chemin(STATIC_PATH_ID) + chemin(STATIC_LOGO)).display(false), "Webdar logo"),
    go_extract("Restore", event_restore),
    go_compare("Compare", event_compare),
    go_test("Test", event_test),
    go_list("List", event_list),
    go_create("Create", event_create),
    go_isolate("Isolate", event_isolate),
    go_merge("Merge", event_merge),
    go_repair("Repair", event_repair)
{
    status = st_idle;
    html_text title; // used to build static text content
    html_text text; // used to build static text content

    licensing.set_download(true);
    licensing.set_filename("webdar licensing.txt");

    default_biblio_path = (chemin(global_envir.get_value_with_default("HOME", "/")) + chemin(".webdarrc")).display();

    biblio.reset(new (nothrow) bibliotheque());
    if(!biblio)
	throw exception_memory();

    h_biblio.reset(new (nothrow) html_bibliotheque(biblio, default_biblio_path));
    if(! h_biblio)
	throw exception_memory();

    test.reset(new (nothrow) html_options_test());
    if(! test)
	throw exception_memory();

    create.set_biblio(biblio);
    archread.set_biblio(biblio);
    merge.set_biblio(biblio);
    isolate.set_biblio(biblio);
    compare.set_biblio(biblio);
    repair.set_biblio(biblio);
    extract.set_biblio(biblio);

	// test is treated differently because there is no (need of) html_archive_test class
    test->set_biblio(biblio);

    guichet_test.set_child(biblio,
			   bibliotheque::conftest,
			   test,
			   false);

    if(biblio->has_config(bibliotheque::conftest, bibliotheque::default_config_name))
	guichet_test.load_from_bibliotheque(bibliotheque::default_config_name);

	// disconnect

    adopt(&disco);

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

	// Configuring archread

    right_pan.adopt(&archread);

	// configuration of the sub-pages brought by "select"

	//  configuration of the about sub-page
    title.clear();
    title.add_text(1,"WEBDAR");
    around_licensing.adopt_static_html(title.get_body_part());
    around_licensing.adopt(&webdar_logo);
    text.clear();
    text.add_text(4,string("version ") + WEBDAR_VERSION);
    text.add_paragraph();
    text.add_text(0, "by Denis CORBIN");
    text.add_nl();
    text.add_nl();
    text.add_text(0, "to my parents and my daughters");
    text.add_nl();
    text.add_text(0, "Capucine and C&eacute;lestine");
    text.add_nl();
    text.add_paragraph();
    around_licensing.adopt_static_html(text.get_body_part());
    around_licensing.adopt(&licensing);
    around_licensing.adopt_static_html(html_text(0,"").add_paragraph().get_body_part());
    select.adopt_in_section(menu_main, &around_licensing);
    about_fs.adopt(&session_name);
    about_form.adopt(&about_fs);
    select.adopt_in_section(menu_main, &about_form);
    select.adopt_in_section(menu_main, &demo);
    select.adopt_in_section(menu_main, &show_demo);

	// configuration of the restore sub-page
    select.adopt_in_section(menu_restore, &extract);
    select.adopt_in_section(menu_restore, &go_extract);

	// comparison sub-page
    select.adopt_in_section(menu_compare, &compare);
    select.adopt_in_section(menu_compare, &go_compare);

	// testing sub-page
    static const char* sect_test_params = "tparams";
    test_params.add_section(sect_test_params, "Testing Options");
    test_params.set_active_section(html_aiguille::noactive);

    test_params.adopt_in_section(sect_test_params, &guichet_test);
    select.adopt_in_section(menu_test, &test_params);
    select.adopt_in_section(menu_test, &go_test);

	// listing sub-page
    select.adopt_in_section(menu_list, &list_or_summ);
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
    select.adopt_in_section(menu_biblio, h_biblio.get());

	// other sessions sub_page
	// nothing to add "saisie" is not involved in that context

	// close section is put at the end of adoption tree for the close
	// confirmation popup (close)
	// to show over a complete rendered page

	/// configuration of "select"
    select.set_active_section(menu_main);
    right_pan.adopt(&select);
    adopt(&right_pan);

	// session close confirmation popup
    adopt(&close);

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
    disco.record_actor_on_event(this, html_disconnect::event_disconn);
    if(! h_biblio)
	throw WEBDAR_BUG;
    else
	h_biblio->record_actor_on_event(this, html_bibliotheque::event_download);
    close.record_actor_on_event(this, html_yes_no_box::answer_yes);
    close.record_actor_on_event(this, html_yes_no_box::answer_no);

    session_name.set_change_event_name(changed_session_name); // using the same event name as the we one we will trigger upon session name change
    session_name.record_actor_on_event(this, changed_session_name);
    show_demo.record_actor_on_event(this, event_demo);

	// other event to register
    register_name(event_restore);
    register_name(event_compare);
    register_name(event_test);
    register_name(event_list);
    register_name(event_summary);
    register_name(event_create);
    register_name(event_isolate);
    register_name(event_merge);
    register_name(event_repair);
    register_name(changed_session_name);
    register_name(event_disconn);
    register_name(event_download);

	// visibility
    demo.set_visible(false);

	// css
    text.add_css_class(css_class_text);
    right_pan.add_css_class(css_class_rightpan);
    webdar_logo.add_css_class(css_class_text);
    webdar_logo.add_css_class(css_class_logo);
    about_form.add_css_class(css_class_float_clear);
    webdar_css_style::normal_button(go_extract);
    webdar_css_style::normal_button(go_compare);
    webdar_css_style::normal_button(go_test);
    webdar_css_style::normal_button(go_list);
    webdar_css_style::normal_button(go_create);
    webdar_css_style::normal_button(go_isolate);
    webdar_css_style::normal_button(go_merge);
    webdar_css_style::normal_button(go_repair);
    go_extract.add_css_class(webdar_css_style::float_right);
    go_compare.add_css_class(webdar_css_style::float_right);
    go_test.add_css_class(webdar_css_style::float_right);
    go_list.add_css_class(webdar_css_style::float_right);
    go_create.add_css_class(webdar_css_style::float_right);
    go_isolate.add_css_class(webdar_css_style::float_right);
    go_merge.add_css_class(webdar_css_style::float_right);
    go_repair.add_css_class(webdar_css_style::float_right);
    webdar_css_style::normal_button(test_params, true);
    choice.add_css_class(css_class_choice);
    around_licensing.add_css_class(css_class_license);
    select.add_css_class(css_class_margin);

    webdar_css_style::normal_button(show_demo);
    show_demo.add_css_class(webdar_css_style::float_right);
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
    if(choice.get_current_tag() == menu_close)
    {
	    // session is closing

	close.ask_question("Do you really want to close this session?", false);
	ret = html_page::inherited_get_body_part(path, req);
    }
    else
    {
	if(choice.get_current_tag() == menu_sessions)
	{
		// user asked to see session list

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
	{
		// normal display

	    ret = html_page::inherited_get_body_part(path, req);
	}
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
    string propagated_event_name = event_name;

    if(event_name == html_menu::changed
       || event_name == html_form_input::changed
       || event_name == "")
    {
	// menu "choice" changed

	string tag_to_use = choice.get_current_tag();

	if(choice.get_current_tag() == menu_close)
	    tag_to_use = choice.get_previous_tag();

	set_title(webdar_tools_get_title(get_session_name(), choice.get_current_label()));
	if(tag_to_use == menu_restore
	   || choice.get_current_tag() == menu_compare
	   || choice.get_current_tag() == menu_test
	   || choice.get_current_tag() == menu_list
	   || choice.get_current_tag() == menu_isolate
	   || choice.get_current_tag() == menu_merge
	   || choice.get_current_tag() == menu_repair)
	    archread.set_visible(true);
	else
	    archread.set_visible(false);
	select.set_active_section(tag_to_use);

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
	{
	    if(list_or_summ.do_we_list())
		status = st_list;
	    else
	    {
		propagated_event_name = event_summary;
		status = st_summary;
	    }
	}
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
	act(propagated_event_name); // propagate the event to the subscribers

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
    else if(event_name == html_disconnect::event_disconn)
	act(event_disconn); // propagate the event
    else if(event_name == html_bibliotheque::event_download)
    {
	if(!to_download)
	    throw WEBDAR_BUG;
	if(!biblio)
	    throw WEBDAR_BUG;
	try
	{
	    to_download->set_data(biblio->save_json().dump());
	}
	catch(json::exception & e)
	{
	    throw exception_json(string("dumping configuration from json object: "), e);
	}
	act(event_download);
    }
    else if(event_name == html_yes_no_box::answer_yes)
    {
	act(event_closing);
	set_title(webdar_tools_get_title(get_session_name(), "Session closed"));
	set_refresh_redirection(0, "/");
    }
    else if(event_name == html_yes_no_box::answer_no)
    {
	choice.set_current_mode(choice.get_previous_mode());
    }
    else if(event_name == event_demo)
    {
	demo.set_visible(true);
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
    case st_summary:
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
    case st_summary:
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
       && status != st_summary
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
    case st_summary:
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

    return test->get_options();
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

bool saisie::do_we_list() const
{
    if(status != st_list
       && status != st_summary)
	throw WEBDAR_BUG;

    return list_or_summ.do_we_list();
}

void saisie::new_css_library_available()
{
    css tmp;

    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);

    tmp.clear();
    tmp.css_max_width("100%");
    csslib->add(css_class_logo, tmp);

    tmp.clear();
    tmp.css_margin_left("9.2em");
    tmp.css_box_sizing(css::bx_border);
    csslib->add(css_class_rightpan, tmp);

    tmp.clear();
    tmp.css_float(css::fl_left);
    csslib->add(css_class_choice, tmp);

    tmp.clear();
    tmp.css_max_width("100%");
    tmp.css_margin("2em");
    tmp.css_text_h_align(css::al_center);
    csslib->add(css_class_license, tmp);

    tmp.clear();
    tmp.css_text_h_align(css::al_center);
    tmp.css_margin("2em");
    csslib->add(css_class_text, tmp);

    tmp.clear();
    tmp.css_margin_left("9.4em");
    csslib->add(css_class_margin, tmp);

    tmp.clear();
    tmp.css_float_clear(css::fc_right);
    csslib->add(css_class_float_clear, tmp);
}
