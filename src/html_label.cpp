/*********************************************************************/
// webdar - a web server and interface program to libdar
// Copyright (C) 2013-2025 Denis Corbin
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
#include "tokens.hpp"
#include "css.hpp"
#include "css_class.hpp"
#include "webdar_css_style.hpp"

    //
#include "html_label.hpp"

using namespace std;

html_label::html_label(const std::string & x_label):
    label(x_label),
    for_field(""),
    emphase(false)
{
}

html_label::html_label(const std::string & x_label, const std::string & x_tooltip):
    label(x_label),
    for_field(""),
    emphase(false)
{
    set_tooltip(x_tooltip);
}

string html_label::inherited_get_body_part(const chemin & path,
						const request & req)
{
    return get_body_part();
}

string html_label::get_body_part() const
{
    string ret;
    string extra;
    string cssdef;
    string forstring;
    string arg;

    if(emphase)
	extra = webdar_css_style::text_bold;
    cssdef = get_css_classes(extra);
    forstring = for_field.empty() ? "" : " for=\"" + for_field + "\"";
    if(!cssdef.empty())
	cssdef = string(" ") + cssdef;
    arg = cssdef + forstring;

    ret = "<label" + arg + ">" + label;
    ret += get_tooltip_body_part();
    ret += "</label>\n";

    return ret;
}
