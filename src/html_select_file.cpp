/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2023 Denis Corbin
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
#include <fnmatch.h>
}

    // C++ system header files
#include <utility>
#include <deque>
#include <algorithm>

    // webdar headers
#include "webdar_css_style.hpp"
#include "html_page.hpp"

    //
#include "html_select_file.hpp"

using namespace std;

    // public static fields:
const string html_select_file::entry_selected = "entry_selected";
const string html_select_file::op_cancelled = "op_cancelled";

    // private static fields:
const string html_select_file::op_chdir_parent = "parent";
const string html_select_file::op_createdir = "createdir";
const string html_select_file::css_float_button_right = "html_select_file_btn_right";
const string html_select_file::css_float_button_left = "html_select_file_btn_left";
const string html_select_file::css_warning = "html_select_warning";
const string html_select_file::css_sticky_top = "html_select_sticky_top";
const string html_select_file::css_sticky_bot = "html_select_sticky_bot";

html_select_file::html_select_file(const std::string & message):
    html_popup(width_pct,height_pct),
    status(st_init),
    which_thread(run_nothing),
    select_dir(false),
    filter(""),
    should_refresh(false),
    apply_refresh_mode(true),
    title(2, message),
    warning(3, ""),
    fieldset(""),
    path_loaded(""),
    parentdir("Parent Directory", op_chdir_parent),
    content(2),
    content_placeholder(2, "Loading directory content..."),
    btn_cancel("Cancel", op_cancelled),
    btn_validate("Select", entry_selected),
    btn_createdir("New Folder", op_createdir),
    createdir_form("Create Folder"),
    createdir_input("Folder Name", html_form_input::text, "", 25),
    fieldset_isdir(true)
{
    entr.reset();       // entr points to nothing
    mem_ui.reset();     // mem_ui points to nothing
    webui.set_warning_list_size(5);

	// events for callers objects
    register_name(entry_selected);  // we'll propagate the even from btn_validate
    register_name(op_cancelled);    // we will propagate the even from btn_cancel

	// events we want to act on from our own html components
    btn_cancel.record_actor_on_event(this, op_cancelled);
    btn_validate.record_actor_on_event(this, entry_selected);
    parentdir.record_actor_on_event(this, op_chdir_parent);
    btn_createdir.record_actor_on_event(this, op_createdir);
    createdir_input.record_actor_on_event(this, html_form_input::changed);
    webui.record_actor_on_event(this, html_web_user_interaction::can_refresh);
    webui.record_actor_on_event(this, html_web_user_interaction::dont_refresh);

	// setting up the adoption tree (the fixed part)
    adopt(&title_box);
    title_box.adopt(&title);
    title_box.adopt(&warning);
    title_box.adopt(&webui);
    adopt(&fieldset);
    fieldset.adopt(&parentdir);
    fieldset.adopt(&content);
    fieldset.adopt(&content_placeholder);
    adopt(&btn_box);
    btn_box.adopt(&btn_createdir);
    btn_box.adopt(&btn_validate);
    btn_box.adopt(&btn_cancel);
    btn_box.adopt(&createdir_form);
    createdir_form.adopt(&createdir_input);

	// assigning CSS classes names to html components
    title_box.add_css_class(css_sticky_top);
    warning.add_css_class(css_warning);
    webdar_css_style::normal_button(btn_cancel);
    webdar_css_style::normal_button(btn_validate);
    webdar_css_style::normal_button(btn_createdir);
    btn_box.add_css_class(css_sticky_bot);
    btn_cancel.add_css_class(css_float_button_right);
    btn_validate.add_css_class(css_float_button_right);
    btn_createdir.add_css_class(css_float_button_left);

	// setup default visibility property

    set_visible(false); // make us invisible until go_select() is called
    webui.set_visible(false);
    webui.auto_hide(true, true);
}

void html_select_file::go_select(const shared_ptr<libdar::entrepot> & x_entr,
				 const std::string & start_dir)
{
    switch(status)
    {
    case st_init:
	break;
    case st_go_select:
	return; // already running, ignoring subsequent requests
    case st_completed:
	throw WEBDAR_BUG; // previous value has not been read
    default:
	throw WEBDAR_BUG;
    }

    set_visible(true);
    status = st_go_select;
    entr = x_entr;
    mem_ui = entr->get_current_user_interaction();
    entr->change_user_interaction(webui.get_user_interaction());
    fieldset.change_label(start_dir);
    createdir_form.set_visible(false);
    run_thread(run_init_fill);
    my_body_part_has_changed(); // should not be necessary thanks to set_visible(true) above, but it does not hurt
};

