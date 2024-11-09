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



    //
#include "html_fsa_scope.hpp"

using namespace std;

html_fsa_scope::html_fsa_scope():
    form("Update"),
    fs("File system Specific Attributes to consider"),
    hfs_family("HFS+ (MacOS) family attributes", html_form_input::check, "1", "1"),
    ext_family("Ext* (Linux) family attributes", html_form_input::check, "1", "1")
{

	// components setup


	// adoption tree
    fs.adopt(&hfs_family);
    fs.adopt(&ext_family);
    form.adopt(&fs);
    adopt(&form);

	// events


	// css


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

string html_fsa_scope::inherited_get_body_part(const chemin & path,
					       const request & req)
{
    return get_body_part_from_all_children(path, req);
}
