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
#include <dar/libdar.hpp>

    // webdar headers
#include "webdar_css_style.hpp"


    //
#include "html_form_mask_file.hpp"

using namespace std;

html_form_mask_file::html_form_mask_file():
    filename("File name", "/", 30, "Select a listing text file..."),
    exclude_checkbox("Exclude listed files",
		     html_form_input::check,
		     "", // unchecked
		     1),
    casesensit("Case sensitivivity",
	       html_form_input::check,
	       "1", // checked
	       1),
    fs("File listing"),
    prefix(libdar::FAKE_ROOT)
{
    init();
}

html_form_mask_file::html_form_mask_file(const html_form_mask_file & ref):
    filename(ref.filename),
    exclude_checkbox(ref.exclude_checkbox),
    casesensit(ref.casesensit),
    fs(ref.fs),
    prefix(ref.prefix)
{

    init();
}

unique_ptr<libdar::mask> html_form_mask_file::get_mask() const
{
    deque<string> empty_list;
    unique_ptr<libdar::mask> tmp(new (nothrow) libdar::mask_list(
				     filename.get_value(),
				     casesensit.get_value_as_bool(),
				     prefix,
				     ! exclude_checkbox.get_value_as_bool(),
				     empty_list));

    if(! tmp)
	throw exception_memory();

    if(exclude_checkbox.get_value_as_bool())
    {
	tmp.reset(new (nothrow) libdar::not_mask(*tmp));
	if(! tmp)
	    throw exception_memory();
    }

    return tmp;
}

string html_form_mask_file::inherited_get_body_part(const chemin & path,
						    const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_form_mask_file::on_event(const std::string & event_name)
{
    if(event_name == html_form_input_file::changed_event
       || event_name == html_form_input::changed)
    {
	fs.change_label(tell_action());
    }
    else
	throw WEBDAR_BUG;
}

void html_form_mask_file::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}


void html_form_mask_file::init()
{
	// adoption tree

    fs.adopt(&filename);
    fs.adopt(&exclude_checkbox);
    fs.adopt(&casesensit);
    adopt(&fs);
    fs.change_label(tell_action());

	// events
    filename.record_actor_on_event(this, html_form_input_file::changed_event);
    exclude_checkbox.record_actor_on_event(this, html_form_input::changed);
    casesensit.record_actor_on_event(this, html_form_input::changed);

	// css
    fs.add_label_css_class(webdar_css_style::wcs_bold_text);

}


string html_form_mask_file::tell_action() const
{
    string ret = "path ";

    if(exclude_checkbox.get_value_as_bool())
	ret += "is not listed ";
    else
	ret += "is listed ";

    if(casesensit.get_value_as_bool())
	ret += "(case sensitive) ";
    else
	ret += "(case INsensitive) ";
    ret += "in file named \"" + filename.get_value() + "\" ";

    return ret;
}
