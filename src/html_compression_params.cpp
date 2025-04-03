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
#include <dar/tools.hpp>

    // webdar headers
#include "webdar_css_style.hpp"


    //
#include "html_compression_params.hpp"

using namespace std;

const string html_compression_params::changed = "html_compression_params_changed";


html_compression_params::html_compression_params(bool show_resave,
						 bool show_min_size,
						 bool show_keep_compressed):
    form_compr("Update"),
    compr_fs("Compression parameters"),
    compression("Compression algorithm"),
    compression_level("Compression level", html_form_input::number, "", "5", ""),
    min_compr_size("Minimum file sized compressed", default_min_compr_size, "30"),
    compression_block("Block compression for parallel compression (zero to zero to disable)", 0, "30"),
    compr_threads("Number of threads for parallel compression", html_form_input::number, "2", "5", ""),
    never_resave_uncompressed("Never resave uncompressed if compressed file took more place than uncompressed", html_form_input::check, "", "", ""),
    keep_compressed("Keep file compressed", html_form_input::check, "", "", ""),
    ignore_events(false),
    x_show_resave(show_resave),
    x_show_min_size(show_min_size)
{

	// component setup
    compr_threads.set_min_only(1);
    compression.set_no_CR();
    min_compr_size.set_min_only(0);

	// visibility
    if(! x_show_resave)
	never_resave_uncompressed.set_visible(false);
    if(! x_show_min_size)
	min_compr_size.set_visible(false);
    if(! show_keep_compressed)
    {
	keep_compressed.set_visible(false);
	keep_compressed.set_value_as_bool(false);
	    // needed to have other fields
	    // visible
    }

	// adoption tree
    compr_fs.adopt(&keep_compressed);
    compr_fs.adopt(&compression);
    compr_fs.adopt(&compression_level);
    compr_fs.adopt(&min_compr_size);
    compr_fs.adopt(&never_resave_uncompressed);
    compr_fs.adopt(&compression_block);
    compr_fs.adopt(&compr_threads);
    form_compr.adopt(&compr_fs);
    adopt(&form_compr);

	// events
    register_name(changed);
    compression.record_actor_on_event(this, html_compression::changed);
    keep_compressed.record_actor_on_event(this, html_form_input::changed);
    compression_block.record_actor_on_event(this, html_form_input_unit::changed);

    clear_json();
	// to set the fields to their default values
	// this also trigger html_compression::changed event
	// wich setup the visibility of components

	// css
    compression_level.add_css_class(webdar_css_style::width_100vw_8em);
    never_resave_uncompressed.add_css_class(webdar_css_style::width_100vw_8em);
    compr_threads.add_css_class(webdar_css_style::width_100vw_8em);
    keep_compressed.add_css_class(webdar_css_style::width_100vw_8em);
}

void html_compression_params::load_json(const json & source)
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
	    throw exception_range(libdar::tools_printf("Json format version too hight for %s, upgrade your webdar software",
						       myclass_id));

	    // setting back the bool mode and mask_type selected value

	ignore_events = true;
	try
	{
	    compression.set_selected_id_with_warning(config.at(jlabel_algo), jlabel_algo);
	    compression_level.set_value_as_int(config.at(jlabel_level));
	    min_compr_size.set_value_as_infinint(libdar::deci(config.at(jlabel_min_compr_sz)).computer());
	    compression_block.set_value_as_infinint(libdar::deci(config.at(jlabel_compr_block)).computer());
	    never_resave_uncompressed.set_value_as_bool(config.at(jlabel_never_resave_uncompr));
	    compr_threads.set_value_as_int(config.at(jlabel_compr_threads));
	    keep_compressed.set_value_as_bool(config.at(jlabel_keep_compr));

	    if(!keep_compressed.get_visible() && keep_compressed.get_value_as_bool())
	    {
		clear_json();
		throw exception_range("Error loading compression configuration, keep compression parameter is not applicable in that context");
	    }
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
	throw exception_json(libdar::tools_printf("Error loading %s config", myclass_id),
			     e);
    }

    on_event(html_compression::changed); // this update component visibility and trigger the change event
}

