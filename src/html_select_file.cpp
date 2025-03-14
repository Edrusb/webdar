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
#if HAVE_FNMATCH_H
#include <fnmatch.h>
#endif
}

    // C++ system header files
#include <utility>
#include <deque>
#include <algorithm>

    // webdar headers
#include "webdar_css_style.hpp"
#include "html_page.hpp"
#include "tokens.hpp"
    //
#include "html_select_file.hpp"

using namespace std;

    // public static fields:
const string html_select_file::entry_selected = "entry_selected";
const string html_select_file::op_cancelled = "op_cancelled";

    // private static fields:
const string html_select_file::op_chdir_parent = "parent";
const string html_select_file::op_createdir = "createdir";
const string html_select_file::op_hide_createdir = "hide_createdir";

const string html_select_file::css_float_button_right = "html_select_file_btn_right";
const string html_select_file::css_float_button_left = "html_select_file_btn_left";
const string html_select_file::css_warning = "html_select_warning";
const string html_select_file::css_sticky_top = "html_select_sticky_top";
const string html_select_file::css_sticky_bot = "html_select_sticky_bot";

html_select_file::html_select_file(const string & message):
    x_message(message),
    html_popup(width_pct,height_pct),
    status(st_init),
    which_thread(run_nothing),
    cur_select_mode(sel_file),
    filter(""),
    should_refresh(false),
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
    btn_hide_createdir("New Folder", op_hide_createdir),
    createdir_form("Create Folder"),
    createdir_input("Folder Name", html_form_input::text, "", "80%"),
    fieldset_isdir(true)
{
    entr.reset();       // entr points to nothing
    mem_ui.reset();     // mem_ui points to nothing
    webui.set_warning_list_size(5);

    warning.ignore_body_changed_from_my_children(true);

	// events for callers objects
    register_name(entry_selected);  // we'll propagate the even from btn_validate
    register_name(op_cancelled);    // we will propagate the even from btn_cancel

	// events we want to act on from our own html components
    btn_cancel.record_actor_on_event(this, op_cancelled);
    btn_validate.record_actor_on_event(this, entry_selected);
    parentdir.record_actor_on_event(this, op_chdir_parent);
    btn_createdir.record_actor_on_event(this, op_createdir);
    btn_hide_createdir.record_actor_on_event(this, op_hide_createdir);
    createdir_input.record_actor_on_event(this, html_form_input::changed);
    webui.record_actor_on_event(this, html_web_user_interaction::can_refresh);
    webui.record_actor_on_event(this, html_web_user_interaction::dont_refresh);
    webui.record_actor_on_event(this,html_web_user_interaction::libdar_has_finished);

	// setting up the adoption tree (the fixed part)
    adopt(&title_box);
    title_box.adopt(&title);
    title_box.adopt(&entrepot_url);
    title_box.adopt(&warning);
    adopt(&fieldset);
    fieldset.adopt(&parentdir);
    fieldset.adopt(&content);
    fieldset.adopt(&content_placeholder);
    adopt(&webui);
    adopt(&btn_box);
    btn_box.adopt(&btn_createdir);
    btn_box.adopt(&btn_hide_createdir);
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
    webdar_css_style::normal_button(btn_hide_createdir);
    btn_box.add_css_class(css_sticky_bot);
    btn_cancel.add_css_class(css_float_button_right);
    btn_validate.add_css_class(css_float_button_right);
    btn_createdir.add_css_class(css_float_button_left);
    btn_hide_createdir.add_css_class(css_float_button_left);

	// setup default visibility property

    ignore_body_changed_from_my_children(true); // ignore visibility change
    btn_hide_createdir.set_visible(false);

    set_visible(false); // make us invisible until go_select() is called
    webui.set_visible(false);
    webui.auto_hide(true, true);
    ignore_body_changed_from_my_children(false); // end of ignore visibility change
    loading_mode(false);
}

void html_select_file::go_select(const shared_ptr<libdar::entrepot> & x_entr,
				 const string & start_dir)
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
    path_loaded = "";
    if(mem_ui)
	throw WEBDAR_BUG;
    mem_ui = entr->get_current_user_interaction();
    entr->change_user_interaction(webui.get_user_interaction());
    entrepot_url.clear();
    entrepot_url.add_text(0, "Entrepot location: ");
    entrepot_url.add_text(0, entr->get_url());
    fieldset.change_label(start_dir);
    createdir_form.set_visible(false);
    clear_content();
    run_thread(run_init_fill);
    my_body_part_has_changed();
};

