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
extern "C"
{

}

    // C++ system header files
#include <dar/tools.hpp>

    // webdar headers


    //
#include "guichet.hpp"

using namespace std;

guichet::guichet():
    ignore_events(false),
    select_form("Update"),
    select_fs(""),
    select("Configuration", event_select),
    around_adopted("Update"),
    around_adopted_fs(""),
    edit("Edit", event_edit),
    clear("Clear", event_clear),
    saveas_form("Save As"),
    saveas_fs(""),
    saveas_name("New configuration name", html_form_input::text, "", "50")
{
	// all the rest of initialization is done in set_child()
    adopted_jsoner = nullptr;
    adopted_subconfig = nullptr;
    adopted_frame = nullptr;
}

void guichet::set_child(const shared_ptr<bibliotheque> & ptr,
			bibliotheque::category cat,
			const shared_ptr<body_builder> & to_adopt,
			bool add_form_around)
{
	// set_child should be run only once

    if(biblio)
	throw WEBDAR_BUG;

    if(adopted)
	throw WEBDAR_BUG;

	// and run with correct arguments

    biblio = ptr;
    if(!biblio)
	throw WEBDAR_BUG;

    categ = cat;

    adopted = to_adopt;
    check_adopted();

    adopted_jsoner = dynamic_cast<jsoner*>(adopted.get());
    if(adopted_jsoner == nullptr)
	throw WEBDAR_BUG;

    adopted_subconfig = dynamic_cast<bibliotheque_subconfig*>(adopted.get());
	// adopted_subconfig may be null if the adopted object
	// does not implement this optional interface

	// component configuration
    saveas_name.set_change_event_name(event_saveas);

	// adption tree
    select_fs.adopt(&select);
    select_form.adopt(&select_fs);
    adopt(&select_form);

    if(add_form_around)
    {
	around_adopted_fs.adopt(adopted.get());
	around_adopted.adopt(&around_adopted_fs);
	adopt(&around_adopted);
	adopted_frame = &around_adopted;
    }
    else
    {
	adopt(adopted.get());
	adopted_frame = adopted.get();
    }

    if(adopted_frame == nullptr)
	throw WEBDAR_BUG;

    adopt(&edit);
    adopt(&clear);

    saveas_fs.adopt(&saveas_name);
    saveas_form.adopt(&saveas_fs);
    adopt(&saveas_form);

	// events
    select.record_actor_on_event(this, event_select);
    edit.record_actor_on_event(this, event_edit);
    clear.record_actor_on_event(this, event_clear);
    saveas_name.record_actor_on_event(this, event_saveas);
    biblio->record_actor_on_event(this, bibliotheque::changed);

	// visibility & config
    update_selected();
    set_adopted();
    set_visibility();

	// csss


}

void guichet::load_json(const json & source)
{
    json conf;
    unsigned int vers;
    string id;
    bool mode;

    check_adopted();

    try
    {
	conf = unwrap_config_from_json_header(source,
					      vers,
					      id);

	if(id != string(class_id))
	    throw exception_range(libdar::tools_printf("Unexpected class_id in json data, found %s while expecting %s",
						       id.c_str(),
						       class_id));

	if(vers > jversion)
	    throw exception_range("Json format version too hight for class guichet, upgrade your webdar software");

	mode = conf.at(jlabel_mode);
	update_selected(); // be sure to have all config name recorded
	if(mode)
	{
		// manual mode

	    select.set_selected(0);
	    adopted_jsoner->load_json(conf.at(jlabel_conf));
	}
	else
	{
		// config mode

	    string conf_cat = conf.at(jlabel_categ);
	    string conf_name = conf.at(jlabel_name);
	    bibliotheque::category newcateg = bibliotheque::string_to_category(conf_cat);

	    if(newcateg != categ)
		throw exception_range(libdar::tools_printf("Unexpected configuration category in json data: found %s, expected %s",
							   conf_cat.c_str(),
							   bibliotheque::category_to_string(categ)));
	    try
	    {
		select.set_selected(conf_name);
	    }
	    catch(exception_base & e)
	    {
		    // any exception, include exception_bug means the config name does not exist

		throw exception_range(libdar::tools_printf("Unavailable configuration name %s in category %s, the json configuration is not coherent",
							  conf_name.c_str(),
							  conf_cat.c_str()));
	    }

	    update_selected(); // this will load the configuration from the bibliotheque
	}
    }
    catch(exception_bug & e)
    {
	throw;
    }
    catch(exception_base & e)
    {
	e.change_message(string("Error met while parsing json configuration for guichet object: ") + e.get_message());
	throw;
    }

    set_visibility();
}

