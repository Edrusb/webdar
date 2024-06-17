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
extern "C"
{

}

    // C++ system header files
#include "html_form_mask_subdir.hpp"

    // webdar headers


    //
#include "html_mask_form_path.hpp"

using namespace std;

html_mask_form_path::html_mask_form_path(bool allow_absolute_paths):
    html_form("Update")
{
    root.add_mask_type("Path expression", html_form_mask_subdir(allow_absolute_paths));
    root.add_mask_myself("Logicial combination");
    adopt(&root);
}

void html_mask_form_path::set_fs_root(const std::string & prefix)
{
    try
    {
	if(!prefix.empty())
	{
	    libdar::path tmp(prefix);
	    root.set_root_prefix(tmp);
	}
    }
    catch(libdar::Egeneric & e)
    {
	e.prepend_message("Error while setting prefix for path base filtering");
	throw exception_libcall(e);
    }
}