void html_select_file::on_event(const std::string & event_name)
{
    if(event_name == entry_selected)
    {
	if(!select_dir && fieldset_isdir)
	{
	    warning.add_text(3, string("This is a directory, please select a non-directory file"));
	    my_body_part_has_changed();
	}
	else
	{
	    status = st_completed;
	    set_visible(false);
	    act(entry_selected); // propagate the event to object that subscribed to us
	    entr->change_user_interaction(mem_ui);
	    entr.reset();   // forget about the go_select() provided entrepot
	    mem_ui.reset(); // forget about the user_interaction entr had
	    my_body_part_has_changed();
	    join();
	}
    }
    else if(event_name == op_cancelled)
    {
	status = st_init;
	set_visible(false);
	act(op_cancelled);
	entr->change_user_interaction(mem_ui);
	entr.reset();    // forget about the go_select() provided entrepot
	mem_ui.reset();  // forget about the user_interaction entr had
	my_body_part_has_changed();
	join();
    }
    else if(event_name == op_chdir_parent)
    {
	chemin chem(fieldset.get_label());

	try
	{
	    chem.pop_back();
	}
	catch(exception_range & e)
	{
		// pop_back() failed because we
		// we reached the root of the filesystem
		// ignoring this error and staying at
		// the same path
	}
	fieldset.change_label(chem.display());
	fieldset_isdir = true;
	loading_mode(true);
	my_body_part_has_changed();
    }
    else if(event_name == op_createdir)
    {
	createdir_form.set_visible(!createdir_form.get_visible());
	createdir_input.set_value("");

	my_body_part_has_changed();
    }
    else if(event_name == html_form_input::changed) // directory name provided to be created
    {
	if(! entr)
	    throw WEBDAR_BUG;

	    // ignoring empty string as value, this event may be triggered by
	    // the clearing of the previous field value
	if(! createdir_input.get_value().empty())
	{
	    createdir_form.set_visible(false);
	    run_thread(run_create_dir);
	}

	my_body_part_has_changed();
    }
    else if(event_name == html_web_user_interaction::can_refresh)
    {
	if(! should_refresh)
	{
	    should_refresh = true;
	    apply_refresh_mode = true;
	    my_body_part_has_changed();
	}
    }
    else if (event_name == html_web_user_interaction::dont_refresh)
    {
	if(should_refresh)
	{
	    should_refresh = false;
	    apply_refresh_mode = true;
	    my_body_part_has_changed();
	}
    }
    else // click on directory list entry?
    {
	    // here were access to "listed" without locking the mutex
	    // but the on_event() call should never be invoked from
	    // outside our own inherited_get_body_part() as we only
	    // registered to events from our own components. These are only
	    // consulted during our own inherited_get_body_part() which
	    // has either acquired to mtx_content lock for us, or has set
	    // content object as invisible, so we can skip "listed" content
	    // check.

	if(content.get_visible())
	{
	    map<string, item>::iterator it = listed.find(event_name);
	    chemin curdir(fieldset.get_label());
	    string tmp;

	    if(it == listed.end())
		throw WEBDAR_BUG; // all events we registered for should be known by us
	    if(it->second.btn == nullptr)
		throw WEBDAR_BUG;


	    if(!fieldset_isdir)
	    {
		string prev_file;

		    // if the current select item is not a directory we have not
		    // changed directory into it, the current path is the dirname
		    // of what fieldset points to, so we "pop" the current selected
		    // filename from the current path to get back to the currentdir

		curdir.pop_back();
	    }

		// we concatenate (as a path subdir) the current path with the filename the user has clicked on:

	    curdir += chemin((it->second.btn)->get_label());
	    fieldset.change_label(curdir.display());
	    fieldset_isdir = it->second.isdir;
	}
	else
		// we avoid consulting listed as it may still be  under construction
		// and second as content and parentdir are hidden, they should not
		// lead to any event generation
	    throw WEBDAR_BUG; // unexpected event

	my_body_part_has_changed();
    }
}

