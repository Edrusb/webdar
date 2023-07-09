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
const string saisie::changed_session_name = "saisie_changed_session_name";

const string saisie::css_class_text = "saisie_text";

saisie::saisie():
    archread("Source archive"),
    show_operation_options("Show operation options",
			   html_form_input::check,
			   "",
			   1),
    show_archive_form_options("Update"),
    show_archive_fs_options("Options details"),
    licensing((chemin(STATIC_PATH_ID) + chemin(STATIC_OBJ_LICENSING)).display(false), "Webdar is released under the GNU Public License v3"),
    session_name("Session name",
		 html_form_input::text,
		 "",
		 20),
    about_fs(""),
    about_form("Change"),
    extract_fs_root_fs("Restoration parameters"),
    extract_fs_root("Directory to take as root for restoration", html_form_input::text, "", 30),
    extract_fs_root_form("Update"),
    go_extract("Restore", event_restore),
    diff_fs_root_fs("Comparison parameters"),
    diff_fs_root("Directory to compare the archive with", html_form_input::text, "", 30),
    diff_fs_root_form("Update"),
    go_compare("Compare", event_compare),
    go_test("Test", event_test),
    go_list("List", event_list),
    go_create("Create", event_create),
    go_isolate("Isolate", event_isolate),
    go_merge("Merge", event_merge),
    close("Do you really want to close this session?", false)
{
    status = st_idle;
    html_text text; // used to build static text content

    webdar_css_style::normal_button(go_extract);
    webdar_css_style::normal_button(go_compare);
    webdar_css_style::normal_button(go_test);
    webdar_css_style::normal_button(go_list);
    webdar_css_style::normal_button(go_create);
    webdar_css_style::normal_button(go_isolate);
    webdar_css_style::normal_button(go_merge);


	// configuration of "choice"
    choice.add_entry("Main Page");
    choice.add_entry("");
    choice.add_entry("Restoration");
    choice.add_entry("Comparison");
    choice.add_entry("Testing");
    choice.add_entry("Listing");
    choice.add_entry("Creation");
    choice.add_entry("Isolation");
    choice.add_entry("Merging");
    choice.add_entry("");
    choice.add_entry("Filters");
    choice.add_entry("Repositories");
    choice.add_entry("Other Sessions");
    choice.add_entry("");
    choice.add_entry("Close Session");
    adopt(&choice);

	// Configuring archive_show
    archive_show.adopt(&archread);
    show_archive_form_options.adopt(&show_operation_options);
    show_archive_fs_options.adopt(&show_archive_form_options);
    archive_show.adopt(&show_archive_fs_options);
    right_pan.adopt(&archive_show);

	// configuration of the sub-pages brought by "select"

	//  configuration of the about sub-page
    text.clear();
    text.add_text(1,"WEBDAR");
    text.add_paragraph();
    text.add_text(4,string("version ") + WEBDAR_VERSION);
    text.add_paragraph();
    text.add_text(0, "by Denis CORBIN");
    text.add_text(0, "");
    text.add_paragraph();
    text.add_css_class(css_class_text);
    around_licensing.adopt_static_html(text.get_body_part());
    around_licensing.adopt(&licensing);
    div_about.adopt(&around_licensing);
    div_about.adopt_static_html(html_text(0,"").add_paragraph().get_body_part());
    about_fs.adopt(&session_name);
    about_form.adopt(&about_fs);
    div_about.adopt(&about_form);
    select.adopt(&div_about);

	// separator
    select.adopt(&div_sep0);

	// configuration of the restore sub-page
    div_extract.adopt(&extract);
    div_extract.adopt(&extract_fs_root_form);
    extract_fs_root_form.adopt(&extract_fs_root_fs);
    extract_fs_root_fs.adopt(&extract_fs_root);
    div_extract.adopt(&go_extract);
    select.adopt(&div_extract);

	// comparison sub-page
    div_compare.adopt(&compare);
    div_compare.adopt(&diff_fs_root_form);
    diff_fs_root_form.adopt(&diff_fs_root_fs);
    diff_fs_root_fs.adopt(&diff_fs_root);
    div_compare.adopt(&go_compare);
    select.adopt(&div_compare);

	// testing sub-page
    div_test.adopt(&test);
    div_test.adopt(&go_test);
    select.adopt(&div_test);

	// listing sub-page
    div_list.adopt(&go_list);
    select.adopt(&div_list);

	// creation sub-page
    div_create.adopt(&create);
    div_create.adopt(&go_create);
    select.adopt(&div_create);

	// isolation sub-page
    div_isolate.adopt(&isolate);
    div_isolate.adopt(&go_isolate);
    select.adopt(&div_isolate);

	// merging sub-page
    div_merge.adopt(&merge);
    div_merge.adopt(&go_merge);
    select.adopt(&div_merge);

	// separator
    select.adopt(&div_sep1);

	// filter sub-page
    text.clear();
    text.add_text(1,"NOT YET IMPLEMENTED");
    div_filters.adopt_static_html(text.get_body_part());
    select.adopt(&div_filters);

	// repository sub-page
    div_repo.adopt_static_html(text.get_body_part());
    select.adopt(&div_repo);

	// other sessions sub_page
    select.adopt(&div_sess);

	// separator
    select.adopt(&div_sep2);

    text.clear();
    text.add_text(1, "BUG: THIS separation PAGE SHOULD NEVER SHOW");
    div_sep0.adopt_static_html(text.get_body_part());
    div_sep1.adopt_static_html(text.get_body_part());
    div_sep2.adopt_static_html(text.get_body_part());

	// close session sub_page
    select.adopt(&close);

	/// configuration of "select"
    select.set_mode(0);
    right_pan.adopt(&select);
    adopt(&right_pan);

	// define the closing event for this
    register_name(event_closing);

	// attaching the "changed" event of the menu "choice" to "this" saisie object
    choice.record_actor_on_event(this);
	// attaching the "changed" event of the show_operation_options checkbox to "this"
    show_operation_options.record_actor_on_event(this, html_form_input::changed);
    on_event(""); // manually triggering the event for the initial setup

    go_extract.record_actor_on_event(this, event_restore);
    go_compare.record_actor_on_event(this, event_compare);
    go_test.record_actor_on_event(this, event_test);
    go_list.record_actor_on_event(this, event_list);
    go_create.record_actor_on_event(this, event_create);
    go_isolate.record_actor_on_event(this, event_isolate);
    go_merge.record_actor_on_event(this, event_merge);

    session_name.set_change_event_name(changed_session_name); // using the same event name as the we we will trigger upon session name change
    session_name.record_actor_on_event(this, changed_session_name);

	// other event to register
    register_name(event_restore);
    register_name(event_compare);
    register_name(event_test);
    register_name(event_list);
    register_name(event_create);
    register_name(event_isolate);
    register_name(event_merge);
    register_name(changed_session_name);
}

