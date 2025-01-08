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
#include <dar/tools.hpp>

    // webdar headers
#include "webdar_css_style.hpp"


    //
#include "html_form_mask_file.hpp"

using namespace std;

html_form_mask_file::html_form_mask_file(const std::shared_ptr<const libdar::path> & fs_root):
    filename("File name",
	     "/",   // see clear() for defaults
	     "80%",
	     "Select a listing text file..."),
    exclude_checkbox("Exclude listed files",
		     html_form_input::check,
		     "", // see clear() for defaults
		     "1"),
    casesensit("Case sensitivivity",
	       html_form_input::check,
	       "1", // see clear() for defaults
	       "1"),
    fs("File listing"),
    prefix(fs_root)
{
    init();
    clear();
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

void html_form_mask_file::clear()
{
    filename.set_value("/");
    exclude_checkbox.set_value_as_bool(false);
    casesensit.set_value_as_bool(true);
}

unique_ptr<libdar::mask> html_form_mask_file::get_mask() const
{
    if(!prefix)
	throw WEBDAR_BUG;

    deque<string> empty_list;
    unique_ptr<libdar::mask> tmp(new (nothrow) libdar::mask_list(
				     filename.get_value(),
				     casesensit.get_value_as_bool(),
				     *prefix,
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

void html_form_mask_file::load_json(const json & source)
{
    try
    {
	unsigned int version;
	string class_id;
	json config = unwrap_config_from_json_header(source,
						     version,
						     class_id);

	if(class_id != "html_form_mask_file")
	    throw exception_range(libdar::tools_printf("Unexpected class_id in json data, found %s while expecting html_form_mask_file",
						       class_id.c_str()));

	if(version > format_version)
	    throw exception_range("Json format version too hight for html_form_mask_file, upgrade your webdar software");

	filename.set_value(config.at(jlabel_filename));
	exclude_checkbox.set_value_as_bool(config.at(jlabel_exclude));
	casesensit.set_value_as_bool(config.at(jlabel_casesensit));
    }
    catch(json::exception & e)
    {
	throw exception_json("Error loading html_form_mask_expression config", e);
    }
}

json html_form_mask_file::save_json() const
{
    json ret;

    ret[jlabel_filename] = filename.get_value();
    ret[jlabel_exclude] = exclude_checkbox.get_value_as_bool();
    ret[jlabel_casesensit] = casesensit.get_value_as_bool();

    return wrap_config_with_json_header(format_version,
					"html_form_mask_file",
					ret);
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

string html_form_mask_file::inherited_get_body_part(const chemin & path,
						    const request & req)
{
    return get_body_part_from_all_children(path, req);
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
