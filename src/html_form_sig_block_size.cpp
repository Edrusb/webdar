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
#include <dar/tools.hpp>

    // webdar headers
#include "webdar_css_style.hpp"
#include "webdar_tools.hpp"

    //
#include "html_form_sig_block_size.hpp"

using namespace std;

const string html_form_sig_block_size::changed = "hfsbs_changed";

html_form_sig_block_size::html_form_sig_block_size():
    delta_sig_min_size("Avoid calculating delta signature for file smaller than", 0, "30"),
    fs_function("Function used to derive delta signature block size from the file size to sign"),
    multiply("Multicative factor", html_form_input::number, "1", "10", ""),
    divisor("Divisor factor", html_form_input::number, "1", "10", ""),
    min_size("Minimum block size", 1, "10"),
    max_size("Maximum block size (set to zero to disable max size)", 1, "10"),
    ignore_events(false)
{

	// components setup
    function.add_choice("fixed","Fixed value of 1");
    function.add_choice("linear", "File size (linear)");
    function.add_choice("log2", "Natural logarithm of the file size");
    function.add_choice("root2", "Square root of the file size");
    function.add_choice("root3", "Cube root of the file size");
    multiply.set_min_only(1);
    divisor.set_min_only(1);
    min_size.set_min_only(libdar::infinint(RS_DEFAULT_BLOCK_LEN));
    max_size.set_min_only(0);

	// adoption tree
    adopt(&delta_sig_min_size);
    adopt(&summary_f);
    adopt(&summary_l);
    fs_function.adopt(&function);
    adopt(&fs_function);
    adopt(&multiply);
    adopt(&divisor);
    adopt(&min_size);
    adopt(&max_size);

	// events
    delta_sig_min_size.record_actor_on_event(this, html_form_input_unit::changed);
    function.record_actor_on_event(this, html_form_radio::changed);
    multiply.record_actor_on_event(this, html_form_input::changed);
    divisor.record_actor_on_event(this, html_form_input::changed);
    min_size.record_actor_on_event(this, html_form_input_unit::changed);
    max_size.record_actor_on_event(this, html_form_input_unit::changed);
    register_name(changed);

	// initial value settings
    clear_json();

	// csss
    summary_f.add_css_class(webdar_css_style::text_bold);
    multiply.add_css_class(webdar_css_style::width_100vw_8em);
    divisor.add_css_class(webdar_css_style::width_100vw_8em);
}

void html_form_sig_block_size::set_value(const libdar::delta_sig_block_size & val)
{
    switch(val.fs_function)
    {
    case libdar::delta_sig_block_size::fixed:
	function.set_selected_num(0);
	break;
    case libdar::delta_sig_block_size::linear:
	function.set_selected_num(1);
	break;
    case libdar::delta_sig_block_size::log2:
	function.set_selected_num(2);
	break;
    case libdar::delta_sig_block_size::root2:
	function.set_selected_num(3);
	break;
    case libdar::delta_sig_block_size::root3:
	function.set_selected_num(4);
	break;
    default:
	throw WEBDAR_BUG;
    }

    multiply.set_value_as_int(webdar_tools_convert_from_infinint<int>(val.multiplier, string("Error while setting min size")));
    divisor.set_value_as_int(webdar_tools_convert_from_infinint<int>(val.divisor, string("Error while setting max size")));
    min_size.set_value_as_infinint(libdar::infinint(val.min_block_len));
    max_size.set_value_as_infinint(libdar::infinint(val.max_block_len));
}

libdar::delta_sig_block_size html_form_sig_block_size::get_value() const
{
    libdar::delta_sig_block_size ret;

    switch(function.get_selected_num())
    {
    case 0:
	ret.fs_function = libdar::delta_sig_block_size::fixed;
	break;
    case 1:
	ret.fs_function = libdar::delta_sig_block_size::linear;
	break;
    case 2:
	ret.fs_function = libdar::delta_sig_block_size::log2;
	break;
    case 3:
	ret.fs_function = libdar::delta_sig_block_size::root2;
	break;
    case 4:
	ret.fs_function = libdar::delta_sig_block_size::root3;
	break;
    default:
	throw WEBDAR_BUG;
    }

    ret.multiplier = libdar::infinint(multiply.get_value_as_int());
    ret.divisor = libdar::infinint(divisor.get_value_as_int());
    ret.min_block_len = webdar_tools_convert_from_infinint<libdar::U_I>(min_size.get_value_as_infinint(), string("Error while converting min size"));
    ret.max_block_len = webdar_tools_convert_from_infinint<libdar::U_I>(max_size.get_value_as_infinint(), string("Error while converting max size"));

    return ret;
}

void html_form_sig_block_size::load_json(const json & source)
{
    try
    {
	unsigned int version;
	string class_id;
	json config = unwrap_config_from_json_header(source,
						     version,
						     class_id);

	if(class_id != myclass_id)
	    throw exception_range(libdar::tools_printf("Unexpected class_id in json data, found %s while expecting %s",
						       class_id.c_str(),
						       myclass_id));

	if(version > format_version)
	    throw exception_range(libdar::tools_printf("Json format version too hight for %s, upgrade your webdar software", myclass_id));

	ignore_events = true;
	try
	{
	    delta_sig_min_size.set_value_as_infinint(libdar::deci(config.at(jlabel_min_file_size)).computer());
	    function.set_selected_id_with_warning(config.at(jlabel_function), jlabel_function);
	    multiply.set_value_as_int(config.at(jlabel_multiply));
	    divisor.set_value_as_int(config.at(jlabel_divisor));
	    min_size.set_value_as_infinint(libdar::deci(config.at(jlabel_min_size)).computer());
	    max_size.set_value_as_infinint(libdar::deci(config.at(jlabel_max_size)).computer());
	}
	catch(...)
	{
	    ignore_events = false;
	    throw;
	}
	ignore_events = false;

	on_event(html_form_input::changed);
    }
    catch(json::exception & e)
    {
	throw exception_json(libdar::tools_printf("Error loading %s config", myclass_id), e);
    }
}