json guichet::save_json() const
{
    json ret;

    check_adopted();

    if(select.get_selected_num() == 0)
    {
	    // manual mode

	ret[jlabel_mode] = true;
	ret[jlabel_conf] = adopted_jsoner->save_json();
    }
    else
    {
	    // config mode

	ret[jlabel_mode] = false;
	ret[jlabel_categ] = bibliotheque::category_to_string(categ);
	ret[jlabel_name] = select.get_selected_id();
    }


    return wrap_config_with_json_header(jversion,
					class_id,
					ret);
}

void guichet::clear_json()
{
    check_adopted();

    adopted_jsoner->clear_json();
}


bibliotheque::using_set guichet::get_using_set() const
{
    bibliotheque::using_set ret;

    check_adopted();

    if(select.get_selected_num() != 0)
    {
	bibliotheque::coordinates coord(categ, select.get_selected_id());
	ret.insert(coord);
    }

    if(adopted_subconfig != nullptr)
	subconfig_add_to(ret, adopted_subconfig->get_using_set());

    return ret;
}

void guichet::on_event(const std::string & event_name)
{
    check_adopted();

    if(ignore_events)
	return;

    if(event_name == event_select)
    {
	set_adopted();
	set_visibility();
    }
    else if(event_name == event_edit)
    {
	if(select.get_selected_num() != 0)
	{
	    select.set_selected(0);
		// this trigers an new event
		// which sets visibility()
	}
	else
	    throw WEBDAR_BUG;
    }
    else if(event_name == event_clear)
    {
	if(select.get_selected_num() == 0)
	{
	    adopted_jsoner->clear_json();
	}
	else
	    throw WEBDAR_BUG;
    }
    else if(event_name == event_saveas)
    {
	if(!biblio)
	    throw WEBDAR_BUG;

	if(saveas_name.get_value().empty())
	    throw exception_range("Cannot save component with an empty name");

	if(adopted_subconfig != nullptr)
	{
	    bibliotheque::using_set sub = adopted_subconfig->get_using_set();

	    biblio->add_config(categ,
			       saveas_name.get_value(),
			       adopted_jsoner->save_json(),
			       sub);
	}
	else
	{
	    biblio->add_config(categ,
			       saveas_name.get_value(),
			       adopted_jsoner->save_json());
	}

	    // config addition as here, does not trigger bibliotheque::changed
	update_selected();

	    // setting the selected to the new created configuration
	try
	{
	    select.set_selected(saveas_name.get_value());
	}
	catch(exception_base & e)
	{
	    throw WEBDAR_BUG;
	}
    }
    else if(event_name == bibliotheque::changed)
    {
	update_selected();
    }
    else
	throw WEBDAR_BUG;
}

string guichet::inherited_get_body_part(const chemin & path,
					const request & req)
{
    check_adopted();
    return get_body_part_from_all_children(path, req);
}

void guichet::update_selected()
{
    deque<string> available;

    check_adopted();

    if(!biblio)
	throw WEBDAR_BUG;
    available = biblio->listing(categ);

    ignore_events = true;
    try
    {
	select.clear();
	select.add_choice("0", "---- manual config ----");
	for(deque<string>::iterator it = available.begin();
	    it != available.end();
	    ++it)
	{
	    select.add_choice(*it, *it); // id and label are the same
	}
    }
    catch(...)
    {
	ignore_events = false;
	throw;
    }
    ignore_events = false;

    on_event(event_select); // calling the event only once
}

void guichet::set_adopted()
{
    check_adopted();

    if(select.get_selected_num() > 0) // not the "manual config"
    {
	if(!biblio)
	    throw WEBDAR_BUG;

	adopted_jsoner->load_json(biblio->fetch_config(categ, select.get_selected_id()));
    }
	// else adopted is unchanged
}

void guichet::set_visibility()
{
    bool manualmode = select.get_selected_num() == 0;

    check_adopted();

    if(adopted_frame == nullptr)
	throw WEBDAR_BUG;
    adopted_frame->set_visible(manualmode);
    edit.set_visible(!manualmode);
    clear.set_visible(manualmode);
    saveas_form.set_visible(manualmode);
    if(!manualmode)
    {
	ignore_events = true;
	try
	{
	    saveas_name.set_value("");
	}
	catch(...)
	{
	    ignore_events = false;
	    throw;
	}
	ignore_events = false;
    }
}
