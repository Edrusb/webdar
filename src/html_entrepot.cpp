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
#include "webdar_tools.hpp"

    // webdar headers
#include "environment.hpp"

    //
#include "html_entrepot.hpp"

using namespace std;

html_entrepot::html_entrepot():
    form("Apply changes"),
    fs("Entrepot parameters"),
    repo_type("Entrepot type"),
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
    verbose("Verbose network connection", html_form_input::check, "", 1)
{
    chemin home = global_envir.get_value_with_default("HOME", "/");
    chemin tmp;
    string val;

    got_inner_event = false;

	// component configuration
    repo_type.add_choice(type_local, "local filesystem");         // index 0
    repo_type.add_choice(type_ftp, "FTP protocol (unciphered)");  // index 1
    repo_type.add_choice(type_sftp, "SFTP protocol (ciphered)");  // index 2
    repo_type.set_selected(0);

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
    repo_type.record_actor_on_event(this, html_form_select::changed);
    auth_type.record_actor_on_event(this, html_form_select::changed);
    knownhosts_check.record_actor_on_event(this, html_form_input::changed);

	// css if needed

	// update components visibility
    update_visible();
}


std::shared_ptr<libdar::entrepot> html_entrepot::get_entrepot(std::shared_ptr<html_web_user_interaction> & webui) const
{
    std::shared_ptr<libdar::entrepot> ret;

    if(!webui)
	throw WEBDAR_BUG;

    webui->clear();

    dialog = webui->get_user_interaction();
    if(!dialog)
	throw WEBDAR_BUG;

    if(! webui->is_libdar_running())
    {
	webui->auto_hide(true, true);

	webui->run_and_control_thread(const_cast<html_entrepot*>(this)); // this launches a new thread running inherited_run() and the caller returns
	join();
	    // we join() ourself, yes, we wait here for the thread launched above to complete or be interrupted
	    // multi-threading is needed here to pass a thread-id for libdar and html_web_user_interaction to be
	    // able to interrupt it gracefully. We cannot interrupt the calling/current thread which may be the
	    // program main thread. However webui should have it get_body_part() running from another thread to
	    // display and update the libdar::user_interaction it contains.
    }
    else
    {
	const_cast<html_entrepot*>(this)->run(); // we run ourself without webui control (assuming the control is on our calling thread
	const_cast<html_entrepot*>(this)->join(); // and we wait for our subthread to complete
    }

    if(!entrep)
	throw WEBDAR_BUG;
	// if an error had to succeed, an exception should be propagated from the join() above
	// as this is not the case (there is no try/catch statement to catch it here) the inherited_run
	// succeeded but did not provide a libdar::entrepot as it should
    else
    {
	ret = entrep;
	entrep.reset();
    }

    return ret;
}

void html_entrepot::on_event(const std::string & event_name)
{
    got_inner_event = true;
    update_visible();
}

std::string html_entrepot::inherited_get_body_part(const chemin & path,
						   const request & req)
{
    got_inner_event = false;
    string ret = get_body_part_from_all_children(path, req);

    if(got_inner_event) // need get the update body part as some changed occured
    {
	request tmp = req;

	tmp.post_to_get();
	got_inner_event = false;
	ret = get_body_part_from_all_children(path, tmp);
	if(got_inner_event)
	    throw WEBDAR_BUG; // not expected to have changed event after visibility change only
    }

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

	    entrep.reset(new (nothrow) libdar::entrepot_libcurl(dialog,
								proto,
								login.get_value(),
								libdar::secu_string(pass.get_value().c_str(), pass.get_value().size()),
								host.get_value(),
								port.get_value(),
								auth_from_file.get_value_as_bool(),
								string("/root/.ssh/id_rsa.pub"), ///<< to review with env variables
								string("/root/.ssh/id_rsa"), ///< to review with env variables
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
	if(port.get_value().empty() || port.get_value() == "0")
	{
	    switch(repo_type.get_selected_num())
	    {
	    case 1:
		port.set_value("21");
		break;
	    case 2:
		port.set_value("22");
		break;
	    default:
		throw WEBDAR_BUG;
	    }
	}

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
		auth_from_file.set_visible(true);
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
