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
#include <dar/tools.hpp>

    // webdar headers
#include "webdar_tools.hpp"
#include "webdar_css_style.hpp"


    //
#include "html_options_read.hpp"

using namespace std;

const string html_options_read::landing_path_changed = "landing_path_changed";
const string html_options_read::entrepot_has_changed = "entrep_has_changed";
const string html_options_read::ref_entrepot_landing_path_changed = "ref_landing_path_changed";
const string html_options_read::ref_entrepot_has_changed = "ref_entrep_has_changed";
const string html_options_read::changed = "horead_changed";

html_options_read::html_options_read():
    form_src("Update Options"),
    fs_src(""),
    fs_ref(""),
    src_crypto_algo("Cipher (for recent archives/backups, this is auto-detected, do not select any cipher)"),
    src_crypto_pass("Passphrase (will be asked later if not provided here)", html_form_input::password, "", "80%"),
    src_crypto_size("Cipher block size", 0, "80%"),
    src_ignore_sig_failure("Ignore signature failure", html_form_input::check, "", "1"),
    src_execute("Command to execute locally before reading each slice", html_form_input::text, "", "80%"),
    src_slice_min_digits("Slice minimum digit", html_form_input::number, "0", "80%"),
    info_details("Detailed information", html_form_input::check, "1", "1"),
    lax("Laxist check mode", html_form_input::check, "", "1"),
    sequential_read("Sequential read", html_form_input::check, "", "1"),
    force_first_slice("Start reading the first instead of the last (available when external catalog is used)", html_form_input::check, "", "1"),
    multi_thread_crypto("Number of thread for cryptography", html_form_input::number, "2", "5"),
    multi_thread_compress("Number of thread for decompression", html_form_input::number, "2", "5"),
    ref_use_external_catalogue("Use external catalog to open the archive", html_form_input::check, "", "1"),
    form_ref("Update Options"),
    ref_path("External catalog path", "/", "80%", "Select the external catalog..."),
    ref_crypto_algo("Cipher (for recent archives/backups, this is auto-detected, do not select any cipher)"),
    ref_crypto_pass("Passphrase", html_form_input::password, "", "80%"),
    ref_crypto_size("Cipher block size", 0, "80%"),
    ref_execute("Command to execute locally before reading each slice", html_form_input::text, "", "80%"),
    ref_slice_min_digits("Slice minimum digit", html_form_input::number, "0", "80%"),
    need_ref_entrepot_update(false),
    ignore_events(false)
{
    entrep.reset(new (nothrow) html_entrepot());
    if(!entrep)
	throw exception_memory();

    ref_entrep.reset(new (nothrow) html_entrepot());
    if(!ref_entrep)
	throw exception_memory();

    ref_path.set_select_mode(html_form_input_file::select_slice);
    ref_path.set_can_create_dir(false);
    multi_thread_crypto.set_min_only(1);
    multi_thread_compress.set_min_only(1);

    src_slice_min_digits.set_min_only(0);
    ref_slice_min_digits.set_min_only(0);

	// set default values from libdar

    set_defaults();

	// build the adoption tree
    deroule.add_section(sect_entrep, "Backup Location");
    deroule.add_section(sect_opt, "Reading options");
    deroule.add_section(sect_extcat, "External Catalog");
    deroule.add_section(sect_ref_entrep, "External Catalog location");

    deroule.adopt_in_section(sect_entrep, &guichet_entrep);

    fs_src.adopt(&src_crypto_algo);
    fs_src.adopt(&src_crypto_pass);
    fs_src.adopt(&src_crypto_size);
    fs_src.adopt(&src_ignore_sig_failure);
    fs_src.adopt(&src_execute);
    fs_src.adopt(&src_slice_min_digits);
    fs_src.adopt(&info_details);
    fs_src.adopt(&lax);
    fs_src.adopt(&sequential_read);
    fs_src.adopt(&force_first_slice);
    fs_src.adopt(&multi_thread_crypto);
    fs_src.adopt(&multi_thread_compress);
    form_src.adopt(&fs_src);
    deroule.adopt_in_section(sect_opt, &form_src);

    deroule.adopt_in_section(sect_ref_entrep, &guichet_ref_entrep);

    fs_ref.adopt(&ref_use_external_catalogue);
    fs_ref.adopt(&ref_path);
    fs_ref.adopt(&ref_crypto_algo);
    fs_ref.adopt(&ref_crypto_pass);
    fs_ref.adopt(&ref_crypto_size);
    fs_ref.adopt(&ref_execute);
    fs_ref.adopt(&ref_slice_min_digits);
    form_ref.adopt(&fs_ref);
    deroule.adopt_in_section(sect_extcat, &form_ref);

    adopt(&deroule);
    adopt(&ref_webui);

	// events

	// modyfing entrepot objects to be able to differentiate which one has changed:
    entrep->set_event_name(entrepot_has_changed);
    ref_entrep->set_event_name(ref_entrepot_has_changed);
    ref_entrep->set_event_landing_path(ref_entrepot_landing_path_changed);

	// these are the same event name as the ones we used for ourself
    entrep->record_actor_on_event(this, entrepot_has_changed);
    entrep->record_actor_on_event(this, html_entrepot::landing_path_changed);
    ref_entrep->record_actor_on_event(this, ref_entrepot_has_changed);
    ref_entrep->record_actor_on_event(this, ref_entrepot_landing_path_changed);


    ref_path.record_actor_on_event(this, html_form_input_file::changed_entrepot);
    ref_path.record_actor_on_event(this, html_form_input_file::changed_event);
    src_crypto_algo.record_actor_on_event(this, html_crypto_algo::changed);
    ref_use_external_catalogue.record_actor_on_event(this, html_form_input::changed);
    ref_crypto_algo.record_actor_on_event(this, html_crypto_algo::changed);

	// to track component changes from subcomponents changes
    src_crypto_pass.record_actor_on_event(this, html_form_input::changed);
    src_crypto_size.record_actor_on_event(this, html_form_input_unit::changed);
    src_ignore_sig_failure.record_actor_on_event(this, html_form_input::changed);
    src_execute.record_actor_on_event(this, html_form_input::changed);
    src_slice_min_digits.record_actor_on_event(this, html_form_input::changed);
    info_details.record_actor_on_event(this, html_form_input::changed);
    lax.record_actor_on_event(this, html_form_input::changed);
    sequential_read.record_actor_on_event(this, html_form_input::changed);
    force_first_slice.record_actor_on_event(this, html_form_input::changed);
    multi_thread_crypto.record_actor_on_event(this, html_form_input::changed);
    multi_thread_compress.record_actor_on_event(this, html_form_input::changed);
    ref_crypto_pass.record_actor_on_event(this, html_form_input::changed);
    ref_crypto_size.record_actor_on_event(this, html_form_input_unit::changed);
    ref_execute.record_actor_on_event(this, html_form_input::changed);
    ref_slice_min_digits.record_actor_on_event(this, html_form_input::changed);

	// setting up our own events
    register_name(landing_path_changed);
    register_name(entrepot_has_changed);
    register_name(changed);

	// manually launching on event to have coherent visibility between fields
    on_event(html_crypto_algo::changed);

	// visible
    ref_webui.set_visible(false);

	// css

    webdar_css_style::grey_button(deroule, true);
}

