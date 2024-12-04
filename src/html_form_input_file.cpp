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
#include "webdar_css_style.hpp"
#include "webdar_css_style.hpp"
#include "webdar_tools.hpp"

    //
#include "html_form_input_file.hpp"

using namespace std;

    // event names
const string html_form_input_file::changed_event = "html_form_input_file_changed";
const string html_form_input_file::changed_entrepot = "html_form_entrepot_changed";
const string html_form_input_file::triggered_event = "triggered";

    // class names
const string html_form_input_file::css_input = "html_form_input_file_input";
const string html_form_input_file::css_button_box = "html_form_input_file_button_box";
const string html_form_input_file::css_button_link = "html_form_input_file_button_lnk";
const string html_form_input_file::css_empty_text = "html_form_input_file_eol";


html_form_input_file::html_form_input_file(const string & label,
					   const string & initial_value,
					   const string & size,
					   const string & popup_message):
    input(label, html_form_input::text, initial_value, size),
    trigger("+", triggered_event),
    user_select(popup_message),
    empty_text(),  // must not add text now, need first to set the css_class
    changed_event_name(changed_event),
    entrep(nullptr),
    refresh_get_body(false),
    selmode(select_file),
    internal_change(false)
{
    init();

    trigger.add_css_class(css_button_box);
    trigger.url_add_css_class(css_button_link);
    input.set_no_CR();

    empty_text.add_css_class(css_empty_text);
	// we ca now add text to empty_text
    empty_text.add_text(0,"");

    entrep.reset(new (nothrow) libdar::entrepot_local("", "", false));
    if(!entrep)
	throw exception_memory();
}

html_form_input_file::html_form_input_file(const html_form_input_file & ref):
    input(ref.input),
	// input_div not copy constructable, must be set in init()
    trigger(ref.trigger),
    user_select(ref.user_select.get_message()), // no copy constructor available for this class
    empty_text(ref.empty_text),
    changed_event_name(ref.changed_event),
    entrep(ref.entrep),
    refresh_get_body(ref.refresh_get_body),
    selmode(ref.selmode),
    min_digits(ref.min_digits),
    internal_change(ref.internal_change)
{
    init();
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
	user_select.go_select(entrep, input.get_value());
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
	if(csslib->class_exists(css_button_box))
	    throw WEBDAR_BUG;
	if(csslib->class_exists(css_button_link))
	    throw WEBDAR_BUG;
	if(csslib->class_exists(css_empty_text))
	    throw WEBDAR_BUG;


	    // css_input definition

	css tmp;

	tmp.css_float(css::fl_left);
	csslib->add(css_input, tmp);

	    // css_empty_text definition

	tmp.clear();
	tmp.css_float_clear(css::fc_both);
	csslib->add(css_empty_text, tmp);

	    // css_button_box definition


	tmp.clear();
        tmp.css_color(COLOR_MENU_FRONT_OFF);
	tmp.css_background_color(COLOR_MENU_BACK_OFF);
	tmp.css_border_width(css::bd_all, css::bd_medium);
	tmp.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF);
	tmp.css_border_style(css::bd_all, css::bd_solid);
	tmp.css_text_h_align(css::al_center);
	tmp.css_width("2em", false);
	tmp.css_padding("0");
	tmp.css_margin_left("1em");
	tmp.css_float(css::fl_left);

	csslib->add(css_button_box, tmp);

	    // css_button_link definition

	css_class tmp_class(css_button_link);

	tmp.clear();
        tmp.css_color(COLOR_MENU_FRONT_OFF);
	tmp.css_background_color(COLOR_MENU_BACK_OFF);
	tmp.css_font_style_normal();
	tmp.css_font_weight_bold();
	tmp.css_text_decoration(css::dc_none);
	tmp.css_text_h_align(css::al_center);

	tmp_class.set_selector(css_class::link, tmp);
	tmp_class.set_selector(css_class::visited, tmp);

	tmp.css_color(COLOR_MENU_FRONT_HOVER_OFF, true);
	tmp.css_text_decoration(css::dc_underline, true);

	tmp_class.set_selector(css_class::hover, tmp);
	tmp_class.set_selector(css_class::active, tmp);

	csslib->add(tmp_class);
    }
    else
    {
	if(!csslib->class_exists(css_button_box))
	    throw WEBDAR_BUG;
	if(!csslib->class_exists(css_button_link))
	    throw WEBDAR_BUG;
	if(!csslib->class_exists(css_empty_text))
	    throw WEBDAR_BUG;
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


void html_form_input_file::init()
{
	// html adoption tree

    input_div.adopt(&input);
    adopt(&input_div);
    adopt(&trigger);
    adopt(&user_select);
    adopt(&empty_text);

	// cabling events

    register_name(changed_event_name);
    register_name(changed_entrepot);

    input.record_actor_on_event(this, html_form_input::changed);
    trigger.record_actor_on_event(this, triggered_event);
    user_select.record_actor_on_event(this, html_select_file::entry_selected);

	// css classes

    input_div.add_css_class(css_input);
}