json html_compression_params::save_json() const
{
    json ret;

	// the following object field are not saved/restored as json
	// - show_resave
	// - show min_size
	// they stay local to the context the object is located in
	// However the values of the fieds they drive the visibility of:
	// - never_resave_uncompressed
	// - min_compr_size
	// are saved/restored as json

    ret[jlabel_algo] = compression.get_selected_id();
    ret[jlabel_level] = compression_level.get_value_as_int();
    ret[jlabel_min_compr_sz] = libdar::deci(min_compr_size.get_value_as_infinint()).human();
    ret[jlabel_compr_block] = libdar::deci(compression_block.get_value_as_infinint()).human();
    ret[jlabel_never_resave_uncompr] = never_resave_uncompressed.get_value_as_bool();
    ret[jlabel_compr_threads] = compr_threads.get_value_as_int();
    ret[jlabel_keep_compr] = keep_compressed.get_value_as_bool();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					ret);
}

void html_compression_params::clear_json()
{
    compression.set_selected_num(0);
    compression_level.set_value_as_int(default_level);
    min_compr_size.set_value_as_infinint(libdar::infinint(default_min_compr_size));
    compression_block.set_value_as_infinint(libdar::infinint(default_compression_block));
    never_resave_uncompressed.set_value_as_bool(default_never_resave_uncompressed);
    compr_threads.set_value_as_int(default_compr_threads);
    keep_compressed.set_value_as_bool(default_keep_compressed);

    if(!keep_compressed.get_visible()) // we thus ignore the keep_compressed value loaded from json
	keep_compressed.set_value_as_bool(false); // this drives to get visible other components

    on_event(html_compression::changed); // this update component visibility and trigger the change event
}

void html_compression_params::on_event(const string & event_name)
{
    if(ignore_events)
	return;

    if(event_name == html_compression::changed
       || event_name == html_form_input::changed
       || event_name == html_form_input_unit::changed)
    {
	if(keep_compressed.get_value_as_bool())
	{
	    compression.set_visible(false);
	    compression_level.set_visible(false);
	    min_compr_size.set_visible(false);
	    compression_block.set_visible(false);
	    never_resave_uncompressed.set_visible(false);
	    compr_threads.set_visible(false);
	}
	else
	{
	    compression.set_visible(true);

	    switch(compression.get_value())
	    {
	    case libdar::compression::none:
		compression_level.set_visible(false);
		compression.set_no_CR(false);
		if(x_show_min_size)
		    min_compr_size.set_visible(false);
		compression_block.set_visible(false);
		if(x_show_resave)
		    never_resave_uncompressed.set_visible(false);
		compr_threads.set_visible(false);
		break;
	    case libdar::compression::lzo1x_1_15:
	    case libdar::compression::lzo1x_1:
		compression.set_no_CR(false);
		compression_level.set_visible(false);
		if(x_show_min_size)
		    min_compr_size.set_visible(true);
		compression_block.set_visible(true);
		if(x_show_resave)
		    never_resave_uncompressed.set_visible(true);
		compr_threads.set_visible(true);
		break;
	    case libdar::compression::gzip:
	    case libdar::compression::bzip2:
	    case libdar::compression::lzo:
	    case libdar::compression::xz:
	    case libdar::compression::zstd:
	    case libdar::compression::lz4:
		compression.set_no_CR(true);
		compression_level.set_visible(true);
		if(compression.get_value() == libdar::compression::zstd)
		    compression_level.set_range(1, 22);
		else
		    compression_level.set_range(1, 9);
		if(x_show_min_size)
		    min_compr_size.set_visible(true);
		compression_block.set_visible(true);
		if(x_show_resave)
		    never_resave_uncompressed.set_visible(true);
		compr_threads.set_visible(true);
		break;
	    default:
		throw WEBDAR_BUG;
	    }

	    if(compression_block.get_value_as_infinint() == 0)
	    {
		compr_threads.set_value_as_int(1);
		compr_threads.set_enabled(false);
	    }
	    else
	    {
		compr_threads.set_enabled(true);
	    }
	}

	act(changed);
    }
    else
	throw WEBDAR_BUG;
}

void html_compression_params::set_to_webdar_defaults()
{
    clear_json();

    keep_compressed.set_value_as_bool(false);
    compression.set_value(libdar::compression::zstd);
    compression_level.set_value_as_int(9);
    min_compr_size.set_value_as_infinint(1024);
    never_resave_uncompressed.set_value_as_bool(false);
}

string html_compression_params::inherited_get_body_part(const chemin & path,
							const request & req)
{
    return get_body_part_from_all_children(path, req);
}

void html_compression_params::css_classes_have_changed()
{
    set<string> css_classes = get_css_classes_as_a_set();

    form_compr.clear_css_classes();
    for(set<string>::iterator it = css_classes.begin();
	it != css_classes.end();
	++it)
	form_compr.add_css_class(*it);
}

void html_compression_params::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}
