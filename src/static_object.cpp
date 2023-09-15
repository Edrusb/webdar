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
#include "tokens.hpp"


    //
#include "static_object.hpp"

using namespace std;

answer static_object_text::give_answer() const
{
    answer ret;

    ret.set_status(STATUS_CODE_OK);
    ret.set_reason("ok");
    ret.set_attribute(HDR_CONTENT_TYPE, "text/plain");
    ret.add_body(data);
	// recreating the answer at each request consume CPU cycles
	// at the advantage of avoiding permanently duplicating
	// text data in memory (present once as static data in data segment
	// and a second time in the answer object as std::string data, but
	// here this second time is temporary for the duration of the request

    return ret;
}