string html_select_file::inherited_get_body_part(const chemin & path,
						 const request & req)
{
    if(get_visible())
    {
	string ret;
	bool acquired_content = mtx_content.try_lock();

	try
	{
	    if(acquired_content && which_thread == run_nothing)
		join(); // possibly trigger exception from our previously running child thread

	    loading_mode(!acquired_content);
		// we show content only we could acquire the mutex lock

	    html_popup::inherited_get_body_part(path, req);
		// a first time to have the even triggering parameter changes

	    if(apply_refresh_mode)
	    {
		html_page* page = nullptr;

		closest_ancestor_of_type(page);
		apply_refresh_mode = false;

		if(page != nullptr)
		{
		    if(should_refresh)
			page->set_refresh_redirection(1, req.get_uri().get_path().display(false));
		    else
			page->set_refresh_redirection(0, ""); // disable refresh
		}
	    }

	if(entr && which_thread == run_nothing)
	    run_thread(run_fill_only);

	ret = html_popup::inherited_get_body_part(path, req);

	return ret;
    }
    else
    {
	join();
	return "";
    }
}

void html_select_file::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    css tmp;

    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
    html_popup::new_css_library_available();

    tmp.clear();

    tmp.css_float(css::fl_right);
    if(!csslib->class_exists(css_float_button_right))
	csslib->add(css_float_button_right, tmp);

    tmp.clear();

    tmp.css_float(css::fl_left);
    if(!csslib->class_exists(css_float_button_left))
	csslib->add(css_float_button_left, tmp);

    tmp.clear();

    tmp.css_font_weight_bold();
    tmp.css_color("rgb(255,30,10)");
    if(!csslib->class_exists(css_warning))
	csslib->add(css_warning, tmp);

    tmp.clear();

    tmp.css_position_type(css::pos_sticky);
    tmp.css_box_sizing(css::bx_border);
    tmp.css_position_top("0");
    tmp.css_border_style(css::bd_bottom, css::bd_solid);
    tmp.css_border_width(css::bd_bottom, css::bd_thin);
    tmp.css_margin("0");
    tmp.css_padding("10px");
    tmp.css_padding_bottom("10px");
    tmp.css_width("100%", true);
    tmp.css_background_color("white");
    if(!csslib->class_exists(css_sticky_top))
	csslib->add(css_sticky_top, tmp);

    tmp.clear();

    tmp.css_position_type(css::pos_sticky);
    tmp.css_box_sizing(css::bx_border);
    tmp.css_float(css::fl_left);
    tmp.css_position_bottom("0");
    tmp.css_border_style(css::bd_top, css::bd_solid);
    tmp.css_border_width(css::bd_top, css::bd_thin);
    tmp.css_margin("0");
    tmp.css_padding("10px");
    tmp.css_width("100%", true);
    tmp.css_background_color("white");
    if(!csslib->class_exists(css_sticky_bot))
	csslib->add(css_sticky_bot, tmp);
}

void html_select_file::inherited_run()
{
    mtx_content.lock();
    try
    {
	switch(which_thread)
	{
	case run_nothing:
	    throw WEBDAR_BUG;
	case run_create_dir:
	    create_dir();
	    break;
	case run_init_fill:
	    init_fieldset_isdir();
		/* no break ! */
	case run_fill_only:
	    fill_content();
	    break;
	default:
	    throw WEBDAR_BUG;
	}
    }
    catch(...)
    {
	which_thread = run_nothing;
	mtx_content.unlock();
	throw;
    }
    which_thread = run_nothing;
    mtx_content.unlock();
}


void html_select_file::init_fieldset_isdir()
{
    chemin target(fieldset.get_label());

    if(!entr)
	throw WEBDAR_BUG;

	// first trying to open target as a if it was a directory
	// if that fails we will try to open the parent directory
	// to check for a possible existence of target

    try
    {
	entr->set_location(target.display());
	entr->read_dir_reset_dirinfo();
	fieldset_isdir = true;
    }
    catch(libdar::Erange & e)
    {
	    // assuming the operation failed
	    // because the target is not a directory
	    // trying to open the parent directory
	try
	{
	    target.pop_back();
	    entr->set_location(target.display());
	    entr->read_dir_reset_dirinfo();
	    fieldset_isdir = false;
	}
	catch(exception_range & f)
	{
		// target.pop_back() failed
		// this means target was empty (aka root filesystem)
		// and "/" was not a directory or failed to be openned
		// reporting the first exception
	    warning.clear();
	    warning.add_text(3, e.get_message());
	}
	catch(libdar::Erange & f)
	{
		// target.pop_back() succeeded by the
		// libdar iperation failed, propagating this
		// exception
	    warning.clear();
	    warning.add_text(3, f.get_message());
	}
    }
    catch(libdar::Egeneric & e)
    {
	warning.clear();
	warning.add_text(3, string("Unexpected error returned by libdar:"));
	warning.add_text(4, e.get_message());
    }
}

