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
#include "base64.hpp"
#include "session.hpp"
#include "error_page.hpp"
    //
#include "challenge.hpp"

using namespace std;

challenge::challenge(const shared_ptr<const authentication> & base):
    page("Webdar")
{
    if(!base)
	throw WEBDAR_BUG;
    database = base;

    title.add_text(1, "Authentication required to access Webdar");
    page.adopt(&title);
}


bool challenge::is_an_authoritative_request(const request & req, string & user)
{
    bool ret = false;
    string val;

    if(req.find_attribute(HDR_AUTHORIZATION, val))
    {
	string sp1, sp2;

	webdar_tools_split_in_two(' ', val, sp1, sp2);
	if(webdar_tools_to_canonical_case(sp1) == webdar_tools_to_canonical_case("Basic"))
	{
	    string clear = base64(sp2).decode();
	    webdar_tools_split_in_two(':', clear, sp1, sp2);

	    if(database->valid_credentials(sp1, sp2))
	    {
		user = sp1;
		ret = true;
	    }
	}
    }

    return ret;
}


answer challenge::give_answer(const request & req)
{
    answer ret;

	// Request a login/password
    ret.set_status(STATUS_CODE_UNAUTHORIZED);
    ret.set_reason("login/password requested");
    ret.set_attribute(HDR_WWW_AUTHENTICATE, "Basic realm=\"/Webdar\"");
    ret.add_body(page.get_body_part(req.get_uri().get_path(), req));

    return ret;
}


