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
#include "html_url.hpp"

using namespace std;

html_url::html_url(const std::string & url,
		   const std::string & label):
    x_url(url),
    x_label(label),
    download(false),
    filename("")
{
}

void html_url::change_url(const string & newurl)
{
    if(x_url != newurl)
    {
	x_url = newurl;
	my_body_part_has_changed();
    }
}

void html_url::change_label(const string & newlabel)
{
    if(x_label != newlabel)
    {
	x_label = newlabel;
	my_body_part_has_changed();
    }
}


string html_url::get_body_part() const
{
    string ret = "<a";
    string x_class = get_css_classes();
    string dnl = download ? " download": "";

    if(! filename.empty() && download)
	dnl = dnl + "=\"" + filename + "\"";

    if(! x_class.empty())
	ret += " " + x_class;

    ret += " href=\"" + x_url + "\"" + dnl + ">" + x_label + "</a>";
    if(!get_no_CR())
	ret += "\n";

    return ret;
}
