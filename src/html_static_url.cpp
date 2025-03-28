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
#include "html_static_url.hpp"

using namespace std;

html_static_url::html_static_url(const std::string & url,
				 const std::string & label):
    html_url(url)
{
    faked.reset(new (nothrow) central_report_stdout(crit));
    if(!faked)
	throw exception_memory();

    adopt(&child);
    change_label(label);
}

void html_static_url::change_label(const string & newlabel)
{
    if(x_label != newlabel)
    {
	x_label = newlabel;
	child.clear();
	child.add_text(0, x_label);
	my_body_part_has_changed();
    }
}

string html_static_url::get_body_part() const
{
    chemin path;
    request req(faked);

    req.fake_valid_request();

    return const_cast<html_static_url*>(this)->html_url::get_body_part(path, req);
}
