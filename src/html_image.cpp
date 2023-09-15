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



    //
#include "html_image.hpp"

using namespace std;

html_image::html_image(const std::string & url, const std::string & alt)
{
    x_url = url;
    x_alt = alt;
    dim_x = "";
    dim_y = "";
}

string html_image::get_body_part() const
{
    string ret;

    ret = "<img " + get_css_classes() + " ";
    ret += "src=\"" + x_url + "\" alt=\"" + x_alt +"\" ";
    if(dim_x != "" && dim_y != "") // yes both
	ret += "width=\"" + dim_x + "\" heigth=\"" + dim_y + "\" ";
    ret += ">";

    if(!get_no_CR())
	ret += "\n";

    return ret;
}
