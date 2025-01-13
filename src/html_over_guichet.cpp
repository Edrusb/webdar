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


    // webdar headers



    //
#include "html_over_guichet.hpp"

using namespace std;

html_over_guichet::html_over_guichet()
{
    inner.reset(new (nothrow) html_form_mask_bool());
    if(!inner)
	throw exception_memory();
}

void html_over_guichet::set_child(const std::shared_ptr<bibliotheque> & ptr,
				  bibliotheque::category cat)
{
    if(!ptr)
	throw WEBDAR_BUG;

	// component configuration

    wicket.set_child(ptr,
		     cat,
		     inner,
		     false);


	// adoption tree

    adopt(&wicket);

	// events


	// css

}

void html_over_guichet::add_mask_type(const std::string & label)
{
    check_inner();
    inner->add_mask_type(label);
}

void html_over_guichet::clear_all_masks_type()
{
    check_inner();
    inner->clear_all_masks_type();
}

void html_over_guichet::set_obj_type_provider(const html_form_dynamic_table_object_provider* provider)
{
    check_inner();
    inner->set_obj_type_provider(provider);
}

unique_ptr<libdar::mask> html_over_guichet::get_mask() const
{
    check_inner();
    return inner->get_mask();
}

void html_over_guichet::load_json(const json & source)
{
    wicket.load_json(source);
}

json html_over_guichet::save_json() const
{
    return wicket.save_json();
}

void html_over_guichet::clear_json()
{
    wicket.clear_json();
}

bibliotheque::using_set html_over_guichet::get_using_set() const
{
    return wicket.get_using_set();
}

string html_over_guichet::inherited_get_body_part(const chemin & path,
						  const request & req)
{
    return get_body_part_from_all_children(path, req);
}

