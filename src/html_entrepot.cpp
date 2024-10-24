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
#include "webdar_tools.hpp"

    // webdar headers
#include "environment.hpp"
#include "webdar_tools.hpp"
#include "css.hpp"
#include "tokens.hpp"

    //
#include "html_entrepot.hpp"

using namespace std;

const string html_entrepot::changed = "html_entrep_changed";
const string html_entrepot::repo_type_changed = html_form_select::changed + "type"; // be sure this is different from the default event name

html_entrepot::html_entrepot():
    form("Apply changes"),
    fs(""),
    repo_type("Repository type", repo_type_changed),
    host("Remote host", html_form_input::text, "", 30),
    port("Port", html_form_input::number, "0", 7),
    login("Login", html_form_input::text, "", 30),
    auth_type("Authentication mode"),
    pass("Password", html_form_input::password, "", 30),
    auth_from_file("Fetch password from netrc file", html_form_input::check, "", 1),
    pub_keyfile("Public key file", "/", 40, "Select the public key file..."),
    prv_keyfile("Private key file", "/", 40, "Select the private key file..."),
    knownhosts_check("Check remote host from the known-hosts file", html_form_input::check, "1", 1),
    known_hosts_file("Known-hosts file", "/", 40, "Select the knowhosts file..."),
    wait_time("Network retry delay (s)", html_form_input::number, "30", 5),
    verbose("Verbose network connection", html_form_input::check, "", 1),
    custom_event_name(""),
    ignore_events(false),
    entrep_type_has_changed(false),
    entrep_need_update(false)
{
    chemin home = global_envir.get_value_with_default("HOME", "/");
    chemin tmp;
    string val;

	// component configuration
    repo_type.add_choice(type_local, "local filesystem");         // index 0
    repo_type.add_choice(type_ftp, "FTP protocol (unciphered)");  // index 1
    repo_type.add_choice(type_sftp, "SFTP protocol (ciphered)");  // index 2
    repo_type.set_selected(0);

    port.set_range(1, 65536); // actually set to zero to track value change, but 0 is invalid

    auth_type.add_choice(auth_pass, "Password authentication");
    auth_type.add_choice(auth_key, "Keyfile authentication");
    auth_type.set_selected(0);

    pub_keyfile.set_select_mode(html_form_input_file::select_file);
    pub_keyfile.set_can_create_dir(false);
    if(global_envir.get_value_of("DAR_SFTP_PUBLIC_KEYFILE", val))
	pub_keyfile.set_value(val);
    else
    {
	tmp = home;
	tmp.push_back(".ssh");
	tmp.push_back("id_rsa.pub");
	pub_keyfile.set_value(tmp.display());
    }

    prv_keyfile.set_select_mode(html_form_input_file::select_file);
    prv_keyfile.set_can_create_dir(false);
    if(global_envir.get_value_of("DAR_SFTP_PRIVATE_KEYFILE", val))
	prv_keyfile.set_value(val);
    else
    {
	tmp = home;
	tmp.push_back(".ssh");
	tmp.push_back("id_rsa");
	prv_keyfile.set_value(tmp.display());
    }

    known_hosts_file.set_select_mode(html_form_input_file::select_file);
    known_hosts_file.set_can_create_dir(false);
    if(global_envir.get_value_of("DAR_SFTP_KNOWNHOSTS_FILE", val))
	known_hosts_file.set_value(val);
    else
    {
	tmp = home;
	tmp.push_back(".ssh");
	tmp.push_back("known_hosts");
	known_hosts_file.set_value(tmp.display());
    }

    	// adoption tree
    fs.adopt(&repo_type);
    fs.adopt(&host);
    fs.adopt(&port);
    fs.adopt(&login);
    fs.adopt(&auth_type);
    fs.adopt(&pass);
    fs.adopt(&auth_from_file);
    fs.adopt(&pub_keyfile);
    fs.adopt(&prv_keyfile);
    fs.adopt(&knownhosts_check);
    fs.adopt(&known_hosts_file);
    fs.adopt(&wait_time);
    fs.adopt(&verbose);
    form.adopt(&fs);
    adopt(&form);

	// events and actors
    repo_type.record_actor_on_event(this, repo_type_changed);
    auth_type.record_actor_on_event(this, html_form_select::changed);
    knownhosts_check.record_actor_on_event(this, html_form_input::changed);
    port.record_actor_on_event(this, html_form_input::changed);

	// my own events
    register_name(changed);

	// css

	// update components visibility
    update_visible();
}


