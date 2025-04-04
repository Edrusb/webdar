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



    //
#include "jsoner.hpp"

using namespace std;

json jsoner::wrap_config_with_json_header(unsigned int version,
					  const string & class_id,
					  const json & config)
{
    json ret;

    try
    {
	ret[jsoner_version_label] = version;
	ret[jsoner_id_label] = class_id;
	ret[jsoner_config_label] = config;
    }
    catch(json::exception & e)
    {
	throw exception_json("wrapping config with json header", e);
    }

    return ret;
}

json jsoner::unwrap_config_from_json_header(const json & source,
					    unsigned int & version,
					    string & class_id)
{
    json ret;
    string step;

    try
    {
	step = "parsing version information";
	version = source.at(jsoner_version_label);
	step = "parsing id information";
	class_id = source.at(jsoner_id_label);
	step = "parsing config information";
	ret = source.at(jsoner_config_label);
    }
    catch(json::exception & e)
    {
	throw exception_json(string("unwrapping config from json header: ") + step + ": ", e);
    }

    return ret;
}
