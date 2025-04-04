/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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
#include "webdar_css_style.hpp"
#include "webdar_tools.hpp"

    //
#include "html_form_input_file.hpp"

using namespace std;

    // event names
const string html_form_input_file::changed_event = "html_form_input_file_changed";
const string html_form_input_file::changed_entrepot = "html_form_entrepot_changed";
const string html_form_input_file::repo_update_needed = "hfif_need_repo_update";
const string html_form_input_file::repo_updated = "hfif_repo_has_been_updated";
const string html_form_input_file::triggered_event = "triggered";

    // class names
const string html_form_input_file::css_input = "html_form_input_file_input";
const string html_form_input_file::css_button_box = "html_form_input_file_button_box";

html_form_input_file::html_form_input_file(const string & label,
					   const string & initial_value,
					   const string & size,
					   const string & css_class,
					   const string & popup_message):
    input(label, html_form_input::text, initial_value, size, webdar_css_style::width_100vw),
    trigger("+", triggered_event),
    user_select(popup_message),
    empty_text(),  // must not add text now, need first to set the css_class
    changed_event_name(changed_event),
    entrep(nullptr),
    refresh_get_body(false),
    selmode(select_file),
    internal_change(false),
    repo_updater(nullptr)
{
	// components setup

    entrep.reset(new (nothrow) libdar::entrepot_local("", "", false));
    if(!entrep)
	throw exception_memory();
    empty_text.add_text(0,"");

	// html adoption tree

    input_div.adopt(&input);
    adopt(&input_div);
    adopt(&trigger);
    adopt(&user_select);
    adopt(&empty_text);

	// cabling events

    register_name(changed_event_name);
    register_name(changed_entrepot);
    register_name(repo_update_needed);
	// we do not register repo_updated because this is the object given to set_entrepot_updater() that trigger those events

    input.record_actor_on_event(this, html_form_input::changed);
    trigger.record_actor_on_event(this, triggered_event);
    user_select.record_actor_on_event(this, html_select_file::entry_selected);

	// css

    add_css_class(css_class); // for the whole component
    input_div.add_css_class(css_input);
    webdar_css_style::small_button(trigger);
    trigger.add_css_class(css_button_box);
    input.set_no_CR();
    empty_text.add_css_class(webdar_css_style::float_flush);
}

void html_form_input_file::set_change_event_name(const string & name)
{
    rename_name(changed_event_name, name);
    changed_event_name = name;
}

void html_form_input_file::set_select_mode(select_mode val)
{
    switch(val)
    {
    case select_dir:
	user_select.set_select_mode(html_select_file::sel_dir);
	break;
    case select_file:
	user_select.set_select_mode(html_select_file::sel_file);
	user_select.set_filter("");
	break;
    case select_slice:
	user_select.set_select_mode(html_select_file::sel_file);
	user_select.set_filter("*.dar");
	break;
    case select_symlink:
	user_select.set_select_mode(html_select_file::sel_symlinks);
	user_select.set_filter("");
	break;
    default:
	throw WEBDAR_BUG;
    }
    min_digits.clear();
    selmode = val;
};

void html_form_input_file::set_entrepot(shared_ptr<libdar::entrepot> entrepot)
{
    if(!entrepot)
	throw WEBDAR_BUG;
    entrep = entrepot;
    act(changed_entrepot);
}

void html_form_input_file::set_entrepot_updater(events* updater)
{
    repo_updater = updater;

    if(repo_updater != nullptr)
	repo_updater->record_actor_on_event(this, repo_updated);
};

string html_form_input_file::get_min_digits() const
{
    if(selmode != select_slice)
	throw WEBDAR_BUG;

    return min_digits;
}

void html_form_input_file::on_event(const string & event_name)
{
    if(event_name == html_form_input::changed)
    {
	if(selmode == select_slice && !internal_change)
	{
	    min_digits.clear();
		// this is to avoid having min-digits
		// changed in the parent object to something
		// that would not be related to the new
		// value the user has set manually in the
		// html form. The user might have well also
		// manually set the min-digit field just before
	}

	act(changed_event_name);
    }
    else if(event_name == triggered_event)
    {
	if(repo_updater == nullptr)
	    user_select.go_select(entrep, input.get_value());
	else
	    act(repo_update_needed);
    }
    else if(event_name == html_select_file::entry_selected)
    {
	switch(selmode)
	{
	case select_dir:
	case select_file:
	case select_symlink:
	    input.set_value(user_select.get_selected_path());
	    break;
	case select_slice:
	    internal_change = true; // avoid html_form_input::change to reset min_digits
	    try
	    {
		input.set_value(slicename_to_basename_update_min_digits(user_select.get_selected_path()));
	    }
	    catch(...)
	    {
		internal_change = false;
		throw;
	    }
	    internal_change = false;
	    break;
	default:
	    throw WEBDAR_BUG;
	}
	refresh_get_body = true;
	my_body_part_has_changed(); // needed also to remove the selection popup
	act(changed_event_name);
    }
    else if(event_name == repo_updated)
	user_select.go_select(entrep, input.get_value());
    else
	throw WEBDAR_BUG;
}

void html_form_input_file::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();

    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);

    if(!csslib->class_exists(css_input))
    {
	css tmp;

	    // css_button_box definition (for trigger button)

	tmp.clear();
	tmp.css_float(css::fl_left);
	csslib->add(css_button_box, tmp);

	    // css for input_div

	tmp.clear();
	tmp.css_width("calc(100% - 3em)",false); // 2em for the trigger button plus more for the spacing around
	tmp.css_float(css::fl_left);
	csslib->add(css_input, tmp);
    }
}

string html_form_input_file::inherited_get_body_part(const chemin & path,
						     const request & req)
{
    string ret;
    refresh_get_body = false;

    ret = html_div::inherited_get_body_part(path, req);
    if(refresh_get_body)
    {
	request tmp = req;
	tmp.post_to_get();
	ret = html_div::inherited_get_body_part(path, tmp);
    }

    return ret;
}

string html_form_input_file::slicename_to_basename_update_min_digits(const string & val)
{
    static const char sep = '.';
    vector<string> splitted;

    min_digits.clear();
    webdar_tools_split_by(sep, val, splitted);

    if(splitted.size() < 3)
	return val; // given arg is not a slicename

    if(splitted[splitted.size() - 1] != "dar")
	return val; // not a dar slice

    string & tmp = splitted[splitted.size() - 2];
    if(tmp.empty())
	return val; // not a dar slice

    string::iterator it = tmp.begin();
    while(it != tmp.end() && *it >= '0' && *it <= '9')
	++it;
    if(it != tmp.end()) // not a slice number found before the dar extension
	return val;

    if(tmp.begin() != tmp.end()
       && *(tmp.begin()) == '0')
	min_digits = webdar_tools_convert_to_string(tmp.size());
    else
	min_digits = "0";

	// we have a well formated slice name

    string ret = splitted[0];
    unsigned int stopat = splitted.size() - 2;
    const string sep_str(webdar_tools_convert_to_string(sep));


    for(unsigned int i = 1; i < stopat ; ++i)
	ret += sep_str + splitted[i];

    return ret;
}