void html_options_read::set_biblio(const shared_ptr<bibliotheque> & ptr)
{
    guichet_entrep.set_child(ptr,
			     bibliotheque::repo,
			     entrep,
			     false);
    guichet_ref_entrep.set_child(ptr,
				 bibliotheque::repo,
				 ref_entrep,
				 false);
}

libdar::archive_options_read html_options_read::get_options(shared_ptr<html_web_user_interaction> & webui) const
{
    libdar::archive_options_read opts;

    opts.clear();
    opts.set_crypto_algo(src_crypto_algo.get_value());
    if(!src_crypto_pass.get_value().empty())
	opts.set_crypto_pass(libdar::secu_string(src_crypto_pass.get_value().c_str(), src_crypto_pass.get_value().size()));
    opts.set_crypto_size(webdar_tools_convert_from_infinint<libdar::U_32>(src_crypto_size.get_value_as_infinint(), "Error converting value of crypto block size"));
    opts.set_ignore_signature_check_failure(src_ignore_sig_failure.get_value_as_bool());
    opts.set_execute(src_execute.get_value());
    opts.set_slice_min_digits(libdar::infinint(webdar_tools_convert_to_int(src_slice_min_digits.get_value())));
    opts.set_entrepot(entrep->get_entrepot(webui));
    opts.set_info_details(info_details.get_value_as_bool());
    opts.set_lax(lax.get_value_as_bool());
    opts.set_sequential_read(sequential_read.get_value_as_bool());
    opts.set_multi_threaded_crypto(webdar_tools_convert_to_int(multi_thread_crypto.get_value()));
    opts.set_multi_threaded_compress(webdar_tools_convert_to_int(multi_thread_compress.get_value()));
    if(ref_use_external_catalogue.get_value_as_bool())
    {
	chemin chem(ref_path.get_value());

	if(chem.size() > 1)
	{
	    string basename = chem.back();
	    chem.pop_back();

	    opts.set_external_catalogue(chem.display(), basename);
	}
	else
	    opts.set_external_catalogue(string(""), chem.display());

	opts.set_ref_crypto_algo(ref_crypto_algo.get_value());
	if(!ref_crypto_pass.get_value().empty())
	    opts.set_ref_crypto_pass(libdar::secu_string(ref_crypto_pass.get_value().c_str(), ref_crypto_pass.get_value().size()));
	opts.set_ref_crypto_size(webdar_tools_convert_from_infinint<libdar::U_32>(ref_crypto_size.get_value_as_infinint(), "Error converting value of crypto block size for the reference catalog"));
	opts.set_ref_execute(ref_execute.get_value());
	opts.set_ref_slice_min_digits(libdar::infinint(webdar_tools_convert_to_int(ref_slice_min_digits.get_value())));
	opts.set_ref_entrepot(ref_entrep->get_entrepot(webui));
	opts.set_force_first_slice(force_first_slice.get_value_as_bool());
    }
    else
	opts.unset_external_catalogue();

    return opts;
}