shared_ptr<libdar::entrepot> html_entrepot::get_entrepot(shared_ptr<html_web_user_interaction> & webui) const
{
    if(!webui)
	throw WEBDAR_BUG;

    webui->clear();

    dialog = webui->get_user_interaction();
    if(!dialog)
	throw WEBDAR_BUG;

    if(! entrep || entrep_need_update)
    {
	if(! webui->is_libdar_running())
	    webui->auto_hide(true, true);

	webui->run_and_control_thread(const_cast<html_entrepot*>(this)); // this launches a new thread running inherited_run() and the caller returns
	join();
	// we join() ourself, yes, we wait here for the thread launched above to complete or be interrupted
	// multi-threading is needed here to pass a thread-id for libdar and html_web_user_interaction to be
	// able to interrupt it gracefully. We cannot interrupt the calling/current thread which may be the
	// program main thread. However webui should have it get_body_part() running from another thread to
	// display and update the libdar::user_interaction it contains.
    }
    entrep_need_update = false;

    if(!entrep)
	throw WEBDAR_BUG;
	// if an error had to succeed, an exception should be propagated from the join() above
	// as this is not the case (there is no try/catch statement to catch it here) the inherited_run
	// succeeded but did not provide a libdar::entrepot as it should

    return entrep;
}

void html_entrepot::on_event(const string & event_name)
{
    if(ignore_events)
	return;

    if(event_name == repo_type_changed)
    {
	entrep_type_has_changed = true;
	    // no need to trigger my_body_part_has_changed()
	    // if the body_builder objects we adopted changed,
	    // our body_builder changed status will be set accordingly

	ignore_events = true;
	try
	{
	    switch(repo_type.get_selected_num())
	    {
	    case 0:
		    // nothing to do
		break;
	    case 1:
		clear_form();
		port.set_value("21");
		break;
	    case 2:
		clear_form();
		port.set_value("22");
		break;
	    default:
		throw WEBDAR_BUG;
	    }
	}
	catch(...)
	{
	    ignore_events = false;
	    throw;
	}
	ignore_events = false;
    }
    else if(event_name == html_form_select::changed
	    || event_name == html_form_input::changed)
	entrep_type_has_changed = true;
    else
	throw WEBDAR_BUG;

    update_visible();
}

void html_entrepot::set_event_name(const string & name)
{
    if(!custom_event_name.empty())
	throw WEBDAR_BUG; // only one alternative event name is allowed

    custom_event_name = name;
    register_name(custom_event_name);
}

string html_entrepot::inherited_get_body_part(const chemin & path,
					      const request & req)
{
    string ret;
    entrep_type_has_changed = false;

    ret = get_body_part_from_all_children(path, req);

	// When a change has just been received:

	// we must record that the entrep will have
	// to be recreated when it will be requested
	// note: entrep_type_has_changed is local to the call
	// and needed to avoid generating the event in all case
	// read the comments below... entrep_need_update is
	// an object field and survives this method.
    entrep_need_update |= has_my_body_part_changed(); // reset by get_entrepot()

    if(has_my_body_part_changed()
       && (! entrep_type_has_changed || repo_type.get_selected_num() == 0))
	    // we trigger an event only if:
	    // there was some change in the html parameters but if the
	    // repo type has changed we only trigger if the new type is of
	    // local filesystem. The reason is to avoid trying to generate
	    // sftp or ftp entrepot while the user has just changed the type
	    // to theses but has not yet got the opportunity to provide
	    // parameters. Local filesystem is different because it has no
	    // parameters to be set.
	trigger_event();

    return ret;
}

