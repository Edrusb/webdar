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
#include "webdar_tools.hpp"
#include "webdar_css_style.hpp"


    //
#include "html_options_read.hpp"

using namespace std;

const string html_options_read::entrepot_has_changed = "entrep_has_changed";
const string html_options_read::ref_entrepot_has_changed = "ref_entrep_has_changed";

html_options_read::html_options_read():
    form_src("Update Options"),
    fs_src(""),
    fs_ref(""),
    src_crypto_algo("Cipher (for recent archives/backups, this is auto-detected, do not select any cipher)"),
    src_crypto_pass("Passphrase (will be asked later if not provided here)", html_form_input::password, "", "80%"),
    src_crypto_size("Cipher block size", html_form_input::number, "0", "80%"),
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
    ref_crypto_size("Cipher block size", html_form_input::number, "0", "80%"),
    ref_execute("Command to execute locally before reading each slice", html_form_input::text, "", "80%"),
    ref_slice_min_digits("Slice minimum digit", html_form_input::number, "0", "80%")
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

    ref_crypto_size.set_min_only(0);
    src_slice_min_digits.set_min_only(0);
    ref_slice_min_digits.set_min_only(0);

	// set default values from libdar

    libdar::archive_options_read defaults;

    src_crypto_algo.set_value(defaults.get_crypto_algo());
    src_crypto_pass.set_value("");
    src_crypto_size.set_min_only(defaults.get_crypto_size());
    src_crypto_size.set_value_as_int(defaults.get_crypto_size());
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
    ref_crypto_size.set_value(webdar_tools_convert_to_string(defaults.get_ref_crypto_size()));
    ref_execute.set_value(defaults.get_ref_execute());
    ref_slice_min_digits.set_value(webdar_tools_convert_to_string(defaults.get_ref_slice_min_digits()));

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

	// these are the same event name as the ones we used for ourself
    entrep->record_actor_on_event(this, entrepot_has_changed);
    ref_entrep->record_actor_on_event(this, ref_entrepot_has_changed);
    ref_path.record_actor_on_event(this, html_form_input_file::changed_entrepot);
    ref_path.record_actor_on_event(this, html_form_input_file::changed_event);
    ref_webui.record_actor_on_event(this, html_libdar_running_popup::libdar_has_finished);
    src_crypto_algo.record_actor_on_event(this, html_crypto_algo::changed);
    ref_use_external_catalogue.record_actor_on_event(this, html_form_input::changed);
    ref_crypto_algo.record_actor_on_event(this, html_crypto_algo::changed);

	// setting up our own events
    register_name(entrepot_has_changed);

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
    opts.set_crypto_size(webdar_tools_convert_to_int(src_crypto_size.get_value()));
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
	opts.set_ref_crypto_size(webdar_tools_convert_to_int(ref_crypto_size.get_value()));
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

void html_options_read::on_event(const string & event_name)
{
    if(event_name == entrepot_has_changed)
	act(entrepot_has_changed);
    else if(event_name == ref_entrepot_has_changed)
    {
	shared_ptr<html_web_user_interaction> localui = ref_webui.get_html_user_interaction();
	if(!localui)
	    throw WEBDAR_BUG;
	localui->auto_hide(true, true);
	localui->clear();
	if(is_running())
	    throw WEBDAR_BUG;
	join(); // in case a previous execution triggered an exception
	localui->run_and_control_thread(this);
	my_body_part_has_changed();
    }
    else if(event_name == html_libdar_running_popup::libdar_has_finished)
	join();
    else if(event_name == html_crypto_algo::changed
	    || event_name == html_form_input::changed
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
    }
    else if(event_name == html_form_input_file::changed_event)
    {
	if(! ref_path.get_min_digits().empty())
	    ref_slice_min_digits.set_value(ref_path.get_min_digits());
    }
    else
	throw WEBDAR_BUG; // unexpected event
}

string html_options_read::inherited_get_body_part(const chemin & path,
						  const request & req)
{
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
