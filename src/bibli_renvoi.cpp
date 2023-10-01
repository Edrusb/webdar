/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2023 Denis Corbin
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
#include "exceptions.hpp"

    //
#include "bibli_renvoi.hpp"

using namespace std;

bibli_renvoi::bibli_renvoi(bibli_referable *ptr)
{
    if(ptr == nullptr)
	throw WEBDAR_BUG;

    peer_with(ptr);
}

const bibli_referable* bibli_renvoi::get_reference() const
{
    reference* ptr = nullptr;

    if(size() != 1)
	throw WEBDAR_BUG;

    reset_read_peers();
    if(! read_next_peer(ptr))
	throw WEBDAR_BUG;

    if(ptr == nullptr)
	throw WEBDAR_BUG;

    const bibli_referable* ret = dynamic_cast<const bibli_referable*>(ptr);

    if(ret == nullptr)
	throw WEBDAR_BUG;

    return ret;
}