void html_options_read::set_src_min_digits(const string & val)
{
    try
    {
	webdar_tools_convert_to_int(val);
    }
    catch(exception_base & e)
    {
	e.change_message(string("Invalid min-digits: ") + e.get_message());
	throw;
    }

    src_slice_min_digits.set_value(val);
}

void html_options_read::load_json(const json & source)
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
	    guichet_entrep.load_json(config.at(jlabel_entrep));
	    src_crypto_algo.set_selected_id(config.at(jlabel_crypto_algo));
	    src_crypto_pass.set_value(config.at(jlabel_crypto_pass));
	    src_crypto_size.set_value_as_infinint(libdar::deci(config.at(jlabel_crypto_size)).computer());
	    src_ignore_sig_failure.set_value_as_bool(config.at(jlabel_ignore_sig_failure));
	    src_execute.set_value(config.at(jlabel_execute));
	    src_slice_min_digits.set_value_as_bool(config.at(jlabel_slice_min_digits));
	    info_details.set_value_as_bool(config.at(jlabel_info_details));
	    lax.set_value_as_bool(config.at(jlabel_lax));
	    sequential_read.set_value_as_bool(config.at(jlabel_seq_read));
	    force_first_slice.set_value_as_bool(config.at(jlabel_force_first_slice));
	    multi_thread_crypto.set_value_as_int(config.at(jlabel_thread_crypto));
	    multi_thread_compress.set_value_as_bool(config.at(jlabel_thread_compress));

	    guichet_ref_entrep.load_json(config.at(jlabel_ref_entrep));
	    ref_use_external_catalogue.set_value_as_bool(config.at(jlabel_ref_used));
	    ref_path.set_value(config.at(jlabel_ref_path));
	    ref_crypto_algo.set_selected_id(config.at(jlabel_ref_crypto_algo));
	    ref_crypto_pass.set_value(config.at(jlabel_ref_crypto_pass));
	    ref_crypto_size.set_value_as_infinint(libdar::deci(config.at(jlabel_ref_crypto_size)).computer());
	    ref_execute.set_value(config.at(jlabel_ref_execute));
	    ref_slice_min_digits.set_value(config.at(jlabel_ref_slice_min_digits));
	}
	catch(...)
	{
	    ignore_events = false;
	    throw;
	}
	ignore_events = false;
	trigger_changed();
    }
    catch(json::exception & e)
    {
	throw exception_json(libdar::tools_printf("Error loading %s config", myclass_id), e);
    }
}

