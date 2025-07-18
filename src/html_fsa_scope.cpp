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
#include "tooltip_messages.hpp"


    //
#include "html_fsa_scope.hpp"

using namespace std;

const string html_fsa_scope::changed = "hfscope_changed";

html_fsa_scope::html_fsa_scope():
    form("Update"),
    fs("File system Specific Attributes to consider"),
    hfs_family("HFS+ (MacOS) family attributes", html_form_input::check, "1", "", ""),
    ext_family("Ext2/3/4 (Linux) family attributes", html_form_input::check, "1", "", "")
{

	// components setup


	// adoption tree
    fs.adopt(&hfs_family);
    fs.adopt(&ext_family);
    form.adopt(&fs);
    adopt(&form);

	// events

    hfs_family.record_actor_on_event(this, html_form_input::changed);
    ext_family.record_actor_on_event(this, html_form_input::changed);
    register_name(changed);

	// css

	// tooltips
    fs.set_tooltip(TOOLTIP_HFS_FS);


}


libdar::fsa_scope html_fsa_scope::get_scope() const
{
    libdar::fsa_scope ret;

    if(hfs_family.get_value_as_bool())
	ret.insert(libdar::fsaf_hfs_plus);
    if(ext_family.get_value_as_bool())
	ret.insert(libdar::fsaf_linux_extX);

    return ret;
}

void html_fsa_scope::load_json(const json & source)
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

	hfs_family.set_value_as_bool(config.at(jlabel_hfs));
	ext_family.set_value_as_bool(config.at(jlabel_ext));
    }
    catch(json::exception & e)
    {
	throw exception_json(libdar::tools_printf("Error loading %s config", myclass_id), e);
    }
}

json html_fsa_scope::save_json() const
{
    json config;

    config[jlabel_hfs] = hfs_family.get_value_as_bool();
    config[jlabel_ext] = ext_family.get_value_as_bool();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					config);
}

void html_fsa_scope::clear_json()
{
    hfs_family.set_value_as_bool(true);
    ext_family.set_value_as_bool(true);
}

void html_fsa_scope::on_event(const string & event_name)
{
    if(event_name == html_form_input::changed)
	act(changed);
    else
	throw WEBDAR_BUG;
}


string html_fsa_scope::inherited_get_body_part(const chemin & path,
					       const request & req)
{
    return get_body_part_from_all_children(path, req);
}
