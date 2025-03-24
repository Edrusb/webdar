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
#include "webdar_css_style.hpp"
#include "tokens.hpp"

    //
#include "guichet.hpp"

using namespace std;

guichet::guichet(bool subcomponent):
    anchor_to(""),
    is_sub(subcomponent),
    ignore_events(false),
    clear_adopted(true),
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

    saveas_text.clear();
    saveas_text.add_text(0, "The configuration exposed above can be saved (or not) for reuse later, and will be editable from the \"Configuration\" menu on the left");
    saveas_text.add_nl();
    saveas_text.add_nl();
}

guichet::~guichet()
{
    if(!currently_locked.empty())
    {
	biblio->delete_external_ref_to(categ, currently_locked, this);
	currently_locked.clear();
    }
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

    box.adopt(&edit);
    select_fs.adopt(&select);

	// adding a form around fieldset to select config when not a subconfig
    if(is_sub)
	box.adopt(&select_fs);
    else
    {
	select_form.adopt(&select_fs);
	box.adopt(&select_form);
    }

    box.adopt(&clear);

    if(add_form_around)
    {
	around_adopted_fs.adopt(adopted.get());
	around_adopted.adopt(&around_adopted_fs);
	box.adopt(&around_adopted);
	adopted_frame = &around_adopted;
    }
    else
    {
	around_adopted_fs.adopt(adopted.get());
	box.adopt(&around_adopted_fs);
	adopted_frame = &around_adopted_fs;
    }

    if(adopted_frame == nullptr)
	throw WEBDAR_BUG;
    adopted_frame->add_css_class(css_left_margin);

    saveas_fs.adopt(&saveas_text);
    saveas_fs.adopt(&saveas_name);
    saveas_form.adopt(&saveas_fs);
    box.adopt(&saveas_form);
    adopt(&box);

	// visibility & config
	// this is done before events cabling to avoid
	// a lot of useless events firing that may lead
	// to a lot of useless cascade of events.
    update_selected();
    set_visibility();

	// events
    select.record_actor_on_event(this, event_select);
    edit.record_actor_on_event(this, event_edit);
    clear.record_actor_on_event(this, event_clear);
    saveas_name.record_actor_on_event(this, event_saveas);
    biblio->record_actor_on_event(this, bibliotheque::changed(categ));

	// anchor
    bind_to_anchor(anchor_to);

	// csss

    webdar_css_style::normal_button(edit);
    webdar_css_style::normal_button(clear);

    edit.add_css_class(webdar_css_style::wcs_float_right);
    clear.add_css_class(webdar_css_style::wcs_float_right);
    select_fs.add_label_css_class(webdar_css_style::wcs_text_bold);
    select_form.add_css_class(css_below_margin);
    saveas_fs.add_css_class(css_above_margin);
    box.clear_css_classes();
    box.add_css_class(css_border_left);
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

	if(vers > format_version)
	    throw exception_range(libdar::tools_printf("Json format version too hight for class %s, upgrade your webdar software", myclass_id));

	mode = conf.at(jlabel_mode);
	update_selected(); // be sure to have all config name recorded
	if(mode)
	{
		// manual mode

	    select.set_selected_num(0);
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
		select.set_selected_id_with_warning(conf_name, jlabel_name);
	    }
	    catch(exception_bug & e)
	    {
		throw;
	    }
	    catch(exception_base & e)
	    {
		    // any exception, include exception_bug means the config name does not exist

		throw exception_range(libdar::tools_printf("Exception met while setting configuration \"%s\" in category \"%s\": %s",
							   conf_name.c_str(),
							   conf_cat.c_str(),
							   e.get_message().c_str()));
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
	e.change_message(libdar::tools_printf("Error met while parsing json configuration for %s object: %s", myclass_id, e.get_message().c_str()));
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


    return wrap_config_with_json_header(format_version,
					class_id,
					ret);
}

void guichet::clear_json()
{
    check_adopted();

    clear_adopted = true;
    select.set_selected_num(0);
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
	if(select.get_selected_num() == 0)
	{
	    if(!biblio)
		throw WEBDAR_BUG;

	    if(! currently_locked.empty())
	    {
		biblio->delete_external_ref_to(categ, currently_locked, this);
		currently_locked.clear();
	    }

	    if(clear_adopted)
		adopted_jsoner->clear_json();
		// clear_adopted is true when the user has selected "manual mode"
		// but is false when the user clicked the "edit" button

	    box.clear_css_classes();
	    box.add_css_class(css_border_left);

	}
	else
	    set_adopted();

	if(is_sub)
	{
	    string fs_title = string(fs_title_subcomp);

	    if(select.get_selected_num() == 0)
		fs_title += "...";
	    else
		fs_title += " "  + select.get_selected_id();
	    select_fs.change_label(fs_title);
	}

	set_visibility();
    }
    else if(event_name == event_edit)
    {
	if(select.get_selected_num() != 0)
	{
	    clear_adopted = false;
	    select.set_selected_num(0);
		// this trigers an new event
		// which in particular sets visibility()
		// (see just above the event_select)
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
	    select.set_selected_id(saveas_name.get_value());
	}
	catch(exception_base & e)
	{
	    throw WEBDAR_BUG;
	}
    }
    else if(event_name == bibliotheque::changed(categ))
    {
	update_selected();
    }
    else
	throw WEBDAR_BUG;
}

