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
#include "html_form.hpp"

using namespace std;

const string html_form::changed = "form_changed";

html_form::html_form(const std::string & validate_msg):
    go_mesg(validate_msg),
    enctype(""),
    anchor_to(""),
    self_anchor(false)
{
    register_name(changed);
}


void html_form::clear_button_css_classes()
{
    css_button_classes.clear();
    my_body_part_has_changed();
}

void html_form::add_button_css_class(const string & name)
{
    if(css_button_classes.find(name) != css_button_classes.end())
        throw exception_range(string("the css_class name to add is already present: ") + name);

    css_button_classes.insert(name);
    css_classes_have_changed();
    my_body_part_has_changed();
}

string html_form::get_button_css_classes() const
{
    string ret;
    set<string>::const_iterator it = css_button_classes.begin();

    while(it != css_button_classes.end())
    {
        if(!ret.empty())
            ret += " ";
        ret += *it;

        ++it;
    }

    if(!ret.empty())
        ret = "class=\"" + ret + "\"";

    return ret;
}

void html_form::bind_to_anchor(const string & value)
{
    free_anchor_to();
    anchor_to = value;
}

void html_form::set_anchor_to_self(bool mode)
{
    free_anchor_to();

    if(mode)
    {
	assign_anchor(true);
	anchor_to = get_assigned_anchor();
	self_anchor = true;
    }
}

string html_form::inherited_get_body_part(const chemin & path,
					  const request & req)
{
    string ret = "";
    uri cible(get_path().display());

    if(!anchor_to.empty())
	cible.set_anchor_to(anchor_to);

    ret += "<form " + get_css_classes() + " method=\"post\" action=\"" + cible.get_string() + "\"";
    if(!enctype.empty())
	ret += " enctype=\""+enctype+"\"";
    ret += ">\n";
    if( ! req.get_uri().get_path().is_the_beginning_of(get_path()) && req.get_method() == "POST")
    {
	request tmp = req;
	tmp.change_method("GET");
	ret += get_body_part_from_all_children(path, tmp);
    }
    else
    {
	ret += get_body_part_from_all_children(path, req);
	if(req.get_method() == "POST")
	    act(changed);
    }
    ret += "<input " + get_button_css_classes() + " type=\"submit\" value=\"" + go_mesg + "\" />\n";
    ret += "</form>\n";

    return ret;
}


void html_form::free_anchor_to()
{
    if(!anchor_to.empty())
    {
	if(self_anchor)
	{
	    assign_anchor(false);
	    self_anchor = false;
	}
	anchor_to = "";
    }
}