void html_select_file::on_event(const string & event_name)
{
    if(event_name == entry_selected)
    {
	if(fieldset_isdir && cur_select_mode != sel_dir)
	{
	    warning.add_text(3, string("This is a directory, please select a non-directory file"));
		// not necessary to call my_body_part_has_changed() as we just changed "warning" one of our own children
	}
	else
	{
	    status = st_completed;
	    act(entry_selected); // propagate the event to object that subscribed to us
	    my_join(true);
	}
    }
    else if(event_name == op_cancelled)
    {
	status = st_init;
	act(op_cancelled); // propagate the event to objects that subscribed to us
	my_join(true);
    }
    else if(event_name == op_chdir_parent)
    {
	fieldset.change_label(get_parent_path(fieldset.get_label()));
	fieldset_isdir = true;
	run_thread(run_fill_only);
    }
    else if(event_name == op_createdir)
    {
	createdir_form.set_visible(true);
	createdir_input.set_value("");
	btn_createdir.set_visible(false);
	btn_hide_createdir.set_visible(true);
	    // not necessary to call my_body_part_has_changed() as we just changed a child visibility
    }
    else if(event_name == op_hide_createdir)
    {
	createdir_form.set_visible(false);
	btn_createdir.set_visible(true);
	btn_hide_createdir.set_visible(false);
	    // not necessary to call my_body_part_has_changed() as we changed children visibility
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
	    my_body_part_has_changed();
	}
    }
    else if(event_name == html_web_user_interaction::dont_refresh)
    {
	if(should_refresh)
	{
	    should_refresh = false;
	    my_body_part_has_changed();
	}
    }
    else if(event_name == html_web_user_interaction::libdar_has_finished)
    {
	loading_mode(false);
	my_body_part_has_changed();
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
		    // if the current select item is not a directory we have not
		    // changed directory into it, the current path is the dirname
		    // of what fieldset points to, so we "pop" the current selected
		    // filename from the current path to get back to the currentdir
		curdir.pop_back();
	    }

		// we concatenate (as a path subdir) the current path with the filename the user has clicked on:

	    curdir += chemin((it->second.btn)->get_label());
	    fieldset.change_label(curdir.display());
	    if(it->second.type == libdar::inode_type::unknown)
	    {
		    // we must determine whether this a directory or not
		try
		{
		    entr->set_location(fieldset.get_label());
		    entr->read_dir_reset_dirinfo();
		    it->second.type = libdar::inode_type::isdir;
		}
		catch(...)
		{
		    it->second.type = libdar::inode_type::nondir;
		}
	    }


	    fieldset_isdir = (it->second.type == libdar::inode_type::isdir);

		// Warning: clearing content here will delete
		// the object which generated the event we
		// act on... leading to a SEGFAULT as at return
		// of on_event() the object will no more exist.
	    if(fieldset_isdir)
		run_thread(run_fill_only);
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
    string ret;
    html_page* page = nullptr;
    closest_ancestor_of_type(page);

    if(! webui.is_libdar_running())
    {
	my_join(false);

	if(!entr)
	    throw WEBDAR_BUG;
    }

    bool lock_acquired = content_mutex.try_lock();

    try
    {
	if(!lock_acquired && !is_loading_mode)
	    throw WEBDAR_BUG;
	    // ELSE as we are in loading mode or no thread will run until we release the mutex
	    // either we will not read 'content' because it is hidden, or no subthread will
	    // run during the time we proceed with html_popup::inherited_get_get_body_part()
	ret = html_popup::inherited_get_body_part(path, req);
    }
    catch(...)
    {
	if(lock_acquired)
	    content_mutex.unlock();
	throw;
    }
    if(lock_acquired)
	content_mutex.unlock();

    if(page != nullptr)
    {
	if(should_refresh)
	    page->set_refresh_redirection(1, req.get_uri().get_path().display(false));
	else
	    page->set_refresh_redirection(0, ""); // disable refresh
    }
    else
	throw WEBDAR_BUG; // cannot set refresh mode

    warning.ignore_body_changed_from_my_children(true);
    if(!should_refresh && ! has_my_body_part_changed())
	warning.clear();
    warning.ignore_body_changed_from_my_children(false);

    return ret;
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
    tmp.css_color(COLOR_ARED);
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
    content_mutex.lock();
    try
    {
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
		if(!init_fieldset_isdir())
		    break;
		else
		    cancellation_checkpoint();
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
	    throw;
	}

	which_thread = run_nothing;
    }
    catch(...)
    {
	content_mutex.unlock();
	throw;
    }
    content_mutex.unlock();
}


