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
#include "html_form_mask_subdir.hpp"

using namespace std;


html_form_mask_subdir::html_form_mask_subdir(bool absolute_path_accepted):
    absolute_ok(absolute_path_accepted),
    prefix(libdar::FAKE_ROOT),
    fs(""),
    mask_type("Mask Type"),
    casesensitivity("Case Sensitive",
		    html_form_input::check,
		    "", // see clear() method for defaults
		    "1"),
    regex("Regular expression in place of glob expression",
	  html_form_input::check,
	  "", // see clear() method for defaults
	  "1"),
    mask_subdir("Concerned path",
		html_form_input::text,
		"",
		"80%")
{

	// component configuration

    mask_type.add_choice("0", "Include path and subdirs (no wildcard)");
    mask_type.add_choice("1", "Exclude path and subdirs");

    init();
    clear();
}

html_form_mask_subdir::html_form_mask_subdir(const html_form_mask_subdir & ref):
    html_mask(ref), // parent class
    absolute_ok(ref.absolute_ok),
    prefix(ref.prefix),
    fs(ref.fs),
    mask_type(ref.mask_type),
    casesensitivity(ref.casesensitivity),
    regex(ref.regex),
    mask_subdir(ref.mask_subdir)
{
    init();
}

void html_form_mask_subdir::clear()
{
	// absolute_ok is not changed
    prefix = libdar::FAKE_ROOT;
    mask_type.set_selected(0);
    casesensitivity.set_value_as_bool(true);
    regex.set_value_as_bool(false);
    mask_subdir.set_value("");
}

void html_form_mask_subdir::set_root_prefix(const libdar::path & x_prefix)
{
    prefix = x_prefix;
    fs.change_label(tell_action());
    my_body_part_has_changed();
};


unique_ptr<libdar::mask> html_form_mask_subdir::get_mask() const
{
    bool casesensit = casesensitivity.get_value_as_bool();
    unique_ptr<libdar::mask> ret;
    libdar::path pathval("/");

    try
    {
	pathval = libdar::path(mask_subdir.get_value());
    }
    catch(libdar::Egeneric & e)
    {
	e.prepend_message("Error met while creating file name based filtering");
	throw exception_libcall(e);
    }

    switch(mask_type.get_selected_num())
    {
    case 0: // Include path
	ret.reset(new (nothrow) libdar::simple_path_mask(prefix + pathval,
							 casesensit));
	break;
    case 1: // Exclude path
	if(! regex.get_value_as_bool())
	{
		// glob expression
	    libdar::et_mask* tmp = new (nothrow) libdar::et_mask();

	    ret.reset(tmp);
		// the object is now managed by the shared_ptr but we
		// still have tmp as a more specific pointer type on it
	    if(!ret || tmp == nullptr)
		throw exception_memory();

	    tmp->add_mask(libdar::not_mask(libdar::simple_mask((prefix + pathval).display(), casesensit)));
	    tmp->add_mask(libdar::not_mask(libdar::simple_mask((prefix + pathval).display() + "/*", casesensit)));
	}
	else
	{
		// regular expression (temporary variable used for debugging purposes if needed)
	    string tmp = libdar::tools_build_regex_for_exclude_mask(prefix.display(), mask_subdir.get_value());

	    ret.reset(new (nothrow) libdar::not_mask(libdar::regular_mask(tmp, casesensit)));
	}
	break;
    default:
	throw WEBDAR_BUG;
    }

    if(!ret)
	throw exception_memory();

    return ret;
}

void html_form_mask_subdir::load_json(const json & source)
{
    try
    {
	unsigned int version;
	string class_id;
	json config = unwrap_config_from_json_header(source,
						     version,
						     class_id);

	if(class_id != "html_form_mask_subdir")
	    throw exception_range(libdar::tools_printf("Unexpected class_id in json data, found %s while expecting html_form_mask_subdir",
						       class_id.c_str()));

	if(version > format_version)
	    throw exception_range("Json format version too hight for html_form_mask_subdir, upgrade your webdar software");

	absolute_ok = config.at(jlabel_absolute);
	prefix = libdar::path(config.at(jlabel_prefix));
	mask_type.set_selected(config.at(jlabel_type).template get<string>());
	casesensitivity.set_value_as_bool(config.at(jlabel_casesensit));
	regex.set_value_as_bool(config.at(jlabel_regex));
	mask_subdir.set_value(config.at(jlabel_mask));
    }
    catch(json::exception & e)
    {
	throw exception_json("Error loading html_form_mask_expression config", e);
    }
}

