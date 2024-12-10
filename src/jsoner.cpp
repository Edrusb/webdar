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
					  const std::string & class_id,
					  const json & config)
{
    json ret;

    ret[version_label] = version;
    ret[id_label] = class_id;
    ret[config_label] = config;

    return ret;
}

json jsoner::unwrap_config_from_json_header(const json & source,
					    unsigned int & version,
					    std::string & class_id)
{
    json ret;
    string step;

    try
    {
	step = "while parsing version information";
	version = source.at(version_label);
	step = "while parsing id information";
	class_id = source.at(id_label);
	step = "while parsing config information";
	ret = source.at(config_label);
    }
    catch(json::exception & e)
    {
	throw exception_json(step, e);
    }

    return ret;
}