bool html_select_file::init_fieldset_isdir()
{
    bool ret = false;

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
	ret = true;
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
	    ret = true;
	}
	catch(exception_range & f)
	{
		// target.pop_back() failed
		// this means target was empty (aka root file system)
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

    return ret;
}

void html_select_file::fill_content()
{
    string entry;
    bool isdir;
    string event_name;
    unsigned int count = 0;
    deque<string> entry_dirs;
    deque<string> entry_files;
    deque<string> entry_symlinks;
    deque<string> entry_unknown;
    libdar::inode_type tp;

    clear_content();

    if(!entr)
	throw WEBDAR_BUG;

    if(fieldset_isdir)
	entr->set_location(fieldset.get_label());
    cancellation_checkpoint();
    entr->read_dir_reset_dirinfo();

    while(entr->read_dir_next_dirinfo(entry, tp))
    {
	cancellation_checkpoint();
	if(tp == libdar::inode_type::isdir
	   || tp == libdar::inode_type::unknown
	   || filter.empty()
	   || fnmatch(filter.c_str(), entry.c_str(), FNM_PERIOD) == 0)
	{
	    switch(tp)
	    {
	    case libdar::inode_type::isdir:
		entry_dirs.push_back(entry);
		break;
	    case libdar::inode_type::nondir:
		entry_files.push_back(entry);
		break;
	    case libdar::inode_type::symlink:
		entry_symlinks.push_back(entry);
		break;
	    case libdar::inode_type::unknown:
		entry_unknown.push_back(entry);
		break;
	    default:
		throw WEBDAR_BUG;
	    }
	}
	    // else ignoring entry because
	    // this is not a directory
	    // and a mask filtering has been set
	    // and the entry does not match the filter
    }

    sort(entry_dirs.begin(), entry_dirs.end());
    sort(entry_files.begin(), entry_files.end());
    sort(entry_symlinks.begin(), entry_symlinks.end());
    sort(entry_unknown.begin(), entry_unknown.end());

    deque<string>::iterator it = entry_dirs.begin();

	// we do the following complicated thing to
	// have the listing starting with directories
	// then symlinks, then non directories, followed
	// last by those we don't even know what's their type is.

    while(it != entry_dirs.end())
    {
	cancellation_checkpoint();
	event_name = "x_" + to_string(count++);
	if(listed.find(event_name) != listed.end())
	    throw WEBDAR_BUG; // event already exists!?!
	add_content_entry(event_name, libdar::inode_type::isdir, *it);
	++it;
    }

    it = entry_symlinks.begin();
    while(it != entry_symlinks.end())
    {
	cancellation_checkpoint();
	event_name = "x_" + to_string(count++);
	if(listed.find(event_name) != listed.end())
	    throw WEBDAR_BUG; // event already exists!?!
	add_content_entry(event_name, libdar::inode_type::symlink, *it);
	++it;
    }

    it = entry_files.begin();
    while(it != entry_files.end())
    {
	cancellation_checkpoint();
	event_name = "x_" + to_string(count++);
	if(listed.find(event_name) != listed.end())
	    throw WEBDAR_BUG; // event already exists!?!
	add_content_entry(event_name, libdar::inode_type::nondir, *it);
	++it;
    }

    it = entry_unknown.begin();
    while(it != entry_unknown.end())
    {
	cancellation_checkpoint();
	event_name = "x_" + to_string(count++);
	if(listed.find(event_name) != listed.end())
	    throw WEBDAR_BUG; // event already exists!?!
	add_content_entry(event_name, libdar::inode_type::unknown, *it);
	++it;
    }

}

