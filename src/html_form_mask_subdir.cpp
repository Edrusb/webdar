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
		    "1", // checked
		    1),
    regex("Regular expression in place of glob expression",
	  html_form_input::check,
	  "", // unchecked
	  1),
    mask_subdir("Concerned path",
		    html_form_input::text,
		    "",
		    30)
{

	// component configuration

    mask_type.add_choice("0", "Include path and subdirs (no wildcard)");
    mask_type.add_choice("1", "Exclude path and subdirs");
    mask_type.set_selected(0);

    init();
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
	e.prepend_message("Error met while creating Filename based filtering");
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