json html_form_sig_block_size::save_json() const
{
    json ret;

    ret[jlabel_min_file_size] = libdar::deci(delta_sig_min_size.get_value_as_infinint()).human();
    ret[jlabel_function] = function.get_selected_id();
    ret[jlabel_multiply] = multiply.get_value_as_int();
    ret[jlabel_divisor] = divisor.get_value_as_int();
    ret[jlabel_min_size] = libdar::deci(min_size.get_value_as_infinint()).human();
    ret[jlabel_max_size] = libdar::deci(max_size.get_value_as_infinint()).human();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					ret);

}

void html_form_sig_block_size::clear_json()
{
    ignore_events = true;
    try
    {
	delta_sig_min_size.set_value_as_infinint(0);
	function.set_selected_id("root2");
	min_size.set_value_as_infinint(libdar::infinint(RS_DEFAULT_BLOCK_LEN));
	max_size.set_value_as_infinint(libdar::infinint(64*RS_DEFAULT_BLOCK_LEN));
    }
    catch(...)
    {
	ignore_events = false;
	throw;
    }
    ignore_events = false;

    on_event(html_form_input::changed);
}

void html_form_sig_block_size::on_event(const string & event_name)
{
    if(ignore_events)
	return;

    if(event_name == html_form_input::changed
       || event_name == html_form_radio::changed
       || event_name == html_form_input_unit::changed)
    {
	    // check that min_size <= max_size

	libdar::infinint min = min_size.get_value_as_infinint();
	libdar::infinint max = max_size.get_value_as_infinint();

	if(max < min && ! max.is_zero())
	{
	    min_size.box_set_css_class(webdar_css_style::red_border);
	    max_size.box_set_css_class(webdar_css_style::red_border);

	    throw exception_range("Minimum block size should be less or equal than maximum block size");
	}
	else
	{
	    min_size.box_set_css_class("");
	    max_size.box_set_css_class("");
	}

	make_summary();
	act(changed);
    }
    else
	throw WEBDAR_BUG;
}

string html_form_sig_block_size::inherited_get_body_part(const chemin & path,
							 const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_form_sig_block_size::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}


void html_form_sig_block_size::make_summary()
{
    int multi = multiply.get_value_as_int();
    int divi = divisor.get_value_as_int();
    string ratio;

    summary_f.clear();
    summary_f.add_nl();
    summary_f.add_text(0, "block_size = ");

    if(divi != multi)
    {
	    // yes we convert back to string the int obtained from string,
	    // this gives more accuracy and show possible error of conversion
	    // to the user:
	ratio = webdar_tools_convert_to_string(multi);
	if(divi > 1)
	    ratio += " / " + webdar_tools_convert_to_string(divi);
	ratio += + " ";
    }
    // else we ignore divisor and multiply as their resulting ratio is 1

    if(function.get_selected_num() == 0)
    {
	if(ratio.empty()) // the ratio is 1, thus we use the min_size
	    summary_f.add_text(0, min_size.get_raw_value() + " " + min_size.get_unit_string());
	else // need to find the max from min_size*min_unit and ratio
	{
	    libdar::infinint minval = min_size.get_value_as_infinint();
	    libdar::infinint maxval = max_size.get_value_as_infinint();
	    libdar::infinint i_mult = libdar::infinint(multi);
	    libdar::infinint i_divi = libdar::infinint(divi);

	    if(i_mult < minval * i_divi) // thus: i_mult/i_divi < minval
		summary_f.add_text(0, min_size.get_raw_value() + " " + min_size.get_unit_string());
	    else
		if(i_mult < maxval * i_divi) // this i_mult/i_divi < maxval
		    summary_f.add_text(0, ratio);
		else
		    summary_f.add_text(0, max_size.get_raw_value() + " " + max_size.get_unit_string() + " o");
	}
	summary_f.add_nl();
	summary_f.add_nl();
	summary_l.set_visible(false);
    }
    else
    {
	string func;

	switch(function.get_selected_num())
	{
	case 0: // fixed
	    throw WEBDAR_BUG; // treated appart just above
	case 1: // linear
	    func = "filesize";
	    break;
	case 2: // log2
	    func = "log<sub>2</sub>(filesize)";
	    break;
	case 3: // root2
	    func = "&radic;<span style=\"text-decoration:overline;\">filesize</span>";
	    break;
	case 4: // root3
	    func = "<sup>3</sup>&radic;<span style=\"text-decoration:overline;\">filesize</span>";
	    break;
	default:
	    throw WEBDAR_BUG;
	}

	if(!ratio.empty())
	    summary_f.add_text(0, ratio + " * ");
	summary_f.add_text(0, func);

	summary_l.clear();
	summary_l.add_nl();
	summary_l.add_text(0, "with");
	summary_l.add_text(0,
			 min_size.get_raw_value()
			 + " "
			 + min_size.get_unit_string()
			 + " &le; block_size");
	if(! max_size.get_value_as_infinint().is_zero())
	    summary_l.add_text(0,
			 " &le; "
			 + max_size.get_raw_value()
			 + " "
			 + max_size.get_unit_string());
	summary_l.add_nl();
	summary_l.add_nl();
	summary_l.set_visible(true);
    }
}
