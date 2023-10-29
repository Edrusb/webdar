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

}

    // C++ system header files
#include <utility>

    // webdar headers
#include "webdar_css_style.hpp"

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
    select_dir(false),
    title(2, message),
    warning(3, ""),
    fieldset(""),
    parentdir("Parent Directory", op_chdir_parent),
    content(2),
    btn_cancel("Cancel", op_cancelled),
    btn_validate("Select", entry_selected),
    btn_createdir("New Folder", op_createdir),
    ignore_events(false),
    fieldset_isdir(true)
{
    entr.reset();       // entr points to nothing
    set_visible(false); // make us invisible until run() is called
    ack_visible();      // and invisible immediately (acknowlegment)

	// events for callers objects
    register_name(entry_selected);  // we'll propagate the even from btn_validate
    register_name(op_cancelled);    // we will propagate the even from btn_cancel

	// events we want to act on from our own html_button
    btn_cancel.record_actor_on_event(this, op_cancelled);
    btn_validate.record_actor_on_event(this, entry_selected);
    parentdir.record_actor_on_event(this, op_chdir_parent);
    btn_createdir.record_actor_on_event(this, op_createdir);

	// setting up the adoption tree (the fixed part)

    adopt(&title_box);
    title_box.adopt(&title);
    title_box.adopt(&warning);
    adopt(&fieldset);
    fieldset.adopt(&parentdir);
    fieldset.adopt(&content);
    adopt(&btn_box);
    btn_box.adopt(&btn_createdir);
    btn_box.adopt(&btn_validate);
    btn_box.adopt(&btn_cancel);

    title_box.add_css_class(css_sticky_top);
    warning.add_css_class(css_warning);
    webdar_css_style::normal_button(btn_cancel);
    webdar_css_style::normal_button(btn_validate);
    webdar_css_style::normal_button(btn_createdir);
    btn_box.add_css_class(css_sticky_bot);
    btn_cancel.add_css_class(css_float_button_right);
    btn_validate.add_css_class(css_float_button_right);
    btn_createdir.add_css_class(css_float_button_left);

}

void html_select_file::run(const shared_ptr<libdar::entrepot> & x_entr,
			   const std::string & start_dir)
{
    switch(status)
    {
    case st_init:
	break;
    case st_run:
	return; // already running
    case st_completed:
	throw WEBDAR_BUG; // previous value has not been red
    default:
	throw WEBDAR_BUG;
    }

    if(libdar::path(start_dir).is_relative())
	throw WEBDAR_BUG;

    set_visible(true);
    ack_visible();
    status = st_run;
    entr = x_entr;
    fieldset.change_label(start_dir);
    fill_content();
};

void html_select_file::on_event(const std::string & event_name)
{
    if(ignore_events)
	return;

    if(event_name == entry_selected)
    {
	if(!select_dir && fieldset_isdir)
	    warning.add_text(3, string("This is a directory, please select a non-directory file"));
	else
	{
	    status = st_completed;
	    set_visible(false);
	    ack_visible();
	    act(entry_selected); // propagate the event to object that subscribed to us
	    entr.reset();
	}
    }
    else if(event_name == op_cancelled)
    {
	status = st_init;
	set_visible(false);
	ack_visible();
	act(op_cancelled);
	entr.reset();
    }
    else if(event_name == op_chdir_parent)
    {
	libdar::path chem(fieldset.get_label());
	string tmp;

	if(!chem.pop(tmp))
	    warning.add_text(3, string("Cannot go change to parent directory of ") + fieldset.get_label());

	fieldset.change_label(chem.display());
	fieldset_isdir = true;
	fill_content();
    }
    else if(event_name == op_createdir)
    {
	    // to be implemented
	throw exception_feature("create directory in entrepot");
    }
    else
    {
	map<string, item>::iterator it = listed.find(event_name);
	libdar::path curdir = libdar::path(fieldset.get_label());

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

	    if(! curdir.pop(prev_file))
		throw WEBDAR_BUG; // should be a popable path
	}

		// we concatenate (as a path subdir) the current path with the filename the user has clicked on:

	fieldset.change_label(curdir.append((it->second.btn)->get_label())
			            .display());
	fieldset_isdir = it->second.isdir;
    }

}

string html_select_file::inherited_get_body_part(const chemin & path,
						 const request & req)
{
    if(get_visible())
    {
	string ret;

	html_popup::inherited_get_body_part(path, req); // a first time to have the even triggering parameter changes
	if(entr) // when user finally selects the path, the button event destroys the entrepot, fill_content() cannot work anymore and is not needed neither
	    fill_content();                             // now we can setup the possibly new content and destroy the html_button that triggered an event
	ignore_events = true;

	try
	{
	    ret = html_popup::inherited_get_body_part(path, req);
	}
	catch(...)
	{
	    ignore_events = false;
	    throw;
	}
	ignore_events = false;

	warning.clear();
	return ret;
    }
    else
	return "";
}

void html_select_file::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    css tmp;

    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);

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

void html_select_file::fill_content()
{
    string entry;
    bool isdir;
    string event_name;
    unsigned int count = 0;
    item current;

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
	    event_name = "x_" + to_string(count++);
	    if(listed.find(event_name) != listed.end())
		throw WEBDAR_BUG; // event already exists!?!
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