json html_options_read::save_json() const
{
    json config;

    config[jlabel_entrep] = guichet_entrep.save_json();
    config[jlabel_crypto_algo] = src_crypto_algo.get_selected_id();
    config[jlabel_crypto_pass] = src_crypto_pass.get_value();
    config[jlabel_crypto_size] = libdar::deci(src_crypto_size.get_value_as_infinint()).human();
    config[jlabel_ignore_sig_failure] = src_ignore_sig_failure.get_value_as_bool();
    config[jlabel_execute] = src_execute.get_value();
    config[jlabel_slice_min_digits] = src_slice_min_digits.get_value_as_bool();
    config[jlabel_info_details] = info_details.get_value_as_bool();
    config[jlabel_lax] = lax.get_value_as_bool();
    config[jlabel_seq_read] = sequential_read.get_value_as_bool();
    config[jlabel_force_first_slice] = force_first_slice.get_value_as_bool();
    config[jlabel_thread_crypto] = multi_thread_crypto.get_value_as_int();
    config[jlabel_thread_compress] = multi_thread_compress.get_value_as_bool();

    config[jlabel_ref_entrep] = guichet_ref_entrep.save_json();
    config[jlabel_ref_used] = ref_use_external_catalogue.get_value_as_bool();
    config[jlabel_ref_path] = ref_path.get_value();
    config[jlabel_ref_crypto_algo] = ref_crypto_algo.get_selected_id();
    config[jlabel_ref_crypto_pass] = ref_crypto_pass.get_value();
    config[jlabel_ref_crypto_size] = libdar::deci(ref_crypto_size.get_value_as_infinint()).human();
    config[jlabel_ref_execute] = ref_execute.get_value();
    config[jlabel_ref_slice_min_digits] = ref_slice_min_digits.get_value();

    return wrap_config_with_json_header(format_version,
					myclass_id,
					config);
}

void html_options_read::clear_json()
{
    ignore_events = true;
    try
    {
	set_defaults();
    }
    catch(...)
    {
	ignore_events = false;
	throw;
    }
    ignore_events = false;
    trigger_changed();
}

bibliotheque::using_set html_options_read::get_using_set() const
{
    bibliotheque::using_set ret;

    subconfig_add_to(ret, guichet_entrep.get_using_set());
    subconfig_add_to(ret, guichet_ref_entrep.get_using_set());

    return ret;
}

void html_options_read::on_event(const string & event_name)
{
    if(event_name == entrepot_has_changed)
    {
	act(entrepot_has_changed);
	trigger_changed();
    }
    else if(event_name == ref_entrepot_has_changed)
    {
	need_ref_entrepot_update = true;
    }
    else if(event_name == html_crypto_algo::changed
	    || event_name == html_form_input::changed
	    || event_name == html_form_input_unit::changed
	    || event_name == html_form_input_file::changed_entrepot)
    {
	if(ref_use_external_catalogue.get_value_as_bool())
	{
	    ref_path.set_visible(true);
	    ref_crypto_algo.set_visible(true);
	    ref_execute.set_visible(true);
	    ref_slice_min_digits.set_visible(true);
	    ref_crypto_pass.set_visible(true);
	    ref_crypto_size.set_visible(true);
	    deroule.section_set_visible(sect_ref_entrep, true);
	    force_first_slice.set_enabled(true);
	}
	else
	{
	    ref_path.set_visible(false);
	    ref_crypto_algo.set_visible(false);
	    ref_execute.set_visible(false);
	    ref_slice_min_digits.set_visible(false);
	    ref_crypto_pass.set_visible(false);
	    ref_crypto_size.set_visible(false);
	    deroule.section_set_visible(sect_ref_entrep, false);
	    force_first_slice.set_value_as_bool(false);
	    force_first_slice.set_enabled(false);
	}

	    // no need to call my_body_part_has_changed()
	    // because changed done in on_event concern
	    // body_builder objects we have adopted
	trigger_changed();
    }
    else if(event_name == html_form_input_file::changed_event)
    {
	if(! ref_path.get_min_digits().empty())
	    ref_slice_min_digits.set_value(ref_path.get_min_digits());
    }
    else if(event_name == html_entrepot::landing_path_changed)
	act(landing_path_changed); // propagate the event
    else if(event_name == ref_entrepot_landing_path_changed)
    {
	ref_path.set_value(ref_entrep->get_landing_path());
    }
    else
	throw WEBDAR_BUG; // unexpected event
}

