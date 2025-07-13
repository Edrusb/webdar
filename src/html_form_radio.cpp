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
#include "webdar_css_style.hpp"


    //
#include "html_form_radio.hpp"

using namespace std;

const string html_form_radio::changed = "html_form_radio::changed";

html_form_radio::html_form_radio():
    enabled(true),
    selected(0),
    value_set(false),
    emphase(-1)
{
    register_name(changed);
}

void html_form_radio::add_choice(const string & id, const string & label)
{
    record x(id, label);
    css_class_group css_set = get_css_class_group();

    x.label.set_for_field(webdar_tools_html_display(id));
    x.label.add_css_class(css_set);
    choices.push_back(x);
    my_body_part_has_changed();
}

void html_form_radio::set_selected_num(unsigned int x)
{
    if(x >= choices.size())
    {
	unsigned int next_val = choices.size() - 1;
	if(next_val != selected)
	{
	    selected = next_val;
	    my_act();
	    my_body_part_has_changed();
	}
    }
    else
    {
	if(x != selected)
	{
	    selected = x;
	    my_act();
	    my_body_part_has_changed();
	}
    }

    value_set = true;
}

void html_form_radio::set_selected_id(const string & id)
{
    try
    {
	set_selected_id_with_warning(id, "");
    }
    catch(exception_range & e)
    {
	throw WEBDAR_BUG;
    }
}

void html_form_radio::set_selected_id_with_warning(const string & id, const string & jlabel)
{
    unsigned int val = 0;
    while(val < choices.size() && choices[val].id != id)
	++val;

    if(val < choices.size())
	set_selected_num(val);
    else
	throw exception_range(libdar::tools_printf("Unexpected requested id value %s read from key %s", id.c_str(), jlabel.c_str()));
}

void html_form_radio::unset_selected()
{
    if(selected != choices.size())
    {
	selected = choices.size();
	my_body_part_has_changed();
    }
}

const string & html_form_radio::get_selected_id() const
{
    if(selected < choices.size())
	return choices[selected].id;
    else
	throw WEBDAR_BUG;
}

void html_form_radio::set_emphase(unsigned int num)
{
    if(emphase > 0 && emphase < choices.size())
	choices[emphase].label.set_emphase(false);

    if(num < choices.size())
    {
	emphase = num;
	choices[emphase].label.set_emphase(true);
    }
    else
	throw WEBDAR_BUG;
    my_body_part_has_changed();
}

void html_form_radio::set_change_event_name(const string & name)
{
    if(name.empty())
	throw WEBDAR_BUG;

    if(modif_changed.empty())
	rename_name(changed, name);
    else
	rename_name(modif_changed, name);

    modif_changed = name;
}

void html_form_radio::set_enabled(bool val)
{
    if(enabled != val)
    {
	enabled = val;
	my_act();
	my_body_part_has_changed();
    }
}

string html_form_radio::inherited_get_body_part(const chemin & path,
						const request & req)
{
    string ret = "";
    string radio_id = get_path().namify();

	// for POST method only, extract user choice from the body of the request
	// and update this object's fields

    update_field_from_request(req);

	// for any request provide an updated HMTL content in response

    for(unsigned int i = 0; i < choices.size(); ++i)
    {
	ret += "<input " + get_css_classes() + " type=\"radio\" name=\"" + radio_id + "\" id=\"" +
	    choices[i].label.get_for_field() + "\" value=\"" +
	    choices[i].label.get_for_field() + "\" ";
	if(i == selected)
	    ret += "checked ";
	if(!enabled)
	    ret += "disabled ";
	ret += "/>\n";
	ret += choices[i].label.get_body_part();
	if(i+1 < choices.size() || !get_no_CR())
	    ret += "<br />\n";
	else
	    ret += "\n";
    }

	// now unlocking the update_field_from_request()
	// if it was locked due to a call to set_selected()

    unlock_update_field_from_request();

    return ret;
}

void html_form_radio::css_classes_have_changed()
{
    css_class_group css_set = get_css_class_group();

    for(unsigned int i = 0; i < choices.size(); ++i)
    {
	choices[i].label.clear_css_classes();
	choices[i].label.add_css_class(css_set);
    }
}

void html_form_radio::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);

}


void html_form_radio::update_field_from_request(const request & req)
{
    if(req.get_method() == "POST" && ! value_set)
    {
	map<string, string> bd = req.get_body_form();
	map<string, string>::const_iterator it = bd.find(get_path().namify());
	if(it != bd.end())
	{
	    unsigned int u = 0;
	    while(u < choices.size() && webdar_tools_html_display(choices[u].id) != webdar_tools_html_display(it->second))
		++u;
	    if(u < choices.size())
	    {
		bool has_changed = selected != u;

		selected = u;
		if(has_changed)
		{
		    my_act();
		    my_body_part_has_changed();
		}
	    }
	}
    }
}

void html_form_radio::unlock_update_field_from_request()
{
    if(!has_my_body_part_changed())
	value_set = false;
}

void html_form_radio::my_act()
{
    if(modif_changed.empty())
	act(changed);
    else
	act(modif_changed);
}
