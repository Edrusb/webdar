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
#include "html_entrepot_landing.hpp"

using namespace std;

const string html_entrepot_landing::changed = "html_entrep_land_changed";
const string html_entrepot_landing::landing_path_changed = "html_entrep_landing_changed";


html_entrepot_landing::html_entrepot_landing():
        form("Apply changes"),
	fs(""),
	use_landing_path("Replace current backup path by the landing path", html_form_input::check, "", "1"),
	landing_path("Landing path", "/", "30", "Select default path to store and look for backups..."),
	custom_event_name(changed),
	custom_event_landing_path(landing_path_changed),
	ignore_events(false),
	delayed_landing_update(false)
{

	// component configuration
    if(repoxfer.get_html_user_interaction())
	repoxfer.get_html_user_interaction()->auto_hide(true, false);
    else
	throw WEBDAR_BUG;

    landing_path.set_select_mode(html_form_input_file::select_dir);
    landing_path.set_can_create_dir(true);

	// adoption tree
    fs.adopt(&entrep);
    fs.adopt(&use_landing_path);
    fs.adopt(&landing_path);
    form.adopt(&fs);
    adopt(&form);
    adopt(&repoxfer);

	// events
    entrep.record_actor_on_event(this, html_entrepot::changed);
    landing_path.set_change_event_name(landing_path_changed);
    landing_path.record_actor_on_event(this, landing_path_changed);
    use_landing_path.set_change_event_name(landing_path_changed);
    use_landing_path.record_actor_on_event(this, landing_path_changed);
    repoxfer.record_actor_on_event(this, html_libdar_running_popup::libdar_has_finished);
    register_name(custom_event_name);         // is equal to "changed" at cosntruction time (here)
    register_name(custom_event_landing_path); // is equal to landing_path_changed at construction time


    	// visibility
    repoxfer.set_visible(false);

	// css
}

shared_ptr<libdar::entrepot> & html_entrepot_landing::get_entrepot(shared_ptr<html_web_user_interaction> & webui) const
{
    return entrep.get_entrepot(webui);
}


void html_entrepot_landing::on_event(const string & event_name)
{
    if(ignore_events)
	return;

    if(event_name == html_entrepot::changed)
    {
	delayed_landing_update = true;
	my_body_part_has_changed(); // update will be done from inherited_get_body_part()

	trigger_changed_event();
	    // yes we propagate right now as other
	    // actor component may be visible and
	    // able to handle this
    }
    else if(event_name == html_libdar_running_popup::libdar_has_finished)
    {
	if(delayed_landing_update)
	{
	    delayed_landing_update = false;
	    trigger_changed_event();
	}
    }
    else if(event_name == custom_event_landing_path)
    {
	if(use_landing_path.get_value_as_bool())
	    act(custom_event_landing_path);
    }
    else
	throw WEBDAR_BUG;
}


void html_entrepot_landing::set_event_name(const string & name)
{
    rename_name(custom_event_name, name);
    custom_event_name = name;
}

void html_entrepot_landing::set_event_landing_path(const string & name)
{
    rename_name(custom_event_landing_path, name);
    custom_event_landing_path = name;
}

void html_entrepot_landing::load_json(const json & source)
{
    try
    {
	unsigned int version;
	string class_id;
	json config = unwrap_config_from_json_header(source,
						     version,
						     class_id);

	if(class_id != myclass_id)
	    throw exception_range(libdar::tools_printf("Unexpected class_id in json data, found %s while expecting %s",
						       class_id.c_str(),
						       myclass_id));

	if(version > format_version)
	    throw exception_range(libdar::tools_printf("Json format version too hight for %s, upgrade your webdar software",
						       myclass_id));

	ignore_events = true;

	try
	{
	    entrep.load_json(config.at(jlabel_entrepot));
	    use_landing_path.set_value_as_bool(config.at(jlabel_use_landing_path));
	    landing_path.set_value(config.at(jlabel_landing_path));
	}
	catch(...)
	{
	    ignore_events = false;
	    throw;
	}
	ignore_events = false;
    }
    catch(json::exception & e)
    {
	throw exception_json(libdar::tools_printf("Error loading %s config", myclass_id), e);
    }
    on_event(html_entrepot::changed);
    on_event(custom_event_landing_path);
}

json html_entrepot_landing::save_json() const
{
    json config;

    config[jlabel_entrepot] = entrep.save_json();
    config[jlabel_use_landing_path] = use_landing_path.get_value_as_bool();
    config[jlabel_landing_path] = landing_path.get_value();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					config);
}

void html_entrepot_landing::clear_json()
{
    ignore_events = true;

    try
    {
	entrep.clear_json();
	use_landing_path.set_value_as_bool(false);
	landing_path.set_value("/");
    }
    catch(...)
    {
	ignore_events = false;
	throw;
    }
    ignore_events = false;

    on_event(html_entrepot::changed);
    on_event(custom_event_landing_path);
}

void html_entrepot_landing::set_to_webdar_defaults()
{
    entrep.set_to_webdar_defaults();
    use_landing_path.set_value_as_bool(true);
    landing_path.set_value("/tmp");
}

string html_entrepot_landing::inherited_get_body_part(const chemin & path,
						      const request & req)
{
    if(delayed_landing_update)
	start_updating_landing();

    return get_body_part_from_all_children(path, req);
}

void html_entrepot_landing::css_classes_have_changed()
{
    set<string> css_classes = get_css_classes_as_a_set();

    form.clear_css_classes();
    for(set<string>::iterator it = css_classes.begin();
	it != css_classes.end();
	++it)
	form.add_css_class(*it);
}

void html_entrepot_landing::inherited_run()
{
    shared_ptr<html_web_user_interaction> ptr(repoxfer.get_html_user_interaction());

    if(!ptr)
	throw WEBDAR_BUG;
    landing_path.set_entrepot(entrep.get_entrepot(ptr));
	// this should not trigger a change event from landing_path
}

void html_entrepot_landing::signaled_inherited_cancel()
{
    pthread_t libdar_tid;
    libdar::thread_cancellation th;

    if(is_running(libdar_tid))
	th.cancel(libdar_tid, true, 0);
}

void html_entrepot_landing::trigger_changed_event()
{
    if(custom_event_name.empty())
	act(changed);
    else
	act(custom_event_name);
}

void html_entrepot_landing::start_updating_landing()
{
    delayed_landing_update = false;
    repoxfer.set_visible(true);
    repoxfer.run_and_control_thread(this);
}