void html_options_read::set_to_webdar_defaults()
{
    set_defaults(); // libdar defaults

    guichet_entrep.load_from_bibliotheque(bibliotheque::default_config_name);
    info_details.set_value_as_bool(true);
    multi_thread_crypto.set_value_as_int(2);
    multi_thread_compress.set_value_as_int(4);
    guichet_ref_entrep.load_from_bibliotheque(bibliotheque::default_config_name);
}

string html_options_read::inherited_get_body_part(const chemin & path,
						  const request & req)
{
    if(need_ref_entrepot_update)
	update_ref_entrepot();
    return get_body_part_from_all_children(path, req);
}


void html_options_read::new_css_library_available()
{
    unique_ptr<css_library> & csslib = lookup_css_library();
    if(!csslib)
	throw WEBDAR_BUG;

    webdar_css_style::update_library(*csslib);
}

void html_options_read::inherited_run()
{
	// below is a shortcut to the webui owned by ref_webui
    shared_ptr<html_web_user_interaction> localui = ref_webui.get_html_user_interaction();
    if(!localui)
	throw WEBDAR_BUG;

    ref_path.set_entrepot(ref_entrep->get_entrepot(localui));
}

void html_options_read::signaled_inherited_cancel()
{
    pthread_t libdar_tid;
    libdar::thread_cancellation th;

    if(is_running(libdar_tid))
	th.cancel(libdar_tid, true, 0);
}

void html_options_read::update_ref_entrepot()
{
    shared_ptr<html_web_user_interaction> localui = ref_webui.get_html_user_interaction();
    if(!localui)
	throw WEBDAR_BUG;

    localui->clear();
    localui->auto_hide(true, true);

    if(is_running())
	throw WEBDAR_BUG;
    ref_webui.set_visible(true);
    need_ref_entrepot_update = false;
    ref_webui.run_and_control_thread(this);
}

void html_options_read::set_defaults()
{
    libdar::archive_options_read defaults;

    src_crypto_algo.set_value(defaults.get_crypto_algo());
    src_crypto_pass.set_value("");
    src_crypto_size.set_value_as_infinint(defaults.get_crypto_size());
    src_execute.set_value(defaults.get_execute());
    src_slice_min_digits.set_value(webdar_tools_convert_to_string(defaults.get_slice_min_digits()));
    lax.set_value_as_bool(defaults.get_lax());
    sequential_read.set_value_as_bool(defaults.get_sequential_read());
    force_first_slice.set_value_as_bool(defaults.get_force_first_slice());
    ref_use_external_catalogue.set_value_as_bool(defaults.is_external_catalogue_set());
    if(ref_use_external_catalogue.get_value_as_bool())
	ref_path.set_value(defaults.get_ref_path().display());
    ref_crypto_algo.set_value(defaults.get_ref_crypto_algo());
    ref_crypto_pass.set_value("");
    ref_crypto_size.set_value_as_infinint(defaults.get_ref_crypto_size());
    ref_execute.set_value(defaults.get_ref_execute());
    ref_slice_min_digits.set_value(webdar_tools_convert_to_string(defaults.get_ref_slice_min_digits()));
}

void html_options_read::trigger_changed()
{
    if(! ignore_events)
	act(changed);
}