string saisie::inherited_get_body_part(const chemin & path,
				       const request & req)
{
    chemin sub_path = path;
    string ret;

    if(!sub_path.empty())
	sub_path.pop_front();

    status = st_idle;

	// now we can generate in return the whole HTML code for "this" object
    set_refresh_redirection(0,""); // clearing redirection that could have been set previously
    ret = html_page::inherited_get_body_part(path, req);
    if(choice.get_current_label() == "Close Session")
    {
	if(close.get_value())
	{
	    act(event_closing);
	    set_title(webdar_tools_get_title(get_session_name(), "Session closed"));
	    set_refresh_redirection(0, "/");
	    ret = html_page::inherited_get_body_part(path, req);
	}
    }
    else
	if(choice.get_current_label() == "Other Sessions")
	{
	    set_title(webdar_tools_get_title(get_session_name(), "Redirection to all user sessions"));
	    set_refresh_redirection(0, "/");
	    ret = html_page::inherited_get_body_part(path, req);
	    choice.set_current_mode(choice.get_previous_mode());
	}

    return ret;
}

void saisie::on_event(const std::string & event_name)
{
    if(event_name == html_menu::changed
       || event_name == html_form_input::changed
       || event_name == "")
    {
	// menu "choice" changed

	set_title(webdar_tools_get_title(get_session_name(), choice.get_current_label()));
	if(choice.get_current_label() == "Restoration"
	   || choice.get_current_label() == "Comparison"
	   || choice.get_current_label() == "Testing"
	   || choice.get_current_label() == "Listing")
	    archive_show.set_visible(true);
	else
	    archive_show.set_visible(false);
	select.set_mode(choice.get_current_mode());

	if(show_operation_options.get_value_as_bool())
	{
	    extract.set_visible(true);
	    compare.set_visible(true);
	    test.set_visible(true);
	}
	else
	{
	    extract.set_visible(false);
	    compare.set_visible(false);
	    test.set_visible(false);
	}
    }
    else if(event_name == event_restore
	    || event_name == event_compare
	    || event_name == event_test
	    || event_name == event_list
	    || event_name == event_create
	    || event_name == event_isolate
	    || event_name == event_merge)
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
	else
	    throw WEBDAR_BUG;
	act(event_name); // propagate the event to the subscribers
    }
    else if(event_name == changed_session_name)
    {
	set_title(webdar_tools_get_title(get_session_name(), choice.get_current_label()));
	act(changed_session_name);
	// propagating the event
    }
    else
	throw WEBDAR_BUG;
}