json html_form_mask_subdir::save_json() const
{
    json ret;

    ret[jlabel_absolute] = absolute_ok;
    ret[jlabel_prefix] = prefix.display();
    ret[jlabel_type] = mask_type.get_selected_id();
    ret[jlabel_casesensit] = casesensitivity.get_value_as_bool();
    ret[jlabel_regex] = regex.get_value_as_bool();
    ret[jlabel_mask] = mask_subdir.get_value();

    return wrap_config_with_json_header(format_version,
					"html_form_mask_subdir",
					ret);
}

void html_form_mask_subdir::on_event(const std::string & event_name)
{
    if(event_name == html_form_select::changed)
    {
	regex.set_visible(mask_type.get_selected_num() == 1);
	    // regex is only visible if excluding a path
	fs.change_label(tell_action());
    }
    else if(event_name == html_form_input::changed)
    {
	fs.change_label(tell_action());
    }
    else
	throw WEBDAR_BUG;
}

string html_form_mask_subdir::inherited_get_body_part(const chemin & path,
						      const request & req)
{
    string ret = get_body_part_from_all_children(path, req);
    if(!absolute_ok)
    {
	string val = mask_subdir.get_value();

	if(! val.empty())
	{
	    libdar::path valpath = libdar::path(val);

	    if(valpath.is_absolute())
	    {
		if(!mask_subdir.has_css_class(webdar_css_style::wcs_red_border))
		{
		    mask_subdir.add_css_class(webdar_css_style::wcs_red_border);
		    throw exception_range("absolute path are not allowed for path filtering in that context");
		}
	    }
	    else
	    {
		if(mask_subdir.has_css_class(webdar_css_style::wcs_red_border))
		    mask_subdir.remove_css_class(webdar_css_style::wcs_red_border);
	    }
	}
	else
	{
	    if(mask_subdir.has_css_class(webdar_css_style::wcs_red_border))
		mask_subdir.remove_css_class(webdar_css_style::wcs_red_border);
	}
    }
    else
    {
	if(mask_subdir.has_css_class(webdar_css_style::wcs_red_border))
	    mask_subdir.remove_css_class(webdar_css_style::wcs_red_border);
    }

    return ret;
}

void html_form_mask_subdir::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}

void html_form_mask_subdir::init()
{
	// adoption tree

    fs.adopt(&mask_type);
    fs.adopt(&casesensitivity);
    fs.adopt(&regex);
    fs.adopt(&mask_subdir);
    adopt(&fs);
    fs.change_label(tell_action());

	// events
    mask_type.record_actor_on_event(this, html_form_select::changed);
    casesensitivity.record_actor_on_event(this, html_form_input::changed);
    regex.record_actor_on_event(this, html_form_input::changed);
    mask_subdir.record_actor_on_event(this, html_form_input::changed);

	// visibity
    regex.set_visible(mask_type.get_selected_num() == 1);

	// css stuff
    fs.add_label_css_class(webdar_css_style::wcs_bold_text);
}


string html_form_mask_subdir::tell_action() const
{
    string ret = "";

    switch(mask_type.get_selected_num())
    {
    case 0:
	ret += "path is or is a subdir of ";
	break;
    case 1:
	ret += "path does not match ";
	if(regex.get_value_as_bool())
	    ret += "the regular expression ";
	else
	    ret += "the glob expression ";
	break;
    default:
	throw WEBDAR_BUG;
    }

    ret += "\"";
    if(! mask_subdir.get_value().empty())
    {
	try
	{
	    ret += (prefix + libdar::path(mask_subdir.get_value())).display();
	}
	catch(libdar::Egeneric & e)
	{
	    e.prepend_message("Error met while evaluation path base filtering: ");
	    throw exception_libcall(e);
	}
    }

    ret += "\" ";

    if(casesensitivity.get_value_as_bool())
	ret += "(case sensitive)";
    else
	ret += "(case INsensitive)";

    return ret;
}