void html_select_file::signaled_inherited_cancel()
{
    pthread_t libdar_tid;
    libdar::thread_cancellation th;

    if(is_running(libdar_tid))
	th.cancel(libdar_tid, true, 0);
}

void html_select_file::create_dir()
{
    entr->create_dir(createdir_input.get_value(), 0700);
}

void html_select_file::add_content_entry(const string & event_name, libdar::inode_type tp, const string & entry)
{
    item current;
    bool hyperlink = false;

    switch(tp)
    {
    case libdar::inode_type::isdir:
	content.adopt_static_html(" DIR ");
	hyperlink = true;
	break;
    case libdar::inode_type::symlink:
	content.adopt_static_html(" LNK ");
	hyperlink = true;
	break;
    case libdar::inode_type::nondir:
	content.adopt_static_html("");
	hyperlink = (cur_select_mode == sel_file);
	break;
    case libdar::inode_type::unknown:
	content.adopt_static_html("?");
	hyperlink = true; // because it may be a directory or symlink to a directory user would like to chdir into
	break;
    default:
	throw WEBDAR_BUG;
    }

    current.type = tp;
    if(hyperlink)
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
	should_refresh = true;
	path_loaded = ""; // this will force reloading dir content
	which_thread = val;
	loading_mode(true);
	if(is_running())
	    throw WEBDAR_BUG;
	webui.run_and_control_thread(this);
	break;
    case run_init_fill:
    case run_fill_only:
	if(fieldset.get_label() != path_loaded)
	{
	    should_refresh = true;
	    path_loaded = fieldset.get_label();
	    which_thread = val;
	    loading_mode(true);
	    if(is_running())
		throw WEBDAR_BUG;
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
    if(mode == is_loading_mode)
	return; // nothing to do

    if(mode)
    {
	    // in loading mode we must
	    // hide not only the component
	    // that are not pertinent to show
	    // but also all component generating
	    // an event from the current thread
	    // as subthread will be run and may
	    // generare event to us. Then on_event
	    // method is not designed to be re-entrant
	    // and does not need to be.
	title.set_visible(false);
	entrepot_url.set_visible(false);
	parentdir.set_visible(false);
	content.set_visible(false);
	btn_box.set_visible(false);
	webui.clear();
	webui.set_visible(true);
	content_placeholder.set_visible(true);
    }
    else
    {
	title.set_visible(true);
	entrepot_url.set_visible(true);
	parentdir.set_visible(true);
	content.set_visible(true);
	btn_box.set_visible(true);
	    // not necessary to set webui.set_visible(false) in the
	    // present condition webui has already finished and hide
	    // itself (auto_hide)

	content_placeholder.set_visible(false);
    }

    is_loading_mode = mode;
}

string html_select_file::get_parent_path(const string & somepath)
{
    chemin chem(somepath);

    try
    {
	chem.pop_back();
    }
    catch(exception_range & e)
    {
	    // pop_back() failed because we
	    // we reached the root of the file system
	    // ignoring this error and staying at
	    // the same path
    }

    return chem.display();
}

void html_select_file::my_join(bool last)
{
    try
    {
	bool ok = false;
	try
	{
	    if(last)
		set_visible(false);
	    ok = true;
	    throw ok;
	}
	catch(...)
	{
	    if(last)
	    {
		if(entr)
		{
		    entr->change_user_interaction(mem_ui);
		    entr.reset();   // forget about the go_select() provided entrepot
		}
		else
		    throw WEBDAR_BUG;

		mem_ui.reset(); // forget about the user_interaction entr had
	    }

	    if(!ok)
		throw;
	}
    }
    catch(exception_bug &)
    {
	throw;
    }
    catch(libthreadar::exception_base & e)
    {
	if(last)
	    throw;
	else
	    warning.add_text(3, e.get_message(": "));
    }
    catch(exception_base & e)
    {
	if(last)
	    throw; // cannot display the error in 'warning' is visible is now false
	else
	    warning.add_text(3, e.get_message());
    }
    catch(libdar::Egeneric & e)
    {
	if(last)
	    throw; // cannot display the error in 'warning' is visible is now false
	else
	    warning.add_text(3, e.get_message());
    }
}