void guichet::bind_to_anchor(const std::string & val)
{
    anchor_to = val;
    if(adopted)
	adopted->bind_to_anchor(anchor_to);
    select_form.bind_to_anchor(anchor_to);
    edit.bind_to_anchor(anchor_to);
    clear.bind_to_anchor(anchor_to);
    saveas_form.bind_to_anchor(anchor_to);
}

string guichet::inherited_get_body_part(const chemin & path,
					const request & req)
{
    check_adopted();
    clear_adopted = true;
    return get_body_part_from_all_children(path, req);
}

void guichet::new_css_library_available()
{
    css tmp;
    std::unique_ptr<css_library> & csslib = lookup_css_library();

    if(!csslib)
	throw WEBDAR_BUG;

    if(!csslib->class_exists(css_left_margin))
    {
	css tmp;

	tmp.css_margin_left("2em");

	csslib->add(css_left_margin, tmp);
    }

    if(!csslib->class_exists(css_below_margin))
    {
	css tmp;

	tmp.css_margin_bottom("2em");

	csslib->add(css_below_margin, tmp);
    }

    if(!csslib->class_exists(css_above_margin))
    {
	css tmp;

	tmp.css_margin_top("2em");

	csslib->add(css_above_margin, tmp);
    }

    if(!csslib->class_exists(css_border_left))
    {
	css tmp;

	tmp.css_border_width(css::bd_left, css::bd_medium);
	tmp.css_border_style(css::bd_left, css::bd_solid);
	tmp.css_margin_left("1em");
	tmp.css_padding_left("0.2em");
	tmp.css_margin_top("1em");
	tmp.css_border_color(css::bd_left, COLOR_MENU_BORDER_GREY);

	csslib->add(css_border_left, tmp);
    }

    webdar_css_style::update_library(*csslib);
}


void guichet::update_selected()
{
    deque<string> available;
    bool try_keep_selected = select.num_choices() > 0;
    string current_choice = try_keep_selected ? select.get_selected_id(): "";
    bool still_exists = false;
    bool manual_mode = select.get_selected_num() == 0;

    check_adopted();

    if(!biblio)
	throw WEBDAR_BUG;
    available = biblio->listing(categ);

    ignore_events = true;
    try
    {
	select.clear();

	if(is_sub)
	    select.add_choice("0", "---- select a configuration ----");
	else
	    select.add_choice("0", "---- manual config ----");

	for(deque<string>::iterator it = available.begin();
	    it != available.end();
	    ++it)
	{
	    select.add_choice(*it, *it); // id and label are the same
	    if(try_keep_selected && *it == current_choice)
		still_exists = true;
	}

	if(still_exists)
	    select.set_selected_id(current_choice);
	    // does not triggers event_select
	    // because ignore_event is true
    }
    catch(...)
    {
	ignore_events = false;
	throw;
    }
    ignore_events = false;

    if(! manual_mode)
	on_event(event_select); // calling the event only once
	// we do not trigger a event_select event
	// when current selection is the manual mode
	// because first this is the same selection number
	// and second a change of configuration for a given
	// configuration name in the bibliotheque has no impact
	// here in manual mode as the configuration is local
	// to the guichet object.
}

void guichet::set_adopted()
{
    check_adopted();

    if(select.get_selected_num() > 0) // not the "manual config"
    {
	if(!biblio)
	    throw WEBDAR_BUG;

	try
	{
	    if(!currently_locked.empty())
	    {
		biblio->delete_external_ref_to(categ, currently_locked, this);
		currently_locked.clear();
	    }
	    adopted_jsoner->load_json(biblio->fetch_config(categ, select.get_selected_id()));
	    currently_locked = select.get_selected_id();
	    biblio->add_external_ref_to(categ, currently_locked, this);
	}
	catch(...)
	{
	    clear_adopted = false;
	    select.set_selected_num(0); // passing on manual mode
	    throw;
	}

	box.clear_css_classes();
    }
	// else adopted is unchanged
}

void guichet::set_visibility()
{
    bool manualmode = select.get_selected_num() == 0;

    check_adopted();
    if(adopted_frame == nullptr)
	throw WEBDAR_BUG;

    if(is_sub)
    {
	adopted_frame->set_visible(false);
	edit.set_visible(false);
	clear.set_visible(false);
	saveas_form.set_visible(false);
    }
    else
    {
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
}