void html_entrepot::inherited_run()
{
    if(!dialog)
	throw WEBDAR_BUG;
    try
    {
	libdar::mycurl_protocol proto;

	switch(repo_type.get_selected_num())
	{
	case 0:
	    entrep.reset(new (nothrow) libdar::entrepot_local("", "", true));
	    break;
	case 1:
	case 2:
	    switch(repo_type.get_selected_num())
	    {
	    case 1:
		proto = libdar::proto_ftp;
		break;
	    case 2:
		proto = libdar::proto_sftp;
		break;
	    default:
		throw WEBDAR_BUG;
	    }

	    cancellation_checkpoint();
	    entrep.reset(new (nothrow) libdar::entrepot_libcurl(dialog,
								proto,
								login.get_value(),
								libdar::secu_string(pass.get_value().c_str(), pass.get_value().size()),
								host.get_value(),
								port.get_value(),
								auth_from_file.get_value_as_bool(),
								pub_keyfile.get_value(),
								prv_keyfile.get_value(),
								knownhosts_check.get_value_as_bool() ? "" : known_hosts_file.get_value(),
								libdar::U_I(webdar_tools_convert_to_int(wait_time.get_value())),
								verbose.get_value_as_bool()));
	    break;
	default:
	    throw WEBDAR_BUG;
	}

	if(!entrep)
	    throw exception_memory();
    }
    catch(...)
    {
	dialog.reset();
	throw;
    }
    dialog.reset();
}

void html_entrepot::signaled_inherited_cancel()
{
    pthread_t libdar_tid;
    libdar::thread_cancellation th;

    if(is_running(libdar_tid))
	th.cancel(libdar_tid, true, 0);
}

void html_entrepot::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    css tmp;

    if(!csslib)
	throw WEBDAR_BUG;

    if(! csslib->class_exists(css_warning))
    {
	tmp.clear();
	tmp.css_font_weight_bold();
	tmp.css_color(RED);
	tmp.css_float(css::fl_left);

	csslib->add(css_warning, tmp);
    }
}

void html_entrepot::update_visible()
{
    switch(repo_type.get_selected_num())
    {
    case 0: // local
	host.set_visible(false);
	port.set_visible(false);
	login.set_visible(false);
	auth_type.set_visible(false);
	pass.set_visible(false);
	auth_from_file.set_visible(false);
	pub_keyfile.set_visible(false);
	prv_keyfile.set_visible(false);
	knownhosts_check.set_visible(false);
	known_hosts_file.set_visible(false);
	wait_time.set_visible(false);
	verbose.set_visible(false);
	break;
    case 1: // ftp
    case 2: // sftp
	host.set_visible(true);
	port.set_visible(true);
	login.set_visible(true);
	if(repo_type.get_selected_num() == 1) // ftp
	{
	    auth_type.set_visible(false);
	    auth_from_file.set_visible(true);
	    pub_keyfile.set_visible(false);
	    prv_keyfile.set_visible(false);
	    knownhosts_check.set_visible(false);
	    known_hosts_file.set_visible(false);
	    pass.set_visible(true);
	}
	else // sftp
	{
	    auth_type.set_visible(true);
	    switch(auth_type.get_selected_num())
	    {
	    case 0: // pass
		pass.set_visible(true);
		auth_from_file.set_visible(true);
		pub_keyfile.set_visible(false);
		prv_keyfile.set_visible(false);
		break;
	    case 1: // auth key
		pass.set_visible(false);
		auth_from_file.set_visible(false);
		pub_keyfile.set_visible(true);
		prv_keyfile.set_visible(true);
		break;
	    default:
		throw WEBDAR_BUG;
	    }
	    knownhosts_check.set_visible(true);
	    known_hosts_file.set_visible(knownhosts_check.get_value_as_bool());
	}
	wait_time.set_visible(true);
	verbose.set_visible(true);
	break;
    default:
	throw WEBDAR_BUG;
    }
}

void html_entrepot::trigger_event()
{
    if(custom_event_name.empty())
	act(changed);
    else
	act(custom_event_name);
}

void html_entrepot::clear_form()
{
    host.set_value("");
    port.set_value("0");
    login.set_value("");
    auth_type.set_selected(0);
    pass.set_value("");
    auth_from_file.set_value("");
}