void html_select_file::fill_content()
{
    string entry;
    bool isdir;
    string event_name;
    unsigned int count = 0;
    deque<string> entry_dirs;
    deque<string> entry_files;

    clear_content();

    if(!entr)
	throw WEBDAR_BUG;

    try
    {
	if(fieldset_isdir)
	    entr->set_location(fieldset.get_label());

	entr->read_dir_reset_dirinfo();

	while(entr->read_dir_next_dirinfo(entry, isdir))
	{
	    if(isdir || filter.empty() || fnmatch(filter.c_str(), entry.c_str(), FNM_PERIOD) == 0)
	    {
		if(isdir)
		    entry_dirs.push_back(entry);
		else
		    entry_files.push_back(entry);
	    }
		// else ignoring entry because
		// this is not a directory
		// and a mask filtering has been set
		// and the entry does not match the filter
	}

	sort(entry_dirs.begin(), entry_dirs.end());
	sort(entry_files.begin(), entry_files.end());

	deque<string>::iterator it = entry_dirs.begin();

	while(it != entry_dirs.end())
	{
	    event_name = "x_" + to_string(count++);
	    if(listed.find(event_name) != listed.end())
		throw WEBDAR_BUG; // event already exists!?!
	    add_content_entry(event_name, true, *it);
	    ++it;
	}

	it = entry_files.begin();
	while(it != entry_files.end())
	{
	    event_name = "x_" + to_string(count++);
	    if(listed.find(event_name) != listed.end())
		throw WEBDAR_BUG; // event already exists!?!
	    add_content_entry(event_name, false, *it);
	    ++it;
	}
    }
    catch(libdar::Ebug & e)
    {
	throw;
    }
    catch(libdar::Egeneric & e)
    {
	warning.clear();
	warning.add_text(3, e.get_message());
    }
}

void html_select_file::create_dir()
{
    entr->create_dir(createdir_input.get_value(), 0700);
}

void html_select_file::add_content_entry(const string & event_name, bool isdir, const string & entry)
{
    item current;

    content.adopt_static_html(isdir ? " DIR " : "");

    current.isdir = isdir;
    if(!select_dir || isdir)
    {
	current.btn = new (nothrow) html_button(entry, event_name);
	if(current.btn == nullptr)
	    throw exception_memory();

	listed[event_name] = current;
	content.adopt(current.btn);
	current.btn->record_actor_on_event(this, event_name);
    }
    else
	content.adopt_static_html(entry);
}

void html_select_file::run_thread(thread_to_run val)
{
    if(which_thread != run_nothing)
	throw WEBDAR_BUG; // a thread is already running

    switch(val)
    {
    case run_nothing:
	throw WEBDAR_BUG;
    case run_create_dir:
	path_loaded = ""; // this will force reloading dir content
	which_thread = val;
	webui.run_and_control_thread(this);
	break;
    case run_init_fill:
    case run_fill_only:
	if(fieldset.get_label() != path_loaded)
	{
	    path_loaded = fieldset.get_label();
	    which_thread = val;
	    webui.run_and_control_thread(this);
	}
	    // else we are already displaying the requested directory content
	break;
    default:
	throw WEBDAR_BUG;
    }
}

void html_select_file::clear_content()
{
    map<string, item>::iterator it = listed.begin();

    content.clear();
    while(it != listed.end())
    {
	if(it->second.btn != nullptr)
	{
	    delete it->second.btn;
	    it->second.btn = nullptr;
	}

	++it;
    }
    listed.clear();
}

void html_select_file::loading_mode(bool mode)
{
    if(mode)
    {
	parentdir.set_visible(false);
	content.set_visible(false);
	content_placeholder.set_visible(true);
    }
    else
    {
	parentdir.set_visible(true);
	content.set_visible(true);
	content_placeholder.set_visible(false);
    }
}

