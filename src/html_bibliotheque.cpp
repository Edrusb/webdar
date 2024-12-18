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
#include "html_bibliotheque.hpp"

using namespace std;

html_bibliotheque::html_bibliotheque(std::shared_ptr<bibliotheque> & ptr)
{
    unique_ptr<html_entrepot> tmp;

    if(!ptr)
	throw WEBDAR_BUG;

    biblio = ptr;
    if(! biblio)
	throw WEBDAR_BUG;

    tmp.reset(new (nothrow) html_entrepot());
    if(!tmp)
	throw exception_memory();

    ab_entrepot.reset(new (nothrow) arriere_boutique<html_entrepot>(ptr,
								    bibliotheque::repo,
								    tmp));
    if(!ab_entrepot)
	throw exception_memory();

	// components setups
    tabs.add_tab(tab_main, tab_main);
    tabs.add_tab(tab_repo, tab_repo);


	// adoption tree
    tabs.adopt_in_section(tab_repo, ab_entrepot.get());
    adopt(&tabs);

	// actors and events


	// css


}

string html_bibliotheque::inherited_get_body_part(const chemin & path,
						  const request & req)
{
    return get_body_part_from_all_children(path, req);
}