const string & saisie::get_archive_path() const
{
    switch(status)
    {
    case st_idle:
	throw WEBDAR_BUG;
    case st_restore:
    case st_compare:
    case st_test:
    case st_list:
	return archread.get_archive_path();
	break;
    case st_isolate:
	return isolate.get_archive_path();
	break;
    case st_create:
	return create.get_archive_path();
	break;
    case st_merge:
	return merge.get_archive_path();
    default:
	throw WEBDAR_BUG;
    }
}

const string & saisie::get_archive_basename() const
{
    switch(status)
    {
    case st_idle:
	throw WEBDAR_BUG;
    case st_restore:
    case st_compare:
    case st_test:
    case st_list:
	return archread.get_archive_basename();
    case st_isolate:
	return isolate.get_archive_basename();
    case st_create:
	return create.get_archive_basename();
    case st_merge:
	return merge.get_archive_basename();
    default:
	throw WEBDAR_BUG;
    }
}

const libdar::archive_options_read & saisie::get_read_options() const
{
    if(status != st_restore
       && status != st_compare
       && status != st_test
       && status != st_list)
	throw WEBDAR_BUG;

    return archread.get_read_options();
}

const string & saisie::get_fs_root() const
{
    switch(status)
    {
    case st_idle:
	throw WEBDAR_BUG;
    case st_restore:
	return extract_fs_root.get_value();
    case st_compare:
	return diff_fs_root.get_value();
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

const html_options_create & saisie::get_creating_options() const
{
    if(status != st_create)
	throw WEBDAR_BUG;

    return create.get_options_create();
}

const libdar::archive_options_isolate saisie::get_isolating_options() const
{
    if(status != st_isolate)
	throw WEBDAR_BUG;

    return isolate.get_options_isolate().get_options();
}

const html_options_merge & saisie::get_merging_options() const
{
    if(status != st_merge)
	throw WEBDAR_BUG;

    return merge.get_options_merge();
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

    tmp.css_clear_attributes();
    tmp.css_margin_left("9.4em");
    csslib->add(css_class_rightpan, tmp);
    right_pan.add_css_class(css_class_rightpan);

    tmp.css_clear_attributes();
    tmp.css_float(css::fl_right);
    csslib->add(css_class_right, tmp);
    go_extract.add_css_class(css_class_right);
    go_compare.add_css_class(css_class_right);
    go_test.add_css_class(css_class_right);
    go_list.add_css_class(css_class_right);
    go_create.add_css_class(css_class_right);
    go_isolate.add_css_class(css_class_right);
    go_merge.add_css_class(css_class_right);

    tmp.css_clear_attributes();
    tmp.css_float(css::fl_left);
    csslib->add(css_class_choice, tmp);
    choice.add_css_class(css_class_choice);

    tmp.css_clear_attributes();
    tmp.css_width("90%", true);
    tmp.css_margin_bottom("1em");
    tmp.css_margin_top("1em");
    tmp.css_text_align(css::al_center);
    csslib->add(css_class_license, tmp);
    around_licensing.add_css_class(css_class_license);

    tmp.css_clear_attributes();
    tmp.css_text_align(css::al_center, true);
    tmp.css_margin("2em", true);
    csslib->add(css_class_text, tmp);

    tmp.css_clear_attributes();
    tmp.css_margin_left("9.4em");
    csslib->add(css_class_margin, tmp);
    select.add_css_class(css_class_margin);

    webdar_css_style::update_library(*csslib);
}
